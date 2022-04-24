#include "hw.h"

bool hwInit(void)
{
  bool ret = true;

  ret &= bspInit();

#ifdef _USE_HW_LED
  ret &= ledInit();
#endif

#ifdef _USE_HW_CLI
  ret &= cliInit();
#endif

#ifdef _USE_HW_BUZZER
  ret &= buzzerInit();
#endif

#ifdef _USE_HW_LED
  ret &= ledInit();
#endif

#ifdef _USE_HW_FLASH
  ret &= flashInit();
#endif

#ifdef _USE_HW_GPIO
  ret &= gpioInit();
#endif

#ifdef _USE_HW_UART
  ret &= uartInit();
#endif

#ifdef _USE_HW_SPI
  ret &= spiInit();
#endif

#ifdef _USE_HW_I2C
  ret &= i2cInit();
#endif


  return ret;
}
