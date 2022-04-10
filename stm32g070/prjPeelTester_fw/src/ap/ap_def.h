#pragma once

#include "ext.h"

/*전역에서 접근할 헤더 */
#include "binary.h"
#include "util.h"

#include "def_err.h"
#include "def_obj.h"
#include "ap_util.h"

/* app 객체*/
#include "ap_dat.h"
#include "ap_reg.h"
#include "ap_evt.h"
#include "ap_log.h"

/* interface object*/
#include "interface/iio.h"
#include "interface/iaxis.h"
#include "interface/ivacuum.h"
#include "interface/icylinder.h"

/* ap io 관리 객체*/
#include "ap_io.h"

/* state machine data*/
struct prc_step_t
{
  uint8_t curr_step{};
  uint8_t pre_step{};

  inline void SetStep(uint8_t step)
  {
    pre_step = curr_step;
    curr_step = step;
  }

  inline uint8_t GetStep() const
  {
    return curr_step;
  }
};
