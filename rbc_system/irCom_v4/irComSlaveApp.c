#include "irComSlaveApp.h"
#include "stdio.h"


#include "BSP.h"
#include "SleeepMch.h"


typedef enum{
	IRCOM_SLAVE_STA_DISCOV,
	IRCOM_SLAVE_STA_ACK,
	IRCOM_SLAVE_STA_ESTAB,
	IRCOM_SLAVE_STA_KEEP,
}irComSlaveConnState_t;

irCom_t hIrCom;
bspProfile_t* hbsp;
appSleepState_t sleepState = SLEEP_AWAKE;
irComSlaveConnState_t slaveConnState = IRCOM_SLAVE_STA_DISCOV;

/*------------------IrCom Recv Events------------------------*/


irComRet_t irComSlaveResp_DiscovReq(irCom_t *hircom)
{
	switch(slaveConnState){
	case IRCOM_SLAVE_STA_DISCOV:	
		Bsp_SleepTimerRestart(WAKETIME_CONN);	
		slaveConnState = IRCOM_SLAVE_STA_ACK;

	case IRCOM_SLAVE_STA_ACK:		

		uuit_t slaveUuid = SLAVE_UUID;

		uint8_t buff[sizeof(uuit_t)+sizeof(volt_t)*3+IRBUFF_OVERHEAD];
		irComBuffSize_t dataLen = sizeof(uuit_t)+sizeof(volt_t)*3; 

		memcpy(buff,slaveUuid,sizeof(uuit_t));
		memcpy(buff+sizeof(uuit_t),&hBspProfile.volt_esta,sizeof(volt_t));
		memcpy(buff+sizeof(uuit_t)+sizeof(volt_t),&hBspProfile.volt_window_high,sizeof(volt_t));
		memcpy(buff+sizeof(uuit_t)+sizeof(volt_t)*2,&hBspProfile.volt_window_low,sizeof(volt_t));

		irCom_Response(hircom,hircom->command,buff,dataLen);
		
		break;
	default:

	}

	return IRCOM_OK;
}


irComRet_t irComSlaveResp_DiscovAck(irCom_t *hircom)
{
	switch(slaveConnState){
	case IRCOM_SLAVE_STA_ACK:	
		Bsp_SleepTimerRestart(WAKETIME_CONN);
		SleepMch_StateAwake();
		slaveConnState=IRCOM_SLAVE_STA_ESTAB;

	case IRCOM_SLAVE_STA_ESTAB:

		uint8_t buff[sizeof(uuit_t)+IRBUFF_OVERHEAD];
		irComBuffSize_t dataLen=sizeof(uuit_t);  

		memcpy(buff,slaveUuid,sizeof(uuit_t));

		irCom_Response(hircom,hircom->command,buff,dataLen);

		break;
	default:
		
	}
	return IRCOM_OK;
}

irComRet_t irComSlaveResp_GetPowParam(irCom_t *hircom)
{
	switch(slaveConnState){
	case IRCOM_SLAVE_STA_ESTAB:	
		Bsp_SleepTimerRestart(WAKETIME_POW_ESTAB);
		slaveConnState=IRCOM_SLAVE_STA_KEEP;

	case IRCOM_SLAVE_STA_KEEP:

		uint8_t buff[sizeof(volt_t)+sizeof(curr_t)+IRBUFF_OVERHEAD];
		irComBuffSize_t dataLen=sizeof(volt_t)+sizeof(curr_t);	

		memcpy(buff,&hBspProfile.volt,sizeof(volt_t));
		memcpy(buff+sizeof(volt_t),&hBspProfile.curr,sizeof(curr_t));

		irCom_Response(hircom,hircom->command,buff,dataLen);

		break;
	default:
		
	}
	return IRCOM_OK;
}


/*------------------State Process------------------------*/

void irComSlave_StateRefresh()
{
	//...
	
}

void irComSlave_ConnStateReset()
{
	slaveConnState=IRCOM_SLAVE_STA_DISCOV;
}

/*------------------Sleep Process------------------------*/
void irComSlave_BeginLightSleep()
{
	//...
	//Bsp_ToSleep();
	//开启RTC，5s触发一次中断，通过bsp进入硬件睡眠
	uint32_t wksel=LIGHT_SLP_INTRVL;
	Bsp_RTC_SetTimeInterval(wksel,cnt);
	Bsp_SleepMode_ToSleep(wksel,cnt);
}

void irComSlave_BeginDeepSleep()
{
	//...
	//Bsp_ToSleep();
	//开启RTC，15s触发一次中断，通过bsp进入硬件睡眠
	uint32_t wksel=DEEP_SLP_INTRVL;
	Bsp_RTC_SetTimeInterval(wksel,cnt);
	Bsp_SleepMode_ToSleep(wksel,cnt);
}

void irComSlave_WakeProcess()
{
	//...
	//Bsp_FromSleep();
	//关闭RTC，执行bsp出硬件睡眠，睡眠机1s倒计时，红外连接状态重置
	Bsp_SleepMode_FromSleep();
	Bsp_SleepTimerRestart(1000);
	irComSlave_ConnStateReset();
}

/*------------------Main Process------------------------*/

void irComSlave_Config(irCom_t *hircom)
{
	irCom_Init(&hIrCom, IRCOM_SLAVE);
	irCom_SlaveFunConfig(hircom, IRCOM_CMD_DiscovReq, irComSlaveResp_DiscovReq);
	irCom_SlaveFunConfig(hircom, IRCOM_CMD_DiscovAck, irComSlaveResp_DiscovAck);
	irCom_SlaveFunConfig(hircom, IRCOM_CMD_GetPowParam, irComSlaveResp_GetPowParam);
	irCom_Open(&hIrCom);
}

void irComSlave_Main()
{
	hbsp=Bsp_Init();
	irComSlave_Config();
	Bsp_SleepTimerRestart(15000);

	while(1){
		Bsp_MonitorRefresh();
		irComListenPolling();
		irComSlave_StateRefresh();

		/*----------Sleep Process---------------*/
		toSleepCmd_t sleepState==Bsp_SleepGetCmd();
		switch(sleepCmd){
		case TOSLEEP_LIGHT:
			irComSlave_BeginLightSleep();
			//sleeping ...
			irComSlave_WakeProcess();
			break;
		case TOSLEEP_DEEP:
			irComSlave_BeginDeepSleep();
			//sleeping ...
			irComSlave_WakeProcess();
			break;
		default:
		}
	}

}


