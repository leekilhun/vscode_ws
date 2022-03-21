/*
 * ap_def.h
 *
 *  Created on: Dec 27, 2021
 *      Author: gns2.lee
 */

#ifndef SRC_AP_AP_DEF_H_
#define SRC_AP_AP_DEF_H_

#include "module.h"


#include "commFastech.h"
#include "commNextion.h"
#include "commRobotro.h"

#include "ap_util.h"

/* data object*/
#include "ap_obj.h"
#include "ap_dat.h"
#include "ap_log.h"

/* interface object*/
#include "interface/IIO.h"
#include "interface/IAxis.h"
#include "interface/IVacuum.h"
#include "interface/ICylinder.h"

/* ap object */
/* register -> communication -> basic unit(engine) -> combined unit(engine) -> control */
//register + io_manager
#include "ap_reg.h"
#include "ap_io.h"

//basic
#include "engine/enCylinder.h"
#include "engine/enVacuum.h"
#include "engine/enFastechMotor.h"


struct prc_step_t
{
  uint8_t curr_step{};
  uint8_t pre_step{};

  inline void SetStep(uint8_t step){
    pre_step = curr_step;
    curr_step = step;
  }

  inline uint8_t GetStep() const{
    return curr_step;
  }

} ;
//control
#include "control/cnAutoManager.h"
#include "control/cnProcess.h"


#include "ap_ui/uiNextionLcd.h"
#include "ap_ui/uiRemoteCtrl.h"

typedef struct
{
  bool (*keepLoop)(void);
} mode_args_t;



#endif /* SRC_AP_AP_DEF_H_ */
