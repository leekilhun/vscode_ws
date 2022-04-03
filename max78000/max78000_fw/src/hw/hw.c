/*
 * hw.c
 *
 *  Created on: Jun 13, 2021
 *      Author: baram
 */

#include "hw.h"





bool hwInit(void)
{  
  bspInit();

  cliInit();
  logInit();
  ledInit();
  uartInit();
  
  logOpen(_DEF_UART1, 115200);

  logPrintf("[ Firmware Begin... ]\r\n");
  logPrintf("Booting..Name \t\t: %s\r\n", _DEF_BOARD_NAME);
  logPrintf("Booting..Ver  \t\t: %s\r\n", _DEF_FIRMWATRE_VERSION);
  logPrintf("Clk Sys  \t\t: %d Mhz\r\n", SystemCoreClock/1000000);

  logBoot(false);

  return true;
}