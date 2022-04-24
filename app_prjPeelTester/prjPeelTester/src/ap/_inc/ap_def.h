/*
 * ap_def.h
 *
 *  Created on: Dec 27, 2021
 *      Author: gns2.lee
 */

#ifndef SRC_AP_AP_DEF_H_
#define SRC_AP_AP_DEF_H_

#include "ext.h"

#include "commFastech.h"
#include "commNextion.h"
#include "commRobotro.h"


#include "def_obj.h"

/* ap object*/
#include "ap_util.h"
#include "ap_dat.h"
#include "ap_evt.h"
#include "ap_log.h"


/* interface object*/
#include "interface/iio.h"
#include "interface/iaxis.h"
#include "interface/ivacuum.h"
#include "interface/icylinder.h"

/* ap object */
/* register -> communication -> basic unit(engine) -> combined unit(engine) -> control */
//register + io_manager
#include "ap_reg.h"
#include "ap_io.h"

//basic
#include "enOp.h"
#include "enCyl.h"
#include "enVac.h"
#include "enFaxis.h"


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
#include "cnAuto.h"
#include "cnDo.h"
#include "cnJob.h"


#include "apiNextion.h"
#include "apiRemote.h"



#endif /* SRC_AP_AP_DEF_H_ */
