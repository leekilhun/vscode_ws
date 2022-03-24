#include "ap.h"



void apBoot(void);




void apInit(void)
{

}

void apMain(void)
{
  uint32_t pre_time;
  uint32_t index = 0;


  pre_time = millis();
  while(1)
  {
    if (millis()-pre_time >= 500)
    {
      pre_time = millis();
      
      ledToggle(_DEF_LED1);      
      ledToggle(_DEF_LED2);      
      ledToggle(_DEF_LED3);      

      logPrintf("index %d\n", index++);
    }
  }
}

