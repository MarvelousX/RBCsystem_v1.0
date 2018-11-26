#include "bitfilling.h"
#include"stdio.h"
#include"stdint.h"
#include"stdlib.h"

/*
## bit insertion
@ outbuff: buff for encoded data
@ inbuff : buff for source data
@ len : byte length of inbuff
return: byte length of outbuff
*/
uint16_t bitFillingEncode(unsigned char *outbuff, unsigned char *inbuff, uint16_t len)
{
	uint16_t i_read=0;
	uint16_t i_write=0;
	uint8_t ones_ctn=0;
	uint8_t bit=0,bit_before=0;

	while(i_read<len*8){
		bit = bitget(inbuff, i_read); i_read++;
		bitset(outbuff, i_write, bit); i_write++;
		if (bit == 1){
			if (bit_before == 1){
				if (++ones_ctn == 5){
					ones_ctn = 0;
					bitset(outbuff, i_write, 0); i_write++;
				}
			}
			else{
				ones_ctn = 1;
			}
		}
		else{
			ones_ctn = 0;
		}
		bit_before = bit;
	}

	while(i_write%8!=0){
		bitset(outbuff, i_write, 0); i_write++;
	}
	return  i_write/8;
}

/*
## bit insertion
@ outbuff : buff for source data
@ inbuff  : buff for encoded data
@ len : byte length of inbuff
return: byte length of outbuff
*/
uint16_t bitFillingDecode(unsigned char *outbuff, unsigned char *inbuff, uint16_t len)
{
	uint16_t i_read=0;
	uint16_t i_write=0;
	uint8_t ones_ctn=0;
	uint8_t bit = 0, bit_before = 0;

	while(i_read<len*8){
		bit = bitget(inbuff, i_read); 
		i_read++;
		bitset(outbuff, i_write, bit); 
		i_write++;
		if(bit==1){
			if (bit_before == 1){
				if(++ones_ctn==5){
					ones_ctn=0;
					i_read++;
				}
			}
			else{
				ones_ctn = 1;
			}
		}
		else{
			ones_ctn = 0;
		}
		bit_before = bit;
	}
	return  i_write/8;
}

