#include "LED_control.h"


static float fuzzy_increment(const float value, const float step) {
    const float result = value + step;
    return result > FUZZY_MAX ? FUZZY_MAX : result;
}


static float fuzzy_decrement(const float value, const float step) {
    const float result = value - step;
    return result < FUZZY_MIN ? FUZZY_MIN : result;
}


void LED_set_duty_cycle(
    const ledc_channel_t channel,
    const uint32_t duty_cycle
) {
    ledc_set_duty(LEDC_LOW_SPEED_MODE, channel, duty_cycle);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, channel);
}


void LED_update_state(uint32_t command, LEDState* state) {

    state->full_power = false;

    switch (command) {

        case CMD_ON:     // Turn on the led strip
            state->enabled = true;
            break;
        case CMD_OFF:    // Turn off the led strip
            state->enabled = false;
            break;
        case CMD_DIMM_P: // Increase brightness
            if (state->enabled) { state->brightness = fuzzy_increment(state->brightness, BRIGHTNESS_STEP); }
            break;
        case CMD_DIMM_N: // Decrease brightness unitl BRIGHTNESS_MIN is reached
            if (state->enabled) { state->brightness = fuzzy_decrement(state->brightness, BRIGHTNESS_STEP); }
            if (state->brightness < BRIGHTNESS_MIN) { state->brightness = BRIGHTNESS_MIN; }
            break;
        case CMD_TEMP_P: // Increase color temperature
            if (state->enabled) { state->temperature = fuzzy_increment(state->temperature, TEMPERATURE_STEP); }
            break;
        case CMD_TEMP_N: // Decrease color temperature
            if (state->enabled) { state->temperature = fuzzy_decrement(state->temperature, TEMPERATURE_STEP); }
            break;
        case CMD_6500K:  // Set color to coldest
            if (state->enabled) { state->temperature = FUZZY_MIN; }
            break;
        case CMD_4000K:  // Set color half way between cold and warm 
            if (state->enabled) { state->temperature = FUZZY_MAX * 0.5f; }
            break;
        case CMD_3000K:  // Set color to warmest
            if (state->enabled) { state->temperature = FUZZY_MAX; }
            break;
        case CMD_SEG:    // Turn on "full power mode" (set both channels on full power)
            if (state->enabled) { state->full_power = true; }
            break;
        case CMD_NIGHT:  // Turn ON the led strip and set brightness to minimum
            state->brightness = BRIGHTNESS_MIN;
            state->enabled = true;
            break;
        default:
            break;
    }
}


DutyCycles LED_calculate_duty_cycles(const LEDState* state) {

    if (!state->enabled) {
        return (DutyCycles) { 0.0f, 0.0f };
    }

    if (state->full_power) {
        return (DutyCycles) { DUTY_MAX, DUTY_MAX };
    }

    /* Apply exponential function for more natural dimming experience */
    const float brightness = powf(state->brightness, BRIGHTNESS_EXPONENT);

    const float duty_cool = DUTY_MAX * brightness * state->temperature;
    const float duty_warm = DUTY_MAX * brightness * (1.0f - state->temperature);

    return (DutyCycles) { duty_cool, duty_warm };
}
