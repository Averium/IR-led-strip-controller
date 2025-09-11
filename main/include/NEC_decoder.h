#ifndef NEC_DECODER_H
#define NEC_DECODER_H


#include "driver/rmt_types.h"
#include <stdbool.h>
#include <stdint.h>


#define NEC_HEADER_HIGH_US   9000
#define NEC_HEADER_LOW_US    4500
#define NEC_BIT_MARK_US       560
#define NEC_ONE_SPACE_US     1690
#define NEC_ZERO_SPACE_US     560
#define NEC_REPEAT_SPACE_US  2250
#define NEC_TOLERANCE_PCT    0.2f  // tolerance for us in percentage


static inline bool within(uint32_t value, uint32_t target, const float tolerance) {
    /* Check if <value> is within <tolerance> percentage of <target> value */

    const uint32_t lower = target - (uint32_t)(target * tolerance);
    const uint32_t upper = target + (uint32_t)(target * tolerance);

    return (value >= lower) && (value <= upper);
}


bool NEC_decode_frame(
    const rmt_symbol_word_t *symbols,
    const size_t num_symbols,
    uint32_t *out_address,
    uint32_t *out_command
);


#endif /* NEC_DECODER_H */