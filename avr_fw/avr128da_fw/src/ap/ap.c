#include "ap.h"






void apInit(void)
{  
  #ifdef _USE_HW_CLI
  cliOpen(_DEF_UART1, 115200);   
  #endif

  logPrintf_P(PSTR("\nFirmware..\n"));
}

void apMain(void)
{
  uint32_t pre_time;


  pre_time = millis();
  while(1)
  {
    if (millis()-pre_time >= 500)
    {
      pre_time = millis();

      ledToggle(_DEF_LED1);      
    }
    
    #ifdef _USE_HW_CLI
    cliMain();
    #endif
  }
}