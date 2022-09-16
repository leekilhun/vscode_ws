/*
 * kdr19k_IDsw.c
 *
 *  Created on: 2022. 9. 3.
 *      Author: gns2l
 */

#include "kdr10K_IDsw.h"

#ifdef _USE_EXHW_KDR10K_IDSW


#define KDR10K_ID_MAX_CH    1


static uint8_t kdr10kid_ID[KDR10K_ID_MAX_CH] = {0};

bool kdr10K_IDswInit(void)
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

	kdr10kid_ID[0] = data;

	return false;
}


uint8_t kdr10K_IDswRead(uint8_t ch)
{
	if (KDR10K_ID_MAX_CH < ch)
		return kdr10kid_ID[ch];
	return 0;
}


#endif
