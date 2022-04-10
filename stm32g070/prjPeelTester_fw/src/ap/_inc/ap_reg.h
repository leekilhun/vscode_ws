#pragma once

/*
 * ap_reg.h
 *
 *  Created on: 2022. 4. 7.
 *      Author: gns2l
 */



/*
 *
 [ bank 0] error state [H]
  - bit 0          bit 1        bit 2       bit 3       bit 4       bit 5       bit 6       bit 7
    no error     no-resp LCD  no-resp Mot
 [ bank 1] error state [L]
  - bit 8          bit 9        bit 10      bit 11      bit 12      bit 13      bit 14      bit 15
    hw +limit    hw -limit    sw +limit    sw -limit
 [ bank 2]  setting
  - bit 16         bit 17       bit 18      bit 19      bit 20      bit 21      bit 22      bit 23
    use beep
 [ bank 3]  run state
  - bit 24         bit 25       bit 26      bit 27      bit 28      bit 29      bit 30      bit 31
    auto_run/stop  emg stop   auto_ready   error stop              motor-on   orgin cplt     alarm
 *
 */

#define AP_REG_ERR_CLEAR             REG_BIT(0)
#define AP_REG_ERR_NO_RESP_LCD       REG_BIT(1)
#define AP_REG_ERR_NO_RESP_MOT       REG_BIT(2)
#define AP_REG_ERR_B03               REG_BIT(3)
#define AP_REG_ERR_B04               REG_BIT(4)
#define AP_REG_ERR_B05               REG_BIT(5)
#define AP_REG_ERR_B06               REG_BIT(6)
#define AP_REG_ERR_B07               REG_BIT(7)

#define AP_REG_ERR_HW_PLUS_LIMIT     REG_BIT(0)
#define AP_REG_ERR_HW_MINU_LIMIT     REG_BIT(1)
#define AP_REG_ERR_SW_PLUS_LIMIT     REG_BIT(2)
#define AP_REG_ERR_SW_MINU_LIMIT     REG_BIT(3)
#define AP_REG_ERR_CYL_TIMEOUT       REG_BIT(4)
#define AP_REG_ERR_VAC_TIMEOUT       REG_BIT(5)
#define AP_REG_ERR_MOVE_TIMEOUT      REG_BIT(6)
#define AP_REG_ERR_SEQ_TIMEOUT       REG_BIT(7)

#define AP_REG_USE_BEEP              REG_BIT(0)
#define AP_REG_B21                   REG_BIT(1)
#define AP_REG_B22                   REG_BIT(2)
#define AP_REG_B23                   REG_BIT(3)
#define AP_REG_B24                   REG_BIT(4)
#define AP_REG_B25                   REG_BIT(5)
#define AP_REG_B26                   REG_BIT(6)
#define AP_REG_B27                   REG_BIT(7)

#define AP_REG_AUTO_RUNNING          REG_BIT(0)
#define AP_REG_EMG_STOP              REG_BIT(1)
#define AP_REG_AUTO_READY            REG_BIT(2)
#define AP_REG_ERROR_STOP            REG_BIT(3)
#define AP_REG_B34                   REG_BIT(4)
#define AP_REG_MOTOR_ON              REG_BIT(5)
#define AP_REG_ORG_COMPLETED         REG_BIT(6)
#define AP_REG_ALARM_STATUS          REG_BIT(7)

#define AP_REG_BANK_NO_MAX           (uint8_t)4
#define AP_REG_BANK_ERR_H            (uint8_t)0
#define AP_REG_BANK_ERR_L            (uint8_t)1
#define AP_REG_BANK_SETTING          (uint8_t)2
#define AP_REG_BANK_RUN_STATE        (uint8_t)3







struct Ap_reg
{
  enum class err_t{
    no_error,
    no_response_lcd,
    no_response_mot,
    reserved_03,
    reserved_04,
    reserved_05,
    reserved_06,
    reserved_07,

    hw_plus_limit,
    hw_minus_limit,
    sw_plus_limit,
    sw_minus_limit,
    cyl_timeout,
    vac_timeout,
    move_timout,
    seq_timeout,
    _max
  };
#define AP_ERR_CODE(x)          static_cast<uint8_t>(Ap_reg::err_t::x)

#ifdef _USE_HW_RTOS
  osMutexId ap_reg_mutex_id;
#endif


  fdata   status{AP_REG_BANK_NO_MAX};
  uint8_t   bank_size;

  /*constructor*/
  Ap_reg();
  
  bool IsAutoReadyMode();

  bool IsAutoRunning();

  void SetConfReg(uint8_t data);

  uint8_t GetAlarmState();

  void ClearAlarmState();

  void SetRunState(bit_e bit, bool on_off = true);


};