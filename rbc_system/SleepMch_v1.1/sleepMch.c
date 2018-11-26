#include "sleepMch.h"

typedef enum{
	SLEEP_TIMER_OFF,
	SLEEP_TIMER_ON
}sleepTimerState_t;

sleepTimerState_t timState=SLEEP_TIMER_OFF;

uint32_t downCounterTmStamp;
uint32_t downCounterValueMs;
uint8_t  light_sl_ctn=0;
sleepMode_t sleepMode;

uint32_t SleepMch_TimerCheck()
{
	uint32_t timeDiff = HAL_GetTick()-downCounterTmStamp;
	printf("tn:%d, ts:%d, diff:%d \n",HAL_GetTick(),downCounterTmStamp,timeDiff);
	if(timeDiff >= downCounterValueMs){
		return 0;
	}
	return downCounterValueMs-timeDiff;
}

void SleepMch_TimerRestart(uint32_t setCountMs)
{
	downCounterValueMs = setCountMs;
	downCounterTmStamp = HAL_GetTick();
	timState=SLEEP_TIMER_ON;
}

void SleepMch_StateAwake()
{
	sleepMode=SLEEP_AWAKE;
	light_sl_ctn=0;
}

toSleepCmd_t SleepMch_GetSleepCmd()
{
	if(SLEEP_TIMER_OFF==timState){
		return SLEEPCMD_ERR_TIMEROFF;
	}
	
	if(0==SleepMch_TimerCheck()){
		timState=SLEEP_TIMER_OFF;
		switch(sleepMode){
		case SLEEP_AWAKE:
			sleepMode=SLEEP_LIGHT;
			return SLEEPCMD_TOLIGHT;
		
			break;
		case SLEEP_LIGHT:
			if(++light_sl_ctn<5){
				return SLEEPCMD_TOLIGHT;
			}
			else{
				sleepMode=SLEEP_DEEP;
				light_sl_ctn=0;
				return SLEEPCMD_TODEEP;
			}
			break;
		case SLEEP_DEEP:
			return SLEEPCMD_TODEEP;
		
			break;
		}
	}
	return SLEEPCMD_KEEP;
}

