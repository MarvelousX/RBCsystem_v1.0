#ifndef __IRPACK_H
#define __IRPACK_H
#include "softcrc.h"
#include "bitFilling.h"
#include "../bsp/bsp.h"

#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "stm32l0xx_hal.h"


//#define IR_ROLE_MASTER
//#define IR_UART_SEND(xdata,xlen) do{ HAL_IRDA_Transmit(&hirda3, xdata, xlen, 0xFFFF); }while(0)
#define IR_UART_SEND(xdata,xlen) do{Bsp_IrModuleSendData(xdata,xlen); }while(0)
#define IRPACK_MAX_BUFF	(50)

typedef enum{
	U_RX_STA_S,   
	U_RX_STA_D,  
} irPackRxState_t;

typedef enum 
{
	IRPACK_OK,
	IRPACK_FAILED
} irPackRet_t;

typedef uint8_t irPackAddr_t;
typedef uint8_t irPackCtrl_t;
typedef uint16_t irPackCrc_t;
typedef uint8_t irPackDataLen_t;

typedef struct 
{
	struct{
		irPackAddr_t addr;
		irPackCtrl_t ctrl;
		//irPackCrc_t crc;	这里不需要crc这项
		irPackDataLen_t dataLen;
	} head;
	uint8_t data[IRPACK_MAX_BUFF];
} irPack_t;

irPackRet_t irPackSend(irPack_t *irPack);

irPackRet_t irPackRecv(irPack_t *irPack);

void irPackRecvCallbk(uint8_t recvByte);

#endif
