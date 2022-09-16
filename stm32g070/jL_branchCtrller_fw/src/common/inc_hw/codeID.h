/*
 * codeID.h
 *
 *  Created on: Sep 4, 2022
 *      Author: gns2l
 */

#ifndef SRC_COMMON_INC_HW_CODEID_H_
#define SRC_COMMON_INC_HW_CODEID_H_



#ifdef __cplusplus
extern "C" {
#endif

#include "hw_def.h"


#ifdef _USE_HW_CODE_IDSW

bool codeIDsw_Init(void);
uint8_t codeIDsw_Read(uint8_t ch);

#endif

#ifdef __cplusplus
}
#endif


#endif /* SRC_COMMON_INC_HW_CODEID_H_ */
