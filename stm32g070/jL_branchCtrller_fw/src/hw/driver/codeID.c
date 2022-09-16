/*
 * codeID.c
 *
 *  Created on: Sep 4, 2022
 *      Author: gns2l
 */



#include "codeID.h"

#ifdef _USE_HW_CODE_IDSW


#define CODE_ID_MAX_CH    HW_CODE_MAX_CH


static uint8_t code_ID[CODE_ID_MAX_CH] = {0};

bool codeIDsw_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct = {0};
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();
		GPIO_InitStruct.Pin       = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13;
		GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull      = GPIO_NOPULL;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	}
	uint8_t data = 0;
	data = (uint8_t)HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_10)<<10;
	data |= (uint8_t)HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_11)<<1;
	data |= (uint8_t)HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_12)<<2;
	data |= (uint8_t)HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)<<3;

	code_ID[0] = data;

	return false;
}


uint8_t codeIDsw_Read(uint8_t ch)
{
	if (CODE_ID_MAX_CH < ch)
		return code_ID[ch];
	return 0;
}


#endif


