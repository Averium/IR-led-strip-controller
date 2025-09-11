#ifndef RATE_LIMITED_FLOAT_H
#define RATE_LIMITED_FLOAT_H


typedef struct {
    float value;
    float target;
    float rate;
} RateLimitedFloat;


RateLimitedFloat RLF_init(const float initial_value, const float rate);
int RLF_update(RateLimitedFloat* object, const float dt);
void RLF_set_target(RateLimitedFloat* object, const float new_target);


#endif // RATE_LIMITED_FLOAT_H