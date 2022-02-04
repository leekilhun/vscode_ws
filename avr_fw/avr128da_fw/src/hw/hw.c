#include "hw.h"


bool hwInit(void)
{
  bspInit();
  
  #ifdef _USE_HW_CLI
  cliInit();  
  #endif
  ledInit();
  infoInit();
  uartInit();
  buttonInit();
  resetInit();
  flashInit();

  sei();
  return true;
}

void jumpToApp(void)
{
  cli();

  pgm_jmp_far(FLASH_ADDR_FW/sizeof(uint16_t));  
}

void jumpToBoot(void)
{
  resetToBoot(RESET_MODE_RUN_BOOT);
}