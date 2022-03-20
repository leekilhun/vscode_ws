/*
 * opPanel.c
 *
 *  Created on: Dec 29, 2021
 *      Author: gns2.lee
 */


#include "opPanel.h"
#include "cli.h"

#ifdef _USE_MODULE_OPPANEL
bool is_init = false;
opStatus op_Status;
opMode   op_Mode;
bool opPanel_Init()
{
  is_init = true;
  op_Status = OP_INIT_STATUS;
  op_Mode = OP_MODE_STOP;
  return is_init;
}

opStatus opPanel_GetStatus()
{
  return op_Status;
}

int opPanel_SetStatus(opStatus state)
{
  op_Status =  state;
  return 0;
}

opMode opPanel_GetMode()
{
  return op_Mode;
}

int opPanel_SetMode(opMode mode)
{
  op_Mode =  mode;
  return 0;
}



bool opPanel_GetPressed(opSwitch op_sw)
{
  uint8_t ch = 0;
  switch (op_sw)
  {
    case OP_SWITCH_START: ch = _GPIO_OP_SW_START; break;
    case OP_SWITCH_STOP:  ch = _GPIO_OP_SW_STOP;  break;
    case OP_SWITCH_RESET: ch = _GPIO_OP_SW_RESET; break;
    case OP_SWITCH_ESTOP: ch = _GPIO_OP_SW_ESTOP; break;
    default:
      break;
  }
  return gpioPinRead(ch);
}



#endif
