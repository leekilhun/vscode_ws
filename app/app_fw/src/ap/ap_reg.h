
/*
 * ap_reg.h
 *
 *  Created on: 2022. 2. 22.
 *      Author: gns2l
 */

#ifndef SRC_AP_AP_REG_H_
#define SRC_AP_AP_REG_H_


/*
 *
 [ bank 0] error state [H]
  - bit 0          bit 1        bit 2       bit 3       bit 4       bit 5       bit 6       bit 7
    no error     no-resp LCD  no-resp Mot                                                motor not ready
 [ bank 1] error state [L]
  - bit 8          bit 9        bit 10      bit 11      bit 12      bit 13      bit 14      bit 15
    motor limit                                      cyl timeout vac timeout move timeout seq timeout
 [ bank 2]  setting
  - bit 16         bit 17       bit 18      bit 19      bit 20      bit 21      bit 22      bit 23
    use beep
 [ bank 3]  run state
  - bit 24         bit 25       bit 26      bit 27      bit 28      bit 29      bit 30      bit 31
    auto_run/stop  emg stop   auto_ready   error stop    pause     motor-on   orgin cplt     alarm
 *
 */

#define AP_REG_ERR_CLEAR             REG_BIT(0)
#define AP_REG_ERR_NO_RESP_LCD       REG_BIT(1)
#define AP_REG_ERR_NO_RESP_MOT       REG_BIT(2)
#define AP_REG_ERR_B03               REG_BIT(3)
#define AP_REG_ERR_B04               REG_BIT(4)
#define AP_REG_ERR_B05               REG_BIT(5)
#define AP_REG_ERR_B06               REG_BIT(6)
#define AP_REG_ERR_MOTOR_NOT_READY   REG_BIT(7)

#define AP_REG_ERR_MOTOR_LIMIT       REG_BIT(0)
#define AP_REG_ERR_CYL_INTERLOCK     REG_BIT(1)
#define AP_REG_ERR_B13               REG_BIT(2)
#define AP_REG_ERR_B14               REG_BIT(3)
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
#define AP_REG_DETECT_AREA_SEN       REG_BIT(4)
#define AP_REG_MOTOR_ON              REG_BIT(5)
#define AP_REG_ORG_COMPLETED         REG_BIT(6)
#define AP_REG_ALARM_STATUS          REG_BIT(7)

#define AP_REG_BANK_NO_MAX           (uint8_t)4
#define AP_REG_BANK_ERR_H            (uint8_t)0
#define AP_REG_BANK_ERR_L            (uint8_t)1
#define AP_REG_BANK_SETTING          (uint8_t)2
#define AP_REG_BANK_RUN_STATE        (uint8_t)3




#ifdef _USE_HW_RTOS
#define AP_REG_LOCK_BEGIN osMutexWait(ap_reg_mutex_id, osWaitForever)
#define AP_REG_LOCK_END   osMutexRelease(ap_reg_mutex_id)
#else
#define AP_REG_LOCK_BEGIN
#define AP_REG_LOCK_END
#endif


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
    motor_not_ready,

    motor_limit,
    reserved_11,
    reserved_12,
    reserved_13,
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


  _Regist   status{AP_REG_BANK_NO_MAX};
  uint8_t   bank_size;

  /*constructor*/
  Ap_reg():bank_size(AP_REG_BANK_NO_MAX){
#ifdef _USE_HW_RTOS
    osMutexDef(ap_reg_mutex_id);
    ap_reg_mutex_id = osMutexCreate (osMutex(ap_reg_mutex_id));
#endif
  }
  /*
  inline bool Setter(uint8_t* p_data, uint8_t len){
    bool ret = true;
    if (len > AP_REG_BANK_NO_MAX)
      return ret;
    for (uint8_t i = 0; i < len; i++) {
      status[i] = p_data[i];
    }
    return ret;
  }*/

  inline bool IsAutoReadyMode(){
    return status[AP_REG_BANK_RUN_STATE][AP_REG_AUTO_READY];
  }

  inline bool IsAutoRunning(){
    return status[AP_REG_BANK_RUN_STATE][AP_REG_AUTO_RUNNING];
  }

  inline void SetConfReg(uint8_t data){
    status[AP_REG_BANK_SETTING]= data;
  }

  inline uint16_t GetAlarmState(){
    uint16_t ret = 0;
    ret = status[AP_REG_BANK_ERR_L].get()<<8 | status[AP_REG_BANK_ERR_H].get()<<0;
    return ret;
  }
  inline void ClearAlarmState(){
    status[AP_REG_BANK_ERR_L] = 0;
    status[AP_REG_BANK_ERR_H] = 0;
  }

  inline void SetRunState(reg_ bit, bool on_off = true){
    AP_REG_LOCK_BEGIN;
    uint8_t value = status[AP_REG_BANK_RUN_STATE].get();
    switch (bit) {
      case AP_REG_AUTO_RUNNING:
        if(on_off)
        {
          value = value | B00000001;
          value = value & B11110001;
        }
        else
        {
          value = value & B11110000;
        }
        break;

      case AP_REG_EMG_STOP:
        if(on_off)
        {
          value = value | B00000010;
          value = value & B11110010;
        }
        else
        {
          value = value & B11111101;
        }
        break;

      case AP_REG_AUTO_READY:
        value = value | B00000100;
        value = value & B11110100;
        break;

      case AP_REG_ERROR_STOP:
        value = value | B00001000;
        value = value & B11111000;
        break;

      case AP_REG_DETECT_AREA_SEN:
        if(on_off)
        {
          value = value | B00010000;
        }
        else
          value = value & B11101111;
        break;

      case AP_REG_MOTOR_ON:
        if(on_off)
          value = value | B00100000;
        else
          value = value & B11011111;
        break;

      case AP_REG_ORG_COMPLETED:
        if(on_off)
          value = value | B01000000;
        else
          value = value & B10111111;
        break;

      case AP_REG_ALARM_STATUS:
        if(on_off)
          value = value | B10000000;
        else
          value = value & B01111111;
        break;

      default:
        break;
    }
    status[AP_REG_BANK_RUN_STATE] = value;
    AP_REG_LOCK_END;
  }


};

#endif /* SRC_AP_AP_REG_H_ */
