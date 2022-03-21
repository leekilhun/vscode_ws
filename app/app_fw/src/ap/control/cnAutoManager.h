/*
 * cnAutoManager.h
 *
 *  Created on: 2022. 2. 23.
 *      Author: gns2l
 */

#define CN_AUTOMANAGER_STEP_RETRY_CNT_MAX               3

#define CN_AUTOMANAGER_STEP_INIT                        0
#define CN_AUTOMANAGER_STEP_TODO                        1
#define CN_AUTOMANAGER_STEP_SW_CHECK_DOUBLE             2
#define CN_AUTOMANAGER_STEP_SW_CHECK_DOUBLE_START       3
#define CN_AUTOMANAGER_STEP_SW_CHECK_DOUBLE_WAIT        4
#define CN_AUTOMANAGER_STEP_SW_CHECK_DOUBLE_END         5

#define CN_AUTOMANAGER_STEP_TIMEOUT                     254
#define CN_AUTOMANAGER_STEP_END                         255


#ifndef SRC_AP_CONTROL_CNAUTOMANAGER_H_
#define SRC_AP_CONTROL_CNAUTOMANAGER_H_

class cnAutoManager
{
  /****************************************************
   *  data
   ****************************************************/
public:
  enum class state_e
  {
    ready,
    mcu_unit_err,
    emg_stop,
    error_stop,
    servo_on_err,
    axis_origin_err,
  };


  enum class sw_e
  {
    start,
    stop,
    reset,
    _max,
  };

  enum class sw_event
  {
    invalid,
    short_key,
    long_key,
    double_key,
  };

  struct sw_t
  {
    sw_e type{};
    bool is_press{};
    sw_event state{};
    volatile uint32_t delta_cnt{};
    uint32_t pre_ms{};

    inline void SetType(sw_e type){
      this->type = type;
    }

    inline void init(){
      is_press = false;
      delta_cnt = 0 ;
      pre_ms = 0;
    }

    inline void eventClear(){
      state = sw_event::invalid;
    }

  };

  struct cfg_t  {
    Ap_reg* p_apReg;
  };
  Ap_reg* m_pApReg;
private:
  opMode m_OpMode;
  opStatus m_OpStatus;
  bool m_checkReady;
  uint8_t m_retryCnt;
  uint32_t  m_pre_time;
  prc_step_t  m_step;
  sw_t m_pushSw[static_cast<uint8_t>(sw_e::_max)];
  bool m_FlagStartSw;

  /****************************************************
   *  Constructor
   ****************************************************/
public:
  cnAutoManager ();
  virtual  ~cnAutoManager ();

  /****************************************************
   *  func
   ****************************************************/
private:
  void doRunStep();
  state_e checkStartRunCondition();
  void pushSW(sw_t& sw);
  void updateSw();

public:
  void ThreadJob();
  // condition for auto-run
  int AutoReady();
  void StartSw();
  void StopSw();
  void ResetSw();
  void UiStarSw();
  void AlarmAuto(state_e err);
  opMode GetOPMode();
  opStatus GetOPStatus();
  void SetOPMode(opMode mode);
  void SetOPStatus(opStatus status);
};

#endif /* SRC_AP_CONTROL_CNAUTOMANAGER_H_ */
