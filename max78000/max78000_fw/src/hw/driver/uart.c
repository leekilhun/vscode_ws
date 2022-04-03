/*
 * uart.c
 *
 *  Created on: 2021. 7. 31.
 *      Author: baram
 */


#include "uart.h"


#ifdef _USE_HW_UART



#define UART_RX_BUF_LENGTH      1024


typedef enum
{
  UART_HW_TYPE_MCU,
  UART_HW_TYPE_USB,
} UartHwType_t;


typedef struct
{
  bool     is_open;
  uint32_t baud;
  UartHwType_t type;

  mxc_uart_regs_t *h_uart;
} uart_tbl_t;


static uart_tbl_t uart_tbl[UART_MAX_CH];




bool uartInit(void)
{
  for (int i=0; i<UART_MAX_CH; i++)
  {
    uart_tbl[i].is_open = false;
    uart_tbl[i].baud = 115200;

    uartOpen(i, uart_tbl[i].baud);
  }

  return true;
}

bool uartOpen(uint8_t ch, uint32_t baud)
{
  bool ret = false;


  if (uart_tbl[ch].is_open == true && uart_tbl[ch].baud == baud)
  {
    return true;
  }

  switch(ch)
  {
    case _DEF_UART1:
      uart_tbl[ch].type    = UART_HW_TYPE_MCU;
      uart_tbl[ch].baud    = baud;      
      uart_tbl[ch].is_open = true;
      uart_tbl[ch].h_uart  = MXC_UART_GET_UART(0);

      MXC_UART_Init(uart_tbl[ch].h_uart, baud, MXC_UART_IBRO_CLK);

      ret = true;
      break;
  }

  return ret;
}

bool uartClose(uint8_t ch)
{
  return true;
}

uint32_t uartAvailable(uint8_t ch)
{
  uint32_t ret = 0;

  switch(ch)
  {
    case _DEF_UART1:
      ret = MXC_UART_GetRXFIFOAvailable(uart_tbl[ch].h_uart);
      break;
  }

  return ret;
}

bool uartFlush(uint8_t ch)
{
  uint32_t pre_time;

  pre_time = millis();
  while(uartAvailable(ch))
  {
    if (millis()-pre_time >= 10)
    {
      break;
    }
    uartRead(ch);
  }

  return true;
}

uint8_t uartRead(uint8_t ch)
{
  uint8_t ret = 0;

  switch(ch)
  {
    case _DEF_UART1:
      MXC_UART_ReadRXFIFO(uart_tbl[ch].h_uart, &ret, 1);
      break;
  }

  return ret;
}

uint32_t uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length)
{
  uint32_t ret = 0;
  int len = length;

  switch(ch)
  {
    case _DEF_UART1:
      MXC_UART_Write(uart_tbl[ch].h_uart, p_data, &len);
      ret = len;
      break;
  }

  return ret;
}

uint32_t uartPrintf(uint8_t ch, const char *fmt, ...)
{
  char buf[256];
  va_list args;
  int len;
  uint32_t ret;

  va_start(args, fmt);
  len = vsnprintf(buf, 256, fmt, args);

  ret = uartWrite(ch, (uint8_t *)buf, len);

  va_end(args);


  return ret;
}

uint32_t uartGetBaud(uint8_t ch)
{
  uint32_t ret = 0;


  switch(ch)
  {
    case _DEF_UART1:
      ret = uart_tbl[ch].baud;
      break;
  }

  return ret;
}




#endif