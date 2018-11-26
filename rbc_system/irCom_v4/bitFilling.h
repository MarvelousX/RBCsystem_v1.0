#ifndef __BITFILLING_H
#define __BITFILLING_H
#include"stdio.h"
#include"stdint.h"
#include"stdlib.h"

#define bitset(xbuff,posi,bit) \
do{ \
	xbuff[posi / 8] = \
	bit ? xbuff[posi / 8] | (0x80 >> (posi % 8)) : xbuff[posi / 8] & (~(0x80 >> (posi % 8))); \
} while (0)

#define bitget(xbuff,posi) ((xbuff[posi/8]&(0x80>>(posi%8)))?1:0)


uint16_t bitFillingEncode(unsigned char *outbuff, unsigned char *inbuff, uint16_t len);
uint16_t bitFillingDecode(unsigned char *outbuff, unsigned char *inbuff, uint16_t len);

#endif
