#ifndef COMMANDS_H
#define COMMANDS_H

/* Decoded values from IR remote controller of MÖMAX lamp */

/* DO NOT MODIFY ---- */
#define CMD_REPEAT  ((uint32_t)(0xFF))
#define CMD_ON	    ((uint32_t)(0x30))
#define CMD_OFF	    ((uint32_t)(0x88))
#define CMD_TEMP_P	((uint32_t)(0xF0))
#define CMD_TEMP_N	((uint32_t)(0xE0))
#define CMD_DIMM_P	((uint32_t)(0x18))
#define CMD_DIMM_N	((uint32_t)(0xF8))
#define CMD_SEG	    ((uint32_t)(0xB8))
#define CMD_3000K	((uint32_t)(0xC0))
#define CMD_4000K	((uint32_t)(0x68))
#define CMD_6500K	((uint32_t)(0x98))
#define CMD_NIGHT	((uint32_t)(0x48))
#define CMD_TIMER	((uint32_t)(0x50))
/* DO NOT MODIFY ---- */

#endif