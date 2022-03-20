/*
 * hw.c
 *
 *  Created on: Dec 27, 2021
 *      Author: gns2.lee
 */



#include "hw.h"



bool hwInit(void)
{
  bool ret = true;

  ret &= bspInit();

#ifdef _USE_HW_RTC
  ret &= rtcInit();
#endif

#ifdef _USE_HW_RESET
  ret &= resetInit();
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

#ifdef _USE_HW_BUTTON
  ret &= buttonInit();
#endif

#ifdef _USE_HW_FLASH
  ret &= flashInit();
#endif

#ifdef _USE_HW_GPIO
  ret &= gpioInit();
#endif

#ifdef _USE_HW_PWM
  ret &= pwmInit();
#endif

#ifdef _USE_HW_LOG
  ret &= logInit();
#endif

#ifdef USE_HW_USB
  ret &= usbInit();
#endif

#ifdef _USE_HW_UART
  ret &= uartInit();
#endif


#ifdef _USE_HW_LOG
  logOpen(_DEF_UART2, 115200);
  logPrintf("[ fw Begin... ]\r\n");
#else // use small size log func
  uartOpen(_DEF_UART2, 115200);
  logPrintf("[ fw Begin... ]\r\n");
#endif



#ifdef _USE_HW_SPI
  ret &= spiInit();
#endif

#ifdef _USE_HW_I2C
  ret &= i2cInit();
#endif


#ifdef _USE_HW_SWTIMER
  ret &= swTimerInit();
#endif

#ifdef _USE_HW_CAN
  ret &= canInit();
#endif

  //usbBegin(USB_CDC_MODE);
  return ret;
}
