#ifndef LED_CONTROL_H
#define LED_CONTROL_H


#include "math.h"

#include "driver/ledc.h"
#include "IR_commands.h"

#include "config.h"
#include "rate_limited_float.h"


typedef enum {
    IDLE,
    OPERATION,
    POWER,
    NIGHT
} LEDState;


typedef struct {
    /* Data structure to store LED strip state */
    float brightness;
    float temperature;
    LEDState state;
} LEDData;


typedef struct {
    /* Return structure for LED_calculate_duty_cycles function */
    float white;
    float yellow;
} DutyCycles;


void LED_set_duty_cycle(const ledc_channel_t channel, const uint32_t duty_cycle);
void LED_state_operation(const uint32_t command, LEDData* state);
void LED_state_transition(const uint32_t command, LEDData* data);
DutyCycles LED_calculate_duty_cycles(const LEDData* state);


#endif /* LED_CONTROL_H */