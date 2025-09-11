#ifndef COMMANDS_H
#define COMMANDS_H


typedef uint32_t IRCommand;

/* Decoded values from IR remote controller of MÖMAX lamp */

/* DO NOT MODIFY ---- */
#define CMD_REPEAT  ((IRCommand)(0xFF))
#define CMD_ON	    ((IRCommand)(0x30))
#define CMD_OFF	    ((IRCommand)(0x88))
#define CMD_TEMP_P	((IRCommand)(0xF0))
#define CMD_TEMP_N	((IRCommand)(0xE0))
#define CMD_DIMM_P	((IRCommand)(0x18))
#define CMD_DIMM_N	((IRCommand)(0xF8))
#define CMD_SEG	    ((IRCommand)(0xB8))
#define CMD_3000K	((IRCommand)(0xC0))
#define CMD_4000K	((IRCommand)(0x68))
#define CMD_6500K	((IRCommand)(0x98))
#define CMD_NIGHT	((IRCommand)(0x48))
#define CMD_TIMER	((IRCommand)(0x50))
/* DO NOT MODIFY ---- */

#endif