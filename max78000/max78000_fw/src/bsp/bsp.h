/*
 * bsp.h
 *
 *  Created on: Jun 13, 2021
 *      Author: baram
 */

#ifndef SRC_BSP_BSP_H_
#define SRC_BSP_BSP_H_

#include "def.h"


#ifdef __cplusplus
 extern "C" {
#endif


#include "mxc_sys.h"
#include "mxc_delay.h"
#include "mxc_device.h"

#include "MAX78000/gpio.h"
#include "MAX78000/rtc.h"
#include "MAX78000/uart.h"


bool bspInit(void);

void delay(uint32_t time_ms);
uint32_t millis(void);


void logPrintf(const char *fmt, ...);


#ifdef __cplusplus
 }
#endif

#endif /* SRC_BSP_BSP_H_ */