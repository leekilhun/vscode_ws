/*
 * hw.c
 *
 *  Created on: Aug 21, 2022
 *      Author: gns2l
 */


#include "hw.h"


extern uint32_t _flash_tag_addr;
extern uint32_t __isr_vector_addr;


__attribute__((section(".tag"))) firm_tag_t firm_tag =
{
    .magic_number = 0xAAAA5555,
    //-- fw info
    //
    .addr_tag = (uint32_t)&_flash_tag_addr,
    .addr_fw  = (uint32_t)&__isr_vector_addr,
    .size_tag = 1024,

    //-- tag info
    //
};


__attribute__((section(".version"))) firm_version_t firm_ver =
{
    "V220913R1",
    "Firmware"
};



bool hwInit(void)
{
  bool ret = true;

#ifndef _USE_HW_RTOS
  ret &= bspInit();
#endif

#ifdef _USE_HW_CLI
  ret &= cliInit();
#endif

  ret &= ledInit();


#ifdef _USE_HW_SPI
  ret &= spiInit();
#endif

#ifdef _USE_HW_FLASH
  ret &= flashInit();
#endif

#ifdef _USE_HW_GPIO
  ret &= gpioInit();
#endif

#ifdef _USE_HW_CODE_IDSW
  ret &= codeIDsw_Init();
#endif

#ifdef _USE_HW_BUZZER
  ret &= buzzerInit();
#endif


#ifdef _USE_HW_UART
  ret &= uartInit();
#endif


#ifdef _USE_HW_LOG
  ret &= logInit();

  logOpen(HW_LOG_CH, 115200);
  logPrintf("[ fw Begin... ]\r\n");
#else // use small size log func
  uartOpen(_DEF_UART1, 115200);
  uartPrintf(_DEF_UART1, "[ fw Begin... ]\r\n");
#endif

#ifdef _USE_HW_I2C
  ret &= i2cInit();
#endif


#ifdef _USE_HW_CAN
  ret &= canInit();
#endif


  return ret;
}



