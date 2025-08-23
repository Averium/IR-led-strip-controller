#include "rate_limited_float.h"


RateLimitedFloat RLF_init(const float initial_value, const float rate) {
    return (RateLimitedFloat) { initial_value, initial_value, rate };
}


void RLF_update(RateLimitedFloat* object, const float dt) {
    const float difference = object->target - object->value;
    const float step_limit = object->rate * dt;
    
    float step;

    if (difference > 0.0f) {
        step = (difference < step_limit) ? difference : step_limit;
    }
    else {
        step = (difference > -step_limit) ? difference : -step_limit;
    }

    object->value += step;
}


void RLF_set_target(RateLimitedFloat* object, const float new_target) {
    object->target = new_target;
}
