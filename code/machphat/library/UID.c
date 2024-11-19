#ifndef _UID_C_
#define _UID_C_
#include "UID.h"

void Read_UID(uint8_t *uid) {
	bool EA_temp = EA;

	uint8_t i = 0;

	EA = 0;

	TA_protected;
	CHPCON |= SET_BIT0;

	IAPCN = READ_UID; // CMD IAP
	                  // Address
	IAPAH = 0x00;
	IAPAL = 0x00;

	for (i = 0; i < 12; i++, uid++) {
		IAPAL = addressUID[i];
		TA_protected;
		IAPTRG |= SET_BIT0; // IAP Go
		*uid = IAPFD;
	}

	TA_protected;
	CHPCON &= ~SET_BIT0;
	EA = EA_temp;
}

#endif
