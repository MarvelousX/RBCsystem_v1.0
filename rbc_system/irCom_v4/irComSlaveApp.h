#ifndef __IRCOMCMD_H
#define __IRCOMCMD_H
#include "stdint.h"
#include "ircom.h"


#define SLAVE_UUID (0xAAE1A1A1)

#define WAKETIME_STARTRUN	(15000)
#define	WAKETIME_OBSERVE	(1000)
#define WAKETIME_CONN		(5000)
#define WAKETIME_POW_ESTAB	(10000)
#define WAKETIME_POW_KEEP	(5000)



typedef enum 
{
	IRCOM_CMD_Error,
	IRCOM_CMD_DiscovReq,
	IRCOM_CMD_DiscovAck,
	IRCOM_CMD_GetPowParam,
}irComCmd_Cmd_t;

typedef struct {
	APP_FAILED,
	APP_OK
}appRet_t;

void App_Main();

void irComSlave_StateReset();
void irComSlave_Config(irCom_t *hircom);
void irComSlave_Main();



irComRet_t irComSlaveResp_DiscovReq(irCom_t *hircom);
irComRet_t irComSlaveResp_DiscovAck(irCom_t *hircom);
//irComRet_t irComSlaveResp_LinkEstab(irCom_t *hircom);
irComRet_t irComSlaveResp_GetPowParam(irCom_t *hircom);

void irComCMD_Config(irCom_t *hircom);




//test
//void prt_command(irCom_t *hircom);

#endif

