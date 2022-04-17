#include "ap.h"

void task1_handle(void *p);
void task2_handle(void *p);

TaskHandle_t htask1 = NULL;
TaskHandle_t htask2 = NULL;

void apBoot(void);




void apInit(void)
{
	xTaskCreate(task1_handle, "task1", 512, NULL, 35, &htask1);
	xTaskCreate(task2_handle, "task2", 512, NULL, 32, &htask2);
	vTaskStartScheduler();

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

      logPrintf("index %d\n", index++);
    }
  }
}


void task1_handle(void *p)
{
	for (;;)
	{
		printf("task1_handle\r\n");

    //ledToggle(_DEF_LED2);
    vTaskDelay(1000);
	}
}

void task2_handle(void *p)
{
	for (;;)
	{
		printf("task2_handle\r\n");
    
    //ledToggle(_DEF_LED3);
		vTaskDelay(1000);

  }
}
