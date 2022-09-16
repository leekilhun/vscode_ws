/*
 * hw_def.h
 *
 *  Created on: Aug 21, 2022
 *      Author: gns2l
 */

#ifndef SRC_HW_HW_DEF_H_
#define SRC_HW_HW_DEF_H_



#include "bsp.h"



/****************************************************
   boot/ firmware memory information
 ****************************************************/

#define FLASH_ADDR_BOOT_VER         0x8000400

/* ------------------------------------------------- */




//#define _USE_HW_RTOS  ->

#define _HW_DEF_RTOS_MEM_SIZE(x)              ((x)/4)

#define _HW_DEF_RTOS_THREAD_PRI_MAIN          osPriorityNormal//osPriorityRealtime
#define _HW_DEF_RTOS_THREAD_PRI_CLI           osPriorityNormal

#define _HW_DEF_RTOS_THREAD_MEM_MAIN          _HW_DEF_RTOS_MEM_SIZE( 1*1024)
#define _HW_DEF_RTOS_THREAD_MEM_CLI           _HW_DEF_RTOS_MEM_SIZE( 1*1024)


//#define _USE_HW_FLASH

//#define _USE_HW_RESET

#define _USE_HW_LED
#define      HW_LED_MAX_CH          1


#define _USE_HW_BUZZER

#define _USE_HW_GPIO
#define      HW_GPIO_MAX_CH         19
#define        _GPIO_SPI_NCS        0
#define        _GPIO_SPI_NSS        1
#define        _GPIO_IO_IN_01       2
#define        _GPIO_IO_IN_02       3
#define        _GPIO_IO_IN_03       4
#define        _GPIO_IO_IN_04       5
#define        _GPIO_IO_IN_05       6
#define        _GPIO_IO_IN_06       7
#define        _GPIO_IO_IN_07       8
#define        _GPIO_IO_IN_08       9
#define        _GPIO_IO_OUT_01      10
#define        _GPIO_IO_OUT_02      11
#define        _GPIO_IO_OUT_03      12
#define        _GPIO_IO_OUT_04      13
#define        _GPIO_IO_OUT_05      14
#define        _GPIO_IO_OUT_06      15
#define        _GPIO_IO_OUT_07      16
#define        _GPIO_IO_OUT_08      17
#define        _GPIO_I2C_INTERRUPT  18


#define _USE_HW_CODE_IDSW
#define 	   HW_CODE_MAX_CH         1

#define _USE_HW_UART
#define      HW_UART_MAX_CH         4
//#define _USE_HW_UART_1_DMA
//#define _USE_HW_UART_3_DMA


#define _USE_HW_LOG
#define      HW_LOG_CH              _DEF_UART1
#define      HW_LOG_BOOT_BUF_MAX    1024
#define      HW_LOG_LIST_BUF_MAX    2048

#define _USE_HW_CLI
#define      HW_CLI_CMD_LIST_MAX    16
#define      HW_CLI_CMD_NAME_MAX    16
#define      HW_CLI_LINE_HIS_MAX    4
#define      HW_CLI_LINE_BUF_MAX    64

//#define _USE_HW_CAN
#define      HW_CAN_MAX_CH          1
#define      HW_CAN_MSG_RX_BUF_MAX  16

//#define _USE_HW_SPI
#define      HW_SPI_MAX_CH          1
//#define        _SPI_W25Q16          _DEF_SPI1
//#define        _SPI_SLAVE           _DEF_SPI2


#define _USE_HW_I2C
#define      HW_I2C_MAX_CH          2




#endif /* SRC_HW_HW_DEF_H_ */
