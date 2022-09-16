/*
 * ap.cpp
 *
 *  Created on: Aug 21, 2022
 *      Author: gns2l
 */


#include "ap.h"

static void threadCmdCli(void const *argument);



void apInit(void)
{
	/* rtos initialize*/
	{
		osThreadDef(threadCmdCli, threadCmdCli, _HW_DEF_RTOS_THREAD_PRI_CLI, 0, _HW_DEF_RTOS_THREAD_MEM_CLI);
		if (osThreadCreate(osThread(threadCmdCli), NULL) != NULL)
		{
			logPrintf("threadCmdCli \t\t: OK\r\n");
		}
		else
		{
			logPrintf("threadCmdCli \t\t: Fail\r\n");
		}
	}

	i2cBegin(_DEF_I2C1, I2C_FREQ_400KHz);

	uartOpen(_DEF_UART2, 115200);
	cliOpen(_DEF_UART3, 115200);

}


void apMain(void)
{
	uint32_t pre_ms = millis();

	while (1)
	{
		if (millis() - pre_ms >=1000)
		{
			pre_ms = millis();
			ledToggle(_DEF_LED1);

			//logPrintf("uart1 Test\r\n");
			//uartPrintf(_DEF_UART2, "uart2 Test\r\n");


		}


	}

}



void threadCmdCli(void const *argument)
{
	UNUSED(argument);
	//uint32_t pre_ms = millis();

	while (1)
	{
		cliMain();

		delay(1);
	}
}

