/*
 * gpio.c
 *
 *  jL branchCtrl  digital io
 *
 *  Created on: Nov 27, 2021
 *      Author: gns2l
 */




#include "gpio.h"
#include "cli.h"


#ifdef _USE_HW_GPIO

typedef struct
{
  GPIO_TypeDef *port;
  uint32_t      pin;
  uint8_t       mode;
  GPIO_PinState on_state;
  GPIO_PinState off_state;
  bool          init_value;
} gpio_tbl_t;


const gpio_tbl_t gpio_tbl[GPIO_MAX_CH] =
    {
        {GPIOB, GPIO_PIN_1,  _DEF_OUTPUT_PULLUP, GPIO_PIN_SET, GPIO_PIN_RESET,   _DEF_LOW},      // 0. spi ncs
        {GPIOB, GPIO_PIN_0,  _DEF_OUTPUT_PULLUP, GPIO_PIN_SET, GPIO_PIN_RESET,   _DEF_LOW},      // 1. spi nss

			/*	//BD ID SW
        {GPIOC, GPIO_PIN_10,  _DEF_INPUT_PULLUP,  GPIO_PIN_RESET, GPIO_PIN_SET,   _DEF_LOW},      // 2. BD_ID_1
        {GPIOC, GPIO_PIN_11,  _DEF_INPUT_PULLUP,  GPIO_PIN_RESET, GPIO_PIN_SET,   _DEF_LOW},      // 3. BD_ID_2
        {GPIOC, GPIO_PIN_12,  _DEF_INPUT_PULLUP,  GPIO_PIN_RESET, GPIO_PIN_SET,   _DEF_LOW},      // 4. BD_ID_4
        {GPIOC, GPIO_PIN_13,  _DEF_INPUT_PULLUP,  GPIO_PIN_RESET, GPIO_PIN_SET,   _DEF_LOW},      // 5. BD_ID_8
      */

				// in 0~7
        {GPIOD, GPIO_PIN_3,  _DEF_INPUT_PULLUP,  GPIO_PIN_RESET, GPIO_PIN_SET,   _DEF_LOW},      // 6.
        {GPIOD, GPIO_PIN_4,  _DEF_INPUT_PULLUP,  GPIO_PIN_RESET, GPIO_PIN_SET,   _DEF_LOW},      // 7.
        {GPIOD, GPIO_PIN_5,  _DEF_INPUT_PULLUP,  GPIO_PIN_RESET, GPIO_PIN_SET,   _DEF_LOW},      // 8.
        {GPIOD, GPIO_PIN_6,  _DEF_INPUT_PULLUP,  GPIO_PIN_RESET, GPIO_PIN_SET,   _DEF_LOW},      // 9.
        {GPIOD, GPIO_PIN_0,  _DEF_INPUT_PULLUP,  GPIO_PIN_RESET, GPIO_PIN_SET,   _DEF_LOW},      // 10.
        {GPIOD, GPIO_PIN_1,  _DEF_INPUT_PULLUP,  GPIO_PIN_RESET, GPIO_PIN_SET,   _DEF_LOW},      // 11.
        {GPIOD, GPIO_PIN_2,  _DEF_INPUT_PULLUP,  GPIO_PIN_RESET, GPIO_PIN_SET,   _DEF_LOW},      // 12.
        {GPIOB, GPIO_PIN_6,  _DEF_INPUT_PULLUP,  GPIO_PIN_RESET, GPIO_PIN_SET,   _DEF_LOW},      // 13.

				// out 0~7
        {GPIOC, GPIO_PIN_8,  _DEF_OUTPUT, GPIO_PIN_SET, GPIO_PIN_RESET,   _DEF_LOW},      // 14.
        {GPIOA, GPIO_PIN_15, _DEF_OUTPUT, GPIO_PIN_SET, GPIO_PIN_RESET,   _DEF_LOW},      // 15.
        {GPIOC, GPIO_PIN_9,  _DEF_OUTPUT, GPIO_PIN_SET, GPIO_PIN_RESET,   _DEF_LOW},      // 16.
        {GPIOA, GPIO_PIN_11, _DEF_OUTPUT, GPIO_PIN_SET, GPIO_PIN_RESET,   _DEF_LOW},      // 17.
        {GPIOD, GPIO_PIN_9,  _DEF_OUTPUT, GPIO_PIN_SET, GPIO_PIN_RESET,   _DEF_LOW},      // 18.
        {GPIOD, GPIO_PIN_8,  _DEF_OUTPUT, GPIO_PIN_SET, GPIO_PIN_RESET,   _DEF_LOW},      // 19.
        {GPIOC, GPIO_PIN_7,  _DEF_OUTPUT, GPIO_PIN_SET, GPIO_PIN_RESET,   _DEF_LOW},      // 20.
        {GPIOC, GPIO_PIN_6,  _DEF_OUTPUT, GPIO_PIN_SET, GPIO_PIN_RESET,   _DEF_LOW},      // 21.


				// test i2c interrupt
				{GPIOB, GPIO_PIN_15,  _DEF_INPUT_PULLUP,  GPIO_PIN_RESET, GPIO_PIN_SET,   _DEF_LOW},      // 22.



    };



#ifdef _USE_HW_CLI
static void cliGpio(cli_args_t *args);
#endif



bool gpioInit(void)
{
  bool ret = true;


  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  for (int i=0; i<GPIO_MAX_CH; i++)
  {
    gpioPinMode(i, gpio_tbl[i].mode);
    gpioPinWrite(i, gpio_tbl[i].init_value);
  }

  //Test I2C Interrupt
  {
  	GPIO_InitTypeDef GPIO_InitStruct = {0};
  	/*Configure GPIO pin : PtPin */
  	GPIO_InitStruct.Pin = GPIO_PIN_15;
  	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  	GPIO_InitStruct.Pull = GPIO_NOPULL;
  	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  	/* EXTI interrupt init*/
  	HAL_NVIC_SetPriority(EXTI4_15_IRQn, 3, 0);
  	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
  }

#ifdef _USE_HW_CLI
  cliAdd("gpio", cliGpio);
#endif

  return ret;
}

bool gpioPinMode(uint8_t ch, uint8_t mode)
{
  bool ret = true;
  GPIO_InitTypeDef GPIO_InitStruct = {0};


  if (ch >= GPIO_MAX_CH)
  {
    return false;
  }


  GPIO_InitStruct.Pin      = gpio_tbl[ch].pin;

  switch(mode)
  {
    case _DEF_INPUT:
      GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      break;

    case _DEF_INPUT_PULLUP:
      GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
      GPIO_InitStruct.Pull = GPIO_PULLUP;
      break;

    case _DEF_INPUT_PULLDOWN:
      GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
      GPIO_InitStruct.Pull = GPIO_PULLDOWN;
      break;

    case _DEF_OUTPUT:
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      break;

    case _DEF_OUTPUT_PULLUP:
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Pull = GPIO_PULLUP;
      break;

    case _DEF_OUTPUT_PULLDOWN:
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Pull = GPIO_PULLDOWN;
      break;
  }

  GPIO_InitStruct.Speed    = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(gpio_tbl[ch].port, &GPIO_InitStruct);

  return ret;
}

void gpioPinWrite(uint8_t ch, bool value)
{
  if (ch >= GPIO_MAX_CH)
  {
    return;
  }

  if (value)
  {
    HAL_GPIO_WritePin(gpio_tbl[ch].port, gpio_tbl[ch].pin, gpio_tbl[ch].on_state);
  }
  else
  {
    HAL_GPIO_WritePin(gpio_tbl[ch].port, gpio_tbl[ch].pin, gpio_tbl[ch].off_state);
  }
}

bool gpioPinRead(uint8_t ch)
{
  bool ret = false;

  if (ch >= GPIO_MAX_CH)
  {
    return false;
  }

  if (HAL_GPIO_ReadPin(gpio_tbl[ch].port, gpio_tbl[ch].pin) == gpio_tbl[ch].on_state)
  {
    ret = true;
  }

  return ret;
}

void gpioPinToggle(uint8_t ch)
{
  if (ch >= GPIO_MAX_CH)
  {
    return;
  }

  HAL_GPIO_TogglePin(gpio_tbl[ch].port, gpio_tbl[ch].pin);
}


bool gpioIsOn(uint8_t ch)
{
  return gpioPinRead(ch);
}


void EXTI4_15_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
}


void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(GPIO_Pin);

}

/**
  * @brief  EXTI line detection callback.
  * @param  GPIO_Pin Specifies the port pin connected to corresponding EXTI line.
  * @retval None
  */
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(GPIO_Pin);

}




#ifdef _USE_HW_CLI
void cliGpio(cli_args_t *args)
{
  bool ret = false;


  if (args->argc == 1 && args->isStr(0, "show") == true)
  {
    while(cliKeepLoop())
    {
      for (int i=0; i<GPIO_MAX_CH; i++)
      {
        cliPrintf("%d", gpioPinRead(i));

      }
      cliPrintf("\n");
      cliPrintf("\x1B[%dA", 1);

      delay(100);
    }
    ret = true;
  }

  if (args->argc == 2 && args->isStr(0, "read") == true)
  {
    uint8_t ch;

    ch = (uint8_t)args->getData(1);

    while(cliKeepLoop())
    {
      cliPrintf("gpio read %d : %d\n", ch, gpioPinRead(ch));
      delay(100);
    }

    ret = true;
  }

  if (args->argc == 3 && args->isStr(0, "write") == true)
  {
    uint8_t ch;
    uint8_t data;

    ch   = (uint8_t)args->getData(1);
    data = (uint8_t)args->getData(2);

    gpioPinWrite(ch, data);

    cliPrintf("gpio write %d : %d\n", ch, data);
    ret = true;
  }

  if (ret != true)
  {
    cliPrintf("gpio show\n");
    cliPrintf("gpio read ch[0~%d]\n", GPIO_MAX_CH-1);
    cliPrintf("gpio write ch[0~%d] 0:1\n", GPIO_MAX_CH-1);
  }
}


#endif /*_USE_HW_GPIO*/


#endif
