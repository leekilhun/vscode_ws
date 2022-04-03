#include "led.h"


#ifdef _USE_HW_LED


static bool is_init = false;


typedef struct
{
  GPIO_TypeDef *port;
  uint16_t      pin;
  GPIO_PinState on_state;
  GPIO_PinState off_state;
} led_tbl_t;


const led_tbl_t led_tbl[LED_MAX_CH] =
    {
        {GPIOB, GPIO_PIN_0,  GPIO_PIN_RESET, GPIO_PIN_SET}, 
        {GPIOB, GPIO_PIN_1,  GPIO_PIN_RESET, GPIO_PIN_SET}, 
        {GPIOB, GPIO_PIN_2,  GPIO_PIN_RESET, GPIO_PIN_SET}, 
    };




bool ledInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};


  __HAL_RCC_GPIO_CLK_ENABLE();


  for (int ch=0; ch<LED_MAX_CH; ch++)
  {
    GPIO_InitStruct.Pin   = led_tbl[ch].pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    HAL_GPIO_Init(led_tbl[ch].port, &GPIO_InitStruct);

    ledOff(ch);
  }


  is_init = true;

  return true;
}

bool ledDeInit(void)
{
  return true;
}

bool ledIsInit(void)
{
  return is_init;
}

void ledOn(uint8_t ch)
{
  HAL_GPIO_WritePin(led_tbl[ch].port, led_tbl[ch].pin, led_tbl[ch].on_state);
}

void ledOff(uint8_t ch)
{
  HAL_GPIO_WritePin(led_tbl[ch].port, led_tbl[ch].pin, led_tbl[ch].off_state);
}

void ledToggle(uint8_t ch)
{
  HAL_GPIO_TogglePin(led_tbl[ch].port, led_tbl[ch].pin);
}


#endif