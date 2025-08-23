#ifndef CONFIG_H
#define CONFIG_H


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "IR_commands.h"


/* DO NOT MODIFY ------ */
#define CONTROL_PIN_COOL 0
#define CONTROL_PIN_WARM 1
#define RMT_RX_GPIO 2

#define FUZZY_MIN 0.0f
#define FUZZY_MAX 1.0f

#define loop while(true)
/* DO NOT MODIFY ------ */


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

#endif /* CONFIG_H */