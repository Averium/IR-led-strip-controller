#include "rate_limited_float.h"


unsigned int RLF_update(RateLimitedFloat* object, const float dt) {
    
    if (fabsf(object->value - object->target) < FLOAT_EPS) {
        object->value = object->target;
        return 0u;
    }

    // Calculate step so target is reached in object->ramp_time seconds
    if (object->target_changed_flag) {
        object->step = (object->target - object->value) * (dt / object->ramp_time);
        object->target_changed_flag = 0u;
    }

    object->value += object->step;
    
    // Clamp to target to avoid overshoot
    if ((object->step > 0.0f && object->value > object->target) ||
        (object->step < 0.0f && object->value < object->target)) {
        object->value = object->target;
    }

    return 1u;
}


void RLF_set_target(RateLimitedFloat* object, const float new_target) {
    object->target = new_target;
    object->target_changed_flag = 1u;
}
