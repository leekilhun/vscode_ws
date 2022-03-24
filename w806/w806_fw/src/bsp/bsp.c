#include "bsp.h"



bool bspInit(void)
{
  SystemClock_Config(CPU_CLK_160M);

  return true;
}

void delay(uint32_t ms)
{
  uint32_t pre_time = millis();

  while(millis()-pre_time < ms)
  {    
  }
}

uint32_t millis(void)
{
  return HAL_GetTick();
}

void Error_Handler(void)
{
	while (1)
	{
	}
}

void assert_failed(uint8_t *file, uint32_t line)
{
	printf("Wrong parameters value: file %s on line %d\r\n", file, line);
}

#if 0
void logPrintf(const char *fmt, ...)
{
  va_list arg;
  va_start (arg, fmt);
  int32_t len;
  char print_buffer[128];


  len = vsnprintf(print_buffer, 128, fmt, arg);
  va_end (arg);

  //uartWrite(_DEF_UART1, (uint8_t *)print_buffer, len);
}
#endif
