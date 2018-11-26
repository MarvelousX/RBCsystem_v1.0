#include "irCom.h"
#include "irPack.h"

//test
//#include"irComSlave.h"

extern UART_HandleTypeDef huart1;

uint8_t id_count = 0;

#ifdef IRCOM_RECVMODE_CALLBACK
void irComSlaveRecvCallBack(irCom_t *hircom)
{
//...
}

void irComMasterRecvCallBack(irCom_t *hircom)
{
//...
}
#endif // IRCOM_RECVMODE_CALLBACK


void irComSendPack(irCom_t *hircom)
{
	//TODO: call the irpacksend function
	irPack_t pack;
	pack.head.addr = 0xaa;
	pack.head.ctrl = 0xba;
	pack.head.dataLen = hircom->datalen + sizeof(irComCmd_t);
	memcpy(pack.data,&hircom->command,sizeof(irComCmd_t));
	for (uint8_t i = sizeof(irComCmd_t);i < pack.head.dataLen;i++)
	{
		pack.data[i] = hircom->buff[i-sizeof(irComCmd_t)];
	}
	irPackSend(&pack);
}


irComRet_t irComSlaveResp(irCom_t *hircom) {
	if (hircom->pFunSlaveResp[hircom->command] == NULL) {
		return IRCOM_ERR_FUN;
	}
	return (*(hircom->pFunSlaveResp[hircom->command]))(hircom);
}


irComRet_t irComtoirPack(irPack_t *pack, irCom_t *hircom)
{
	hircom->datalen = pack->head.dataLen - sizeof(irComCmd_t);
	memcpy(&hircom->command, pack->data, sizeof(irComCmd_t));
	for (uint8_t i = 0; i < hircom->datalen; i++){
		hircom->buff[i] = pack->data[i+sizeof(irComCmd_t)];
	}
	return IRCOM_OK;
}

irComRet_t irComRecvPack(irCom_t *hircom) //发送对象为Master则IRCOM_OK，为Slave则调用irComSlaveResp函数
{
	// TODO: call the irpackrecv function
	irPack_t packbuff;
	if(irPackRecv(&packbuff) == IRPACK_OK)
	{
		//uint8_t i = 0, j;
		//uint8_t src[IRPACK_MAX_BUFF];
		switch (hircom->role)
		{
		case IRCOM_MASTER:
			// Be sure, for master , the received command should equal to hircom->command
			/*memcpy(src, (uint8_t*)(&packbuff)->data, (&packbuff)->head.dataLen);
			hircom->datalen = (&packbuff)->head.dataLen - sizeof(irComCmd_t);
			hircom->command = src[0];
			for (uint8_t i = 1;i <= hircom->datalen;i++)
				hircom->buff[i-1] = src[i];*/
			if(IRCOM_OK==irComtoirPack(&packbuff, hircom)){
				
				return IRCOM_OK;
			}
			break;
		case IRCOM_SLAVE:
			/*memcpy(src, (uint8_t*)(&packbuff)->data, (&packbuff)->head.dataLen);
			hircom->datalen = (&packbuff)->head.dataLen - sizeof(irComCmd_t);
			hircom->command = src[0];
			for (uint8_t i = 1;i <= hircom->datalen;i++)
				hircom->buff[i - 1] = src[i];*/
			if(IRCOM_OK==irComtoirPack(&packbuff, hircom)){
				return irComSlaveResp(hircom);
			}
			break;
		default:
			return IRCOM_ERR_ROLE;
		}
	}
	else {
		return IRCOM_NOTRECV;
	}
	return IRCOM_FAILED;
}


irComRet_t irComWaitResp(irCom_t *hircom, irComTimeOutMs_t timeout) 
{
	irComClock_t tm0 = IRCOM_SYSTIME;
	do {
		if (irComRecvPack(hircom) == IRCOM_OK) {
			//printf("---->Assuming responsed within the time limit\n---->Finished\n");
			return IRCOM_OK;
		}
	} while (IRCOM_SYSTIME - tm0 < timeout);
	return IRCOM_TIMEOUT;
}


irComRet_t irCom_CommandOnce(irCom_t *hircom, irComTimeOutMs_t timeout)
{
	irComSendPack(hircom);
	return irComWaitResp(hircom, timeout);
}



irComRet_t irCom_Init(irCom_t *hircom, irComRole_t role)
{
	hircom->irComId = id_count++;
	hircom->role = role;
	hircom->state = IRCOM_CLOSEED;

#ifdef IRCOM_RECVMODE_CALLBACK
	switch (role)
	{
	case IRCOM_MASTER:
		hircom->irComRrcvCallBack = irComMasterRecvCallBack;
		break;
	case IRCOM_SLAVE:
		hircom->irComRrcvCallBack = irComSlaveRecvCallBack;
		break;
	default:
		break;
	}
#endif // IRCOM_RECVMODE_CALLBACK
	

	for (irComCmd_t i = 0; i < IRCOM_MAX_CMDNUM; i++)
	{
		hircom->pFunSlaveResp[i] = NULL;
	}
	return IRCOM_OK;
}


irComRet_t irCom_SlaveFunConfig(irCom_t *hircom, irComCmd_t command, irComFunRecvResp_t pFun)
{
	if (command >= IRCOM_MAX_CMDNUM) {
		return IRCOM_FAILED;
	}
	hircom->pFunSlaveResp[command] = pFun;
	return IRCOM_OK;
}


irComRet_t irCom_Command(irCom_t *hircom, irComCmd_t command, uint8_t* data,
	irComBuffSize_t len, irComTimeOutMs_t timeout)
{
	if (hircom->state != IRCOM_OPENED) {
		return IRCOM_ERR_CLOSED;
	}
	if (hircom->state != IRCOM_MASTER) {
		return IRCOM_ERR_ROLE;
	}
	for (uint8_t i = 0;i < hircom->datalen;i++){		//TODO:memcpy
		hircom->buff[i] = data[i];
	}
	hircom->command = command;
	hircom->datalen = len;
	return irCom_CommandOnce(hircom, timeout);
}


irComRet_t irCom_ListenOnce(irCom_t *hircom)
{
	if (hircom->state != IRCOM_OPENED) {
		return IRCOM_ERR_CLOSED;
	}
	if (hircom->role == IRCOM_MASTER) {
		return IRCOM_ERR_ROLE;
	}
	return irComRecvPack(hircom);
}


irComRet_t irCom_Response(irCom_t *hircom, irComCmd_t command, uint8_t* data, irComBuffSize_t len)
{
	if (hircom->state != IRCOM_OPENED) {
		return IRCOM_ERR_CLOSED;
	}
	if (hircom->role != IRCOM_SLAVE) {
		return IRCOM_ERR_ROLE;
	}
	for (uint8_t i = 0;i < len;i++) {
		hircom->buff[i] =data[i];
	}
	hircom->command = command;
	hircom->datalen = len;
	irComSendPack(hircom);
	return IRCOM_OK;
}



irComRet_t irCom_Open(irCom_t *hircom)
{
	hircom->state = IRCOM_OPENED;
	return IRCOM_OK;
}

irComRet_t irCom_Close(irCom_t *hircom) 
{
	hircom->state = IRCOM_CLOSEED;
	return IRCOM_OK;
}
