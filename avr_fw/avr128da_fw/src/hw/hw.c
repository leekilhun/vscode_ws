#include "hw.h"



volatile const firm_ver_t firm_ver __attribute__((section(".vectors"))) = 
{
  .magic_number = VERSION_MAGIC_NUMBER,
  .version_str  = _DEF_FIRMWATRE_VERSION,
  .name_str     = _DEF_BOARD_NAME,
};




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