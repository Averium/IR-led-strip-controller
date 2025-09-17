#ifndef RATE_LIMITED_FLOAT_H
#define RATE_LIMITED_FLOAT_H


#include <math.h>


#define FLOAT_EPS 1e-6f


typedef struct {
    float ramp_time;
    float value;
    float target;
    float step;
    unsigned int target_changed_flag;
} RateLimitedFloat;


unsigned int RLF_update(RateLimitedFloat* object, const float dt);
void RLF_set_target(RateLimitedFloat* object, const float new_target);


#endif // RATE_LIMITED_FLOAT_H