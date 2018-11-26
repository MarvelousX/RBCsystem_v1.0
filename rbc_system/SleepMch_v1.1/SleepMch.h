#ifndef __SLEEPMCH_H
#define __SLEEPMCH_H
#include "stdio.h"
#include "stdint.h"
#include "stm32f4xx_hal.h" 

#define BSP_SLEEP_TIME_LIGHT	(5000)		// 5  sec light sleep
#define BSP_SLEEP_TIME_DEEP		(15000)		// 15 sec deep sleep

typedef uint16_t miniSec_t;

typedef enum
{	
	SLEEP_AWAKE,
	SLEEP_LIGHT,
	SLEEP_DEEP
}sleepMode_t;
typedef enum
{
	SLEEPCMD_KEEP,
	SLEEPCMD_TOLIGHT,
	SLEEPCMD_TODEEP,
	SLEEPCMD_ERR_TIMEROFF,
}toSleepCmd_t;

/*------sleep entity-------*/
void SleepMch_TimerRestart(uint32_t setCountMs);
void SleepMch_StateAwake();
//void SleepMch_Start();
//void SleepMch_Stop();
toSleepCmd_t SleepMch_GetSleepCmd();
#endif