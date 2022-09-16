/*
 * kdr10K_IDsw.h
 *
 *  Created on: 2022. 9. 3.
 *      Author: gns2l
 */

#ifndef SRC_COMMON_INC_EXHW_KDR10K_IDSW_H_
#define SRC_COMMON_INC_EXHW_KDR10K_IDSW_H_



#ifdef __cplusplus
extern "C" {
#endif

#include "exhw_def.h"


#ifdef _USE_EXHW_KDR10K_IDSW

bool kdr10K_IDswInit(void);
uint8_t kdr10K_IDswRead(uint8_t ch);

#endif

#ifdef __cplusplus
}
#endif


#endif /* SRC_COMMON_INC_EXHW_KDR10K_IDSW_H_ */
