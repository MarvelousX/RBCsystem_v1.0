#ifndef __IRCOM_H
#define __IRCOM_H
#include"stdio.h"
#include "stdint.h"
#include "stm32l0xx_hal.h"
//#include"time.h"

#define IRCOM_SYSTIME	HAL_GetTick()	//NOTE: get system time  
#define IRCOM_MAX_CMDNUM	(7)
#define IRCOM_MAX_BUFF (50)

enum IRCOM_RET_N {
	IRCOM_OK,
	IRCOM_BUSY,
	IRCOM_FAILED,
	IRCOM_NOTRECV,
	IRCOM_TIMEOUT,
	IRCOM_ERR_CLOSED,
	IRCOM_ERR_ROLE,
	IRCOM_ERR_FUN
};

enum IRCOM_STATE_N
{
	IRCOM_OPENED,
	IRCOM_CLOSEED
};

enum IRCOM_ROLE_N
{
	IRCOM_MASTER,
	IRCOM_SLAVE
};

typedef struct irCom_t irCom_t;

typedef uint32_t irComClock_t;	//NOTE:  system time type
typedef uint8_t irComId_t;
typedef uint8_t irComRet_t;
typedef uint8_t irComState_t;
typedef uint8_t irComRole_t;
typedef uint8_t irComCmd_t;
typedef uint8_t irComBuffSize_t;
typedef uint8_t irComBuffState_t;
typedef uint16_t irComTimeOutMs_t;
typedef irComRet_t (*irComFunRecvResp_t)(irCom_t*);

typedef struct irCom_t {
	irComId_t irComId;
	irComCmd_t command;
	//uint8_t *buff;    **old -xmy
	uint8_t buff[IRCOM_MAX_BUFF];
	irComBuffSize_t datalen;
	irComState_t state;
	irComRole_t role;
  
#ifdef IRCOM_RECVMODE_CALLBACK
	void(*irComRrcvCallBack)(irCom_t *hircom);
#endif // IRCOM_RECVMODE_CALLBACK

	irComFunRecvResp_t pFunSlaveResp[IRCOM_MAX_CMDNUM];
}irCom_t;

irComRet_t irCom_Init(irCom_t *hircom, irComRole_t role);
irComRet_t irCom_SlaveFunConfig(irCom_t *hircom, irComCmd_t command, irComFunRecvResp_t pFun);
irComRet_t irCom_Open(irCom_t *hircom); 
irComRet_t irCom_Close(irCom_t *hircom); 
irComRet_t irCom_Command(irCom_t *hircom, irComCmd_t command, uint8_t* data,
			irComBuffSize_t len, irComTimeOutMs_t timeout);  
irComRet_t irCom_Response(irCom_t *hircom, irComCmd_t command, uint8_t* data, irComBuffSize_t len); 
irComRet_t irCom_ListenOnce(irCom_t *hircom); 
#endif // !__IRCOM_H
