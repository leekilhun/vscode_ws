/*
 * led.c
 *
 *  Created on: Nov 12, 2021
 *      Author: baram
 */




#include "led.h"


#ifdef _USE_HW_LED


static bool is_init = false;


typedef struct
{
  mxc_gpio_regs_t *port;
  uint32_t         pin;
  uint8_t          on_state;
  uint8_t          off_state;
} led_tbl_t;


const led_tbl_t led_tbl[LED_MAX_CH] =
    {
        {MXC_GPIO2, MXC_GPIO_PIN_0,  _DEF_LOW, _DEF_HIGH}, // LED_R
        {MXC_GPIO2, MXC_GPIO_PIN_1,  _DEF_LOW, _DEF_HIGH}, // LED_G
        {MXC_GPIO2, MXC_GPIO_PIN_2,  _DEF_LOW, _DEF_HIGH}, // LED_B
    };




bool ledInit(void)
{
  mxc_gpio_cfg_t gpio_cfg;



  for (int ch=0; ch<LED_MAX_CH; ch++)
  {
    gpio_cfg.port = led_tbl[ch].port;
    gpio_cfg.mask = led_tbl[ch].pin;
    gpio_cfg.func = MXC_GPIO_FUNC_OUT;
    gpio_cfg.pad  = MXC_GPIO_PAD_NONE;
    gpio_cfg.vssel= MXC_GPIO_VSSEL_VDDIOH;
    MXC_GPIO_Config(&gpio_cfg);

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
  MXC_GPIO_OutPut(led_tbl[ch].port, led_tbl[ch].pin, led_tbl[ch].pin * led_tbl[ch].on_state);
}

void ledOff(uint8_t ch)
{
  MXC_GPIO_OutPut(led_tbl[ch].port, led_tbl[ch].pin, led_tbl[ch].pin * led_tbl[ch].off_state);
}

void ledToggle(uint8_t ch)
{
  MXC_GPIO_OutToggle(led_tbl[ch].port, led_tbl[ch].pin);
}


#endif
