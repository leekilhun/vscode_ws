/*
 * module.c
 *
 *  Created on: Dec 27, 2021
 *      Author: gns2.lee
 */




#include "module.h"


bool moduleInit(void)
{
  bool ret = true;

  ret &=  at24c64Init();
  ret &=  opPanel_Init();
  //logBoot(false);

  return ret;
}
