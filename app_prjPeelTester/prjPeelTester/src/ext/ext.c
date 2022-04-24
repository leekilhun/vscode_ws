/*
 * ext.c
 *
 *  Created on: Dec 27, 2021
 *      Author: gns2.lee
 */




#include "ext.h"

bool extInit(void)
{
  bool ret = true;
#ifdef _USE_EXT_AT24C64
  ret &= at24c64Init();
#endif
  // logBoot(false);

  return ret;
}
