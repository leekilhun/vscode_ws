#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "hw_def.h"

#include "led.h"
#include "uart.h"
#include "cli.h"
#include "i2c.h"
#include "spi.h"
#include "buzzer.h"
#include "flash.h"
#include "gpio.h"
#include "slog.h"
#include "commFastech.h"
#include "commRobotro.h"
#include "commNextion.h"


bool hwInit(void);

#ifdef __cplusplus
}
#endif

