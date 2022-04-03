#include "bsp.h"






bool bspInit(void)
{
  /* Set system clock to 100 MHz */
  MXC_SYS_Clock_Select(MXC_SYS_CLOCK_IPO);
  SystemCoreClockUpdate();

	/* Initialize RTC */
	MXC_RTC_Init(0, 0);
	MXC_RTC_Start();

  // Enable GPIO
  MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_GPIO0);
  MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_GPIO1);
  MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_GPIO2);

  return true;
}

void delay(uint32_t time_ms)
{
  MXC_Delay(time_ms*1000);
}

uint32_t millis(void)
{  
  int sec;
  double subsec;
  uint32_t ms;

  subsec = MXC_RTC_GetSubSecond() / 4096.0;
  sec = MXC_RTC_GetSecond();

  ms = (sec * 1000) + (int)(subsec * 1000);

  return ms;
}