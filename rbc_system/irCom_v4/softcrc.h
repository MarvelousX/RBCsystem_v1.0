#ifndef __CRC_H
#define __CRC_H
#include "stdio.h"	
#include "stdint.h"

uint8_t crc8(uint8_t *ptr, uint8_t len);
uint16_t crc16_modbus(const uint8_t *bufData, uint8_t buflen);
#endif


