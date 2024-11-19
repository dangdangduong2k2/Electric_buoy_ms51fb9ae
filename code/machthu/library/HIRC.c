#ifndef _HIRC_C_
#define _HIRC_C_
#include "HIRC.h"

void MODIFY_HIRC_166(void) {
	uint8_t  RC_L;
	uint8_t  RC_H;
	uint16_t new_rc;
	/* Check if power on reset, modify HIRC */
	if ((PCON & SET_BIT4) == SET_BIT4) {
		RC_L    = RCTRIM0;
		RC_H    = RCTRIM1;
		new_rc  = ((RC_L << 1) + (RC_H & 0x01));
		new_rc  = new_rc - 15;
		RC_H    = new_rc & 0x01;
		RC_L    = new_rc >> 1;
		TA      = 0XAA;
		TA      = 0X55;
		RCTRIM0 = RC_L;
		TA      = 0XAA;
		TA      = 0X55;
		RCTRIM1 = RC_H;
		/* Clear power on flag */
		PCON &= CLR_BIT4;
	}
}
#endif
