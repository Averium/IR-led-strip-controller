#ifndef CONFIG_H
#define CONFIG_H


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "IR_commands.h"


/* DO NOT MODIFY ------ */
#define CONTROL_PIN_YELLOW 0
#define CONTROL_PIN_WHITE 1
#define RMT_RX_GPIO 2

#define FUZZY_MIN 0.0f
#define FUZZY_MAX 1.0f

#define US_TO_S 0.000001f
#define MS_TO_S 0.001f

#define loop while(true)
/* DO NOT MODIFY ------ */

#define LOG_LEVEL ESP_LOG_ERROR

#define LED_TIMER_RESOLUTION LEDC_TIMER_10_BIT
#define LED_FREQUENCY 30000
#define DUTY_MAX 1024.0f

#define RMT_RESOLUTION_HZ 1000000
#define RMT_CARRIER_FREQUENCY_HZ 38000
#define RMT_SYMBOL_BUFFER_SIZE 64

#define BRIGHTNESS_STEP 0.02f
#define BRIGHTNESS_MIN 0.25f
#define BRIGHTNESS_EXPONENT 2.2f
#define TEMPERATURE_STEP 0.05f
#define PWM_RAMP_TIME 0.25f  // time to ramp from ZERO to DUTY_MAX in seconds

#define CONTROL_REFRESH_PERIOD_MS 10 // 100 Hz update

#define IR_TASK_STACK_DEPTH 4096 // in words (1 word = 4 byte on 32 bit ESP)
#define LED_TASK_STACK_DEPTH 4096 // in words

#endif /* CONFIG_H */