#ifndef LED_CONTROL_H
#define LED_CONTROL_H


#include "math.h"

#include "driver/ledc.h"
#include "IR_commands.h"

#include "config.h"
#include "rate_limited_float.h"


typedef struct {
    /* Data structure to store LED strip state */
    bool enabled;
    bool full_power;
    float brightness;
    float temperature;
} LEDState;


typedef struct {
    /* Return structure for LED_calculate_duty_cycles function */
    float cool;
    float warm;
} DutyCycles;


void LED_set_duty_cycle(const ledc_channel_t channel, const uint32_t duty_cycle);
void LED_update_state(uint32_t command, LEDState* state);
DutyCycles LED_calculate_duty_cycles(const LEDState* state);


#endif /* LED_CONTROL_H */