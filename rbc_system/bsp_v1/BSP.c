#include "bsp.h"
#include "rtc.h"
#include "adc.h"
#include "crc.h"
#include "usart.h"
#include "rtc.h"
#include "gpio.h"

bspProfile_t hBspProfile;

extern UART_HandleTypeDef hlpuart1;
extern IRDA_HandleTypeDef hirda2;

/*-------------Monitor---------------*/
void Bsp_Monitor_Init(){	
  MX_ADC_Init();
  //#### should initialize the related GPIO pins : ，HAL_ADC_MspInit
}
void Bsp_Monitor_DeInit(){
  MX_ADC_DeInit();
}

uint16_t Bsp_Monitor_GetVoltageMini(){	//unit : mV
  return Get_Adc_Average(ADC_CHANNEL_8,8);
}
uint16_t Bsp_Monitor_GetCurrentMini(){	//unit : mA
  return Get_Adc_Average(ADC_CHANNEL_6,8);
}

void Bsp_Monitor_ParamRefresh()
{
	//...
	hBspProfile.volt = Bsp_Monitor_GetVoltageMini();
	hBspProfile.curr = Bsp_Monitor_GetCurrentMini();
}


/*-------------IrModule---------------*/
void Bsp_IrModule_Init()
{
  MX_USART2_IRDA_Init();
  //#### should initialize the related GPIO pins :  HAL_IRDA_MspInit
}

void Bsp_IrModule_DeInit()
{
	MX_USART2_IRDA_DeInit();
}

void Bsp_IrModuleSendData(uint8_t *xdata, uint16_t xlen)
{
	HAL_IRDA_Transmit(&hirda2, xdata, xlen, 0xFFFF);
}

/*-------------LPUART---------------*/

void Bsp_LPUART_Init()
{
  MX_LPUART1_UART_Init();
	//#### should initialize the related GPIO pins : HAL_UART_MspInit
}

void Bsp_LPUART_DeInit()
{
	MX_LPUART1_UART_DeInit();
}

/*-------------RTC---------------*/

void Bsp_RTC_Init(uint32_t wksel,uint16_t cnt)
{
  	MX_RTC_Init();
	RTC_Set_WakeUp(wksel, cnt);
}

void Bsp_RTC_DeInit()
{
  	RTC_DeInit();
}
void Bsp_RTC_SetTimeInterval(uint32_t wksel,uint16_t cnt)	//#### just need one parameter：timer interval in 'ms' unit
{
	RTC_Set_WakeUp(wksel, cnt);
}
/*-------------SleepMode---------------*/
void Bsp_SleepMode_ToSleep(uint32_t wksel,uint16_t cnt)
{
	MX_ADC_DeInit();	//#### bsp_monitor_init
	Bsp_IrModule_DeInit();
	Bsp_LPUART_DeInit();
	Bsp_RTC_Init(wksel, cnt);
	//#### hardware start STOP mode
}
void Bsp_SleepMode_FromSleep()
{
	MX_ADC_Init();		//#### bsp_monitor_deinit
	Bsp_IrModule_Init();
	Bsp_LPUART_Init();
	Bsp_RTC_DeInit();
}

/*---------------Init-----------------*/
bspProfile_t* Bsp_Init()
{
	//####  initialize profile's parameter and all of the hardwares.
	Bsp_Monitor_ParamRefresh();
	return &hBspProfile;
}