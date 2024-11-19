#ifndef _UID_H_
#define _UID_H_
#include "stdbool.h"

#ifndef _IAP_H_
#define READ_UID 0x04
#endif

#ifndef _TA_
#define _TA_
#define TA_protected                                                                                                                                 \
	TA = 0xAA;                                                                                                                                       \
	TA = 0x55
#endif

uint8_t code addressUID[12] = {3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8};

#endif
