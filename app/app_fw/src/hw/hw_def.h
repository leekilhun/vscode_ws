/*
 * hw_def.h
 *
 *  Created on: Dec 27, 2021
 *      Author: gns2.lee
 */

#ifndef SRC_HW_HW_DEF_H_
#define SRC_HW_HW_DEF_H_


#include "bsp.h"


//#define _USE_HW_RTOS  -> 프로젝트 전처리에서 선언 함

#define _HW_DEF_RTOS_MEM_SIZE(x)              ((x)/4)

#define _HW_DEF_RTOS_THREAD_PRI_MAIN          osPriorityNormal
#define _HW_DEF_RTOS_THREAD_PRI_FM_CMD        osPriorityNormal
#define _HW_DEF_RTOS_THREAD_PRI_LCD_CMD       osPriorityNormal
#define _HW_DEF_RTOS_THREAD_PRI_EVENT         osPriorityNormal


#define _HW_DEF_RTOS_THREAD_MEM_MAIN          _HW_DEF_RTOS_MEM_SIZE( 1*1024)
#define _HW_DEF_RTOS_THREAD_MEM_LCD_CMD       _HW_DEF_RTOS_MEM_SIZE( 2*1024)
#define _HW_DEF_RTOS_THREAD_MEM_FM_CMD        _HW_DEF_RTOS_MEM_SIZE( 2*1024)
#define _HW_DEF_RTOS_THREAD_MEM_EVENT         _HW_DEF_RTOS_MEM_SIZE( 1*1024)

#define _USE_HW_SWTIMER

#define _USE_HW_FLASH

//#define _USE_HW_RTC
//#define _USE_HW_RESET

#define _USE_HW_BUZZER
#define _USE_HW_LED
#define      HW_LED_MAX_CH          2

#define _USE_HW_PWM
#define      HW_PWM_MAX_CH          1

//#define _USE_HW_BUTTON
#define      HW_BUTTON_MAX_CH       2


#define _USE_HW_GPIO
#define      HW_GPIO_MAX_CH         25
#define        _GPIO_SPI_NCS        0
#define        _GPIO_SPI_NSS        1
#define        _GPIO_OP_SW_START    2
#define        _GPIO_OP_SW_STOP     3
#define        _GPIO_OP_SW_RESET    4
#define        _GPIO_OP_SW_ESTOP    5
#define        _GPIO_OP_LAMP_START  6
#define        _GPIO_OP_LAMP_STOP   7
#define        _GPIO_OP_LAMP_RESET  8
#define        _GPIO_IO_IN_01       9
#define        _GPIO_IO_IN_02       10
#define        _GPIO_IO_IN_03       11
#define        _GPIO_IO_IN_04       12
#define        _GPIO_IO_IN_05       13
#define        _GPIO_IO_IN_06       14
#define        _GPIO_IO_IN_07       15
#define        _GPIO_IO_IN_08       16
#define        _GPIO_IO_OUT_01      17
#define        _GPIO_IO_OUT_02      18
#define        _GPIO_IO_OUT_03      19
#define        _GPIO_IO_OUT_04      20
#define        _GPIO_IO_OUT_05      21
#define        _GPIO_IO_OUT_06      22
#define        _GPIO_IO_OUT_07      23
#define        _GPIO_IO_OUT_08      24



//#define _USE_HW_CDC
//#define _USE_HW_USB
#define      HW_USE_CDC             1
#define      HW_USE_MSC             0

#define _USE_HW_UART
#define      HW_UART_MAX_CH         4

#define _USE_HW_SW_TIMER
#define      HW_SW_TIMER_MAX_CH     4


//#define _USE_HW_CMD

#define _USE_HW_CMD_ROBOTRO
#define _USE_HW_CMD_FASTECH

#define _USE_HW_CMD_NEXTION
#define      HW_CMD_MAX_DATA_LENGTH 48

//#define _USE_HW_LOG
#define      HW_LOG_CH              _DEF_UART2
#define      HW_LOG_BOOT_BUF_MAX    1024
#define      HW_LOG_LIST_BUF_MAX    2048

/*simple logPrintf func*/
#define _USE_HW_SLOG
#define      HW_SLOG_CH              _DEF_UART2

//#define _USE_HW_CLI
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
