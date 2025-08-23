#ifndef LED_CONFIG_H
#define LED_CONFIG_H

#include "driver/ledc.h"
#include "esp_timer.h"
#include "esp_log.h"

#include "config.h"


inline ledc_timer_config_t LED_timer_config(
    const unsigned int timer,
    const unsigned int resolution,
    const unsigned int frequency
) {
    return (ledc_timer_config_t) {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .timer_num        = timer,
        .duty_resolution  = resolution,
        .freq_hz          = frequency,
        .clk_cfg          = LEDC_AUTO_CLK,
    };
}


inline ledc_channel_config_t LED_channel_config(
    const unsigned int pin,
    const ledc_channel_t channel,
    const ledc_timer_config_t* timer
) {
    return (ledc_channel_config_t) {
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = channel,
        .timer_sel      = timer->timer_num,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = pin,
        .duty           = 0,
        .hpoint         = 0,
    };
}


#endif