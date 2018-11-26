#include "irPack.h"


irPackRet_t irPackRecvFlag = IRPACK_FAILED;
uint8_t		irPackRecvBuff[IRPACK_MAX_BUFF];
uint8_t		irPackRecvBuffLen=0;


irPackDataLen_t _packToHDLC(uint8_t *outBuff, irPack_t *pack)
{
	uint8_t i = 0;
	uint16_t j;	
	uint8_t srcBuff[IRPACK_MAX_BUFF];
	irPackDataLen_t codeLen = pack->head.dataLen + sizeof(irPackAddr_t)+sizeof(irPackCtrl_t)+sizeof(irPackCrc_t);

	outBuff[0] = 0x7E;
	{
		srcBuff[i++] = pack->head.addr;            //addr
		srcBuff[i++] = pack->head.ctrl;            //ctrl
		for (j = 0; j < pack->head.dataLen; j++){  //data
			srcBuff[i++] = pack->data[j];
		}
		j = crc16_modbus(srcBuff, codeLen - sizeof(irPackCrc_t));        //crc calculation
		srcBuff[i++] = (uint8_t)(j >> 8);
		srcBuff[i++] = (uint8_t)(j & 0x00ff);	

		codeLen = bitFillingEncode(outBuff + 1, srcBuff, codeLen);    //bit Filling encode
	}
	outBuff[codeLen+2-1] = 0x7E;
	return codeLen + 2;
}

irPackRet_t _HDLCtoPack(irPack_t *pack, uint8_t *inBuff, irPackDataLen_t inLen)
{
	uint8_t i = 0, j;
	irPackDataLen_t buffLen;
	uint16_t crc;
	uint8_t buff[IRPACK_MAX_BUFF];
	buffLen = bitFillingDecode(buff, inBuff+1, inLen-2);	
	crc = crc16_modbus(buff, buffLen - 2);	
	if (((uint8_t)(crc >> 8) == buff[buffLen - 2]) && ((uint8_t)(crc & 0x00ff) == buff[buffLen - 1])){ //crc check
		pack->head.dataLen = buffLen - sizeof(irPackAddr_t)-sizeof(irPackCtrl_t)-sizeof(irPackCrc_t);
		pack->head.addr = buff[i++];                         //addr
		pack->head.ctrl = buff[i++];                         //ctrl
		for (j = 0; j < pack->head.dataLen; j++){            //data
			pack->data[j] = buff[i++];
		}
		//pack->head.crc = (uint16_t)(buff[buffLen - 2] << 8 | buff[buffLen - 1]); 不需要这项
		return IRPACK_OK;
	}	
	return IRPACK_FAILED;
}

irPackRet_t irPackSend(irPack_t *pack)
{
	uint8_t buff[IRPACK_MAX_BUFF];
	irPackDataLen_t buffLen=_packToHDLC(buff, pack);
	IR_UART_SEND(buff,buffLen);
	return IRPACK_OK;
}

irPackRet_t irPackRecv(irPack_t *pack)
{
	if(irPackRecvFlag == IRPACK_OK){
		if (_HDLCtoPack(pack, irPackRecvBuff, irPackRecvBuffLen) == IRPACK_OK){
			//printf("RCVED>>%s\n",pack->data);
			irPackRecvFlag = IRPACK_FAILED;
			irPackRecvBuffLen = 0;
			return IRPACK_OK;
		}
		irPackRecvFlag = IRPACK_FAILED;
		irPackRecvBuffLen = 0;
		return IRPACK_FAILED;
	}
	else
	{
		return IRPACK_FAILED;
	}
	return IRPACK_FAILED;
}

void irPackRecvCallbk(uint8_t recvByte)
{
	// ... 1. find start Flag
	// ... 2. receive data ->irPackRecvBuff；irPackRecvBuffLen++
	// ... 3. until stop Flag
	// ... 4. irPackRecvFlag=IRPACK_OK
	static irPackRxState_t state = U_RX_STA_S;
	if (irPackRecvFlag == IRPACK_FAILED){
		switch (state)
		{
		case U_RX_STA_S:
			if (recvByte == 0x7E){
				irPackRecvBuff[irPackRecvBuffLen++] = recvByte;	
				state = U_RX_STA_D;
			}
			break;
		case U_RX_STA_D:	
			irPackRecvBuff[irPackRecvBuffLen++] = recvByte;
			if ((recvByte == 0x7E) && (irPackRecvBuffLen == 2)){
				irPackRecvBuffLen = 1;
			}
			else if ((recvByte == 0x7E) && (irPackRecvBuffLen > 2)){	
				irPackRecvFlag = IRPACK_OK;
				state = U_RX_STA_S;
			}
		}
	}
}
