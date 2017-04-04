#include <stdbool.h>
#include <stdint.h>
#include "driverlib/inc/hw_memmap.h"
#include "driverlib/inc/hw_types.h"
#include "driverlib/inc/hw_nvic.h"
#include "driverlib/inc/hw_ints.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "mytm4c129.h"

/* Declaring Global Variables */
uint32_t sys_clock_hz;

/* Declaring Function Prototype */
void Timer_ISR(void);


int main(void)
{
	/* Configure system clock using external clock and PLL */
	sys_clock_hz = MAP_SysCtlClockFreqSet(SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480,
			120000000);

	/*	Configure led as output	*/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
	GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE,GPIO_PIN_0);

	/*	Configure Timer with 100mS period	*/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);
	TimerLoadSet(TIMER0_BASE,TIMER_A,(sys_clock_hz/10)-1);
	TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
	IntEnable(INT_TIMER0A_TM4C129);
	IntMasterEnable();
	TimerIntRegister(TIMER0_BASE,TIMER_A,Timer_ISR);
	TimerEnable(TIMER0_BASE,TIMER_A);

	/* Infinite Loop */
	while(1)
	{
	}
}


void Timer_ISR(void)
{
	/*	Clear the timer interrupt	*/
	TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);

	if(GPIOPinRead(GPIO_PORTN_BASE,GPIO_PIN_0))
	{
		GPIOPinWrite(GPIO_PORTN_BASE,GPIO_PIN_0,0x00);
	}
	else
	{
		GPIOPinWrite(GPIO_PORTN_BASE,GPIO_PIN_0,0xff);
	}
}
