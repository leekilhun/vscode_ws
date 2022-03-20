/*
 * module.h
 *
 *  Created on: Dec 27, 2021
 *      Author: gns2.lee
 */

#ifndef SRC_MODULE_MODULE_H_
#define SRC_MODULE_MODULE_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "module_def.h"

#include "at24c64.h"
#include "opPanel.h"



bool moduleInit(void);

#ifdef __cplusplus
}
#endif

#endif /* SRC_MODULE_MODULE_H_ */
