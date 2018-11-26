#ifndef	__BSP_H
#define __BSP_H
#include "stm32l0xx_hal.h"

typedef uint32_t uuid_t;
typedef uint16_t volt_t;
typedef uint16_t curr_t;

typedef struct{
	uuid_t uuid;
	volt_t volt;
	curr_t curr;
	volt_t volt_esta;
	volt_t volt_window_high;
	volt_t volt_window_low;
} bspProfile_t;

extern bspProfile_t hBspProfile;

void Bsp_MonitorRefresh();
bspProfile_t* Bsp_Init();

void Bsp_IrModuleSendData(uint8_t *xdata, uint16_t xlen);

#endif
