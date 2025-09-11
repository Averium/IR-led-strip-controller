#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/ledc.h"
#include "driver/rmt_rx.h"
#include "driver/gpio.h"

#include "esp_timer.h"
#include "esp_check.h"
#include "esp_log.h"

#include "config.h"

#include "LED_config.h"
#include "RMT_config.h"
#include "NEC_decoder.h"
#include "LED_control.h"


#define LED_TIMER            LEDC_TIMER_0
#define LED_CHANNEL_COOL     LEDC_CHANNEL_0    
#define LED_CHANNEL_WARM     LEDC_CHANNEL_1


static const char *TAG = "IR LED driver";


// LED driver config
ledc_timer_config_t common_timer;
ledc_channel_config_t channel_cool;
ledc_channel_config_t channel_warm;

/* IR COMMUNICATION CONFIG */
rmt_channel_handle_t rx_channel_handle = NULL;
rmt_rx_channel_config_t rx_channel_config;
rmt_carrier_config_t rx_carrier_config;
rmt_rx_event_callbacks_t rx_event_callbacks;

rmt_receive_config_t rx_config;
QueueHandle_t rx_queue_handle;


typedef struct {
    RateLimitedFloat* duty_cool;
    RateLimitedFloat* duty_warm;
    LEDData* state;
} SharedData;


static bool rx_done_callback(
    rmt_channel_handle_t channel,
    const rmt_rx_done_event_data_t* event_data,
    void* user_data
) {
    BaseType_t high_task_wakeup = pdFALSE;
    QueueHandle_t queue = ((QueueHandle_t)(user_data));
    xQueueSendFromISR(queue, event_data, &high_task_wakeup);

    return high_task_wakeup == pdTRUE;
}


void IR_receiver_task(void* shared) {

    SharedData* shared_data = ((SharedData*)(shared));

    RateLimitedFloat* duty_cool = shared_data->duty_cool;
    RateLimitedFloat* duty_warm = shared_data->duty_warm;
    LEDData* state = shared_data->state;

    uint32_t command_memory;
    rmt_rx_done_event_data_t rx_data;
    rmt_symbol_word_t raw_symbols[RMT_SYMBOL_BUFFER_SIZE];
    
    loop {

        ESP_ERROR_CHECK(rmt_receive(rx_channel_handle, raw_symbols, sizeof(raw_symbols), &rx_config));
        
        if (xQueueReceive(rx_queue_handle, &rx_data, portMAX_DELAY) == pdTRUE) {

            uint32_t address;
            uint32_t command;

            if (NEC_decode_frame(rx_data.received_symbols, rx_data.num_symbols, &address, &command)) {

                if (command == CMD_REPEAT) { command = command_memory; }
                else                       { command_memory = command; }

                LED_state_transition(command, state);
                LED_state_operation(command, state);

                const DutyCycles control = LED_calculate_duty_cycles(state);

                RLF_set_target(duty_cool, control.cool);
                RLF_set_target(duty_warm, control.warm);                

            }
        }
    }
}


void LED_control_task(void* shared) {

    RateLimitedFloat *duty_cool = ((SharedData*)shared)->duty_cool;
    RateLimitedFloat *duty_warm = ((SharedData*)shared)->duty_warm;

    const TickType_t period = pdMS_TO_TICKS(CONTROL_REFRESH_PERIOD_MS);

    TickType_t last_wake = xTaskGetTickCount();

    loop {
        vTaskDelayUntil(&last_wake, period);
        float dt = ((float)(period)) * MS_TO_S;

        RLF_update(duty_cool, dt);
        RLF_update(duty_warm, dt);

        LED_set_duty_cycle(LED_CHANNEL_COOL, duty_cool->value);
        LED_set_duty_cycle(LED_CHANNEL_WARM, duty_warm->value);
    }
}


void app_main(void)
{
    // Set log level
    esp_log_level_set("*", ESP_LOG_ERROR);
    esp_log_level_set(TAG, ESP_LOG_INFO);

    // LED driver config
    common_timer = LED_timer_config(LED_TIMER, LED_TIMER_RESOLUTION, LED_FREQUENCY);
    channel_cool = LED_channel_config(CONTROL_PIN_COOL, LED_CHANNEL_COOL, &common_timer);
    channel_warm = LED_channel_config(CONTROL_PIN_WARM, LED_CHANNEL_WARM, &common_timer);

    /* IR COMMUNICATION CONFIG */
    rx_channel_config = RMT_rx_channel_config(RMT_RX_GPIO, RMT_RESOLUTION_HZ);
    rx_carrier_config = RMT_carrier_config(RMT_CARRIER_FREQUENCY_HZ);
    rx_event_callbacks.on_recv_done = rx_done_callback;
    rx_config = RMT_signal_config(2500, 12000000);

    rx_queue_handle = xQueueCreate(1, sizeof(rmt_rx_done_event_data_t));

    ESP_ERROR_CHECK(ledc_timer_config(&common_timer));
    ESP_ERROR_CHECK(ledc_channel_config(&channel_cool));
    ESP_ERROR_CHECK(ledc_channel_config(&channel_warm));

    ESP_ERROR_CHECK(rmt_new_rx_channel(&rx_channel_config, &rx_channel_handle));
    ESP_ERROR_CHECK(rmt_apply_carrier(rx_channel_handle, &rx_carrier_config));
    ESP_ERROR_CHECK(rmt_enable(rx_channel_handle));
    ESP_ERROR_CHECK(rmt_rx_register_event_callbacks(rx_channel_handle, &rx_event_callbacks, rx_queue_handle));
    
    LED_set_duty_cycle(LED_CHANNEL_COOL, 0);
    LED_set_duty_cycle(LED_CHANNEL_WARM, 0);

    LEDData state = {
        .enabled = false,
        .brightness = BRIGHTNESS_MIN,
        .temperature = FUZZY_MAX * 0.5f,
    };
    
    RateLimitedFloat duty_cool = RLF_init(0.0f, 2.0f);
    RateLimitedFloat duty_warm = RLF_init(0.0f, 2.0f);

    SharedData shared = {
        .duty_cool = &duty_cool,
        .duty_warm = &duty_warm,
        .state = &state,
    };

    xTaskCreate(LED_control_task, "LED_control_task", LED_TASK_STACK_DEPTH, &shared, 5, NULL); 
    xTaskCreate(IR_receiver_task, "IR_receiver_task", IR_TASK_STACK_DEPTH, &shared, 5, NULL);
}
