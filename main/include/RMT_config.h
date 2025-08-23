#ifndef RMT_CONFIG_H
#define RMT_CONFIG_H

#include "driver/rmt_rx.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_check.h"


inline rmt_rx_channel_config_t RMT_rx_channel_config(
    const gpio_num_t input_pin,
    const uint32_t resolution_frequency_hz
) {
    return (rmt_rx_channel_config_t) {
        .gpio_num = input_pin,
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = resolution_frequency_hz,
        .mem_block_symbols = 64,
        .flags = { 0, 0, 0 },
    };
}

inline rmt_carrier_config_t RMT_carrier_config(const uint32_t carrier_frequency_hz) {
    return (rmt_carrier_config_t) {
        .frequency_hz = carrier_frequency_hz,
        .duty_cycle = 0.33f,
        .flags = {0, 1},
    };
}


inline rmt_receive_config_t RMT_signal_config(const uint32_t range_min_ns, const uint32_t range_max_ms) {
    return (rmt_receive_config_t) {
        .signal_range_min_ns = range_min_ns,
        .signal_range_max_ns = range_max_ms,
    };
}


#endif /* RMT_CONFIG_H */