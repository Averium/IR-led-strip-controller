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


void LED_state_transition(const IRCommand command, LEDData* data) {

    switch (data->state) {

        case IDLE:
            if      (command == CMD_ON)     { data->state = OPERATION; }
            else if (command == CMD_SEG)    { data->state = POWER;     }
            else if (command == CMD_NIGHT)  { data->state = NIGHT;     }
            break;

        case OPERATION:
            if      (command == CMD_OFF)    { data->state = IDLE;      }
            else if (command == CMD_SEG)    { data->state = POWER;     }
            else if (command == CMD_NIGHT)  { data->state = NIGHT;     }
            break;

        case POWER:
            if      (command == CMD_OFF)    { data->state = IDLE;      }
            break;

        case NIGHT:
            if      (command == CMD_OFF)    { data->state = IDLE;      }
            else if (command == CMD_NIGHT)  { data->state = IDLE;      }
            else if (command == CMD_DIMM_N || command == CMD_DIMM_P) {
                data->brightness = BRIGHTNESS_MIN;
                data->state = OPERATION;
            }
            else                            { data->state = OPERATION; }
    }
} 


void LED_state_operation(const IRCommand command, LEDData* data) {

    if (data->state != OPERATION) { return; }

    switch (command) {

        case CMD_DIMM_P: // Increase brightness
            data->brightness = fuzzy_increment(data->brightness, BRIGHTNESS_STEP);
            break;
        case CMD_DIMM_N: // Decrease brightness unitl BRIGHTNESS_MIN is reached
            data->brightness = fuzzy_decrement(data->brightness, BRIGHTNESS_STEP);
            if (data->brightness < BRIGHTNESS_MIN) { data->brightness = BRIGHTNESS_MIN; }
            break;
        case CMD_TEMP_P: // Increase color temperature
            data->temperature = fuzzy_increment(data->temperature, TEMPERATURE_STEP);
            break;
        case CMD_TEMP_N: // Decrease color temperature
            data->temperature = fuzzy_decrement(data->temperature, TEMPERATURE_STEP);
            break;
        case CMD_6500K:  // Set color to coldest
            data->temperature = FUZZY_MIN;
            break;
        case CMD_4000K:  // Set color half way between cold and warm 
            data->temperature = FUZZY_MAX * 0.5f;
            break;
        case CMD_3000K:  // Set color to warmest
            data->temperature = FUZZY_MAX;
            break;
    }
}


DutyCycles LED_calculate_duty_cycles(const LEDData* data) {

    float brightness = data->brightness;

    switch (data->state) {
        case IDLE:
            return (DutyCycles) { 0.0f, 0.0f };

        case POWER:
            return (DutyCycles) { DUTY_MAX, DUTY_MAX };

        case NIGHT:
            brightness = BRIGHTNESS_MIN;
    }

    /* Apply exponential function for more natural dimming experience */
    brightness = powf(data->brightness, BRIGHTNESS_EXPONENT);

    const float duty_cool = DUTY_MAX * brightness * data->temperature;
    const float duty_warm = DUTY_MAX * brightness * (1.0f - data->temperature);

    return (DutyCycles) { duty_cool, duty_warm };
}
