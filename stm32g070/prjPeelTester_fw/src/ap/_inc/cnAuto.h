/*
 * cnAuto.h
 *
 *  Created on: 2022. 4. 7.
 *      Author: gns2l
 * 
 *  시퀀싱을 자동으로 운영하기 위해 상태 클래스
 */

#pragma once


#define CN_AUTO_STEP_RETRY_CNT_MAX               3

#define CN_AUTO_STEP_INIT                        0
#define CN_AUTO_STEP_TODO                        1
#define CN_AUTO_STEP_SW_CHECK_DOUBLE             2
#define CN_AUTO_STEP_SW_CHECK_DOUBLE_START       3
#define CN_AUTO_STEP_SW_CHECK_DOUBLE_WAIT        4
#define CN_AUTO_STEP_SW_CHECK_DOUBLE_END         5

#define CN_AUTO_STEP_TIMEOUT                     254
#define CN_AUTO_STEP_END                         255

#define AUTO_ALARM(head, msg)  AlarmAuto(head, (__FILE__), __FUNCTION__, __LINE__,  msg)

class cnAuto
{
  /****************************************************
   *  data
   ****************************************************/
public:
  enum class state_e
  {
    ready,
    mcu_unit_err,
    seq_initial_timeout,
    emg_stop,
    error_stop,

    servo_on_err,
    servo_off_err,
    axis_origin_err,
    axis_move_timeout,
    axis_stop_timeout,

    cyl_0_open_timeout,
    cyl_1_open_timeout,
    cyl_2_open_timeout,
    cyl_3_open_timeout,
    cyl_0_close_timeout,
    cyl_1_close_timeout,
    cyl_2_close_timeout,
    cyl_3_close_timeout,

    vac_0_on_timeout,
    vac_1_on_timeout,
    vac_2_on_timeout,
    vac_0_off_timeout,
    vac_1_off_timeout,
    vac_2_off_timeout,

    cyl_interlock_State,
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
    enOp* p_op;
    ap_log* p_apLog;
    iio * p_ApIo;
  };
private:
  Ap_reg* m_pApReg;
  enOp* m_pOP;
  ap_log* m_pApLog;
  iio * m_pApIo;

  enOp::mode m_OpMode;
  enOp::status m_OpStatus;
  bool m_checkReady;
  uint8_t m_retryCnt;
  uint32_t  m_pre_time;
  prc_step_t  m_step;
  sw_t m_pushSw[static_cast<uint8_t>(sw_e::_max)];
  bool m_FlagStartSw;
  bool m_IsDetectedPauseSensor;

  /****************************************************
   *  Constructor
   ****************************************************/
public:
  cnAuto ();
  virtual  ~cnAuto ();

  /****************************************************
   *  func
   ****************************************************/
private:
  void doRunStep();

  // 모듈의 자동 운전 조건을 확인
  state_e checkStartRunCondition();

  // 물리 key 이벤트 처리 (롱, 숏, 더블)
  void pushSW(sw_t& sw);
  void updateSw();

public:
  void Init(cnAuto::cfg_t &cfg);
  void ThreadJob();
  // condition for auto-run
  int AutoReady();
  void StartSw();
  void StopSw();
  void ResetSw();
  void PauseStop();
  void UiStarSw();
  void AlarmAuto(cnAuto::state_e err);
  void AlarmAuto(log_dat::head_t *p_head,
    const char* file,
    const char* func,
    const int line,
    const char* msg);
  enOp::mode GetOPMode();
  enOp::status GetOPStatus();
  void SetOPMode(enOp::mode mode);
  void SetOPStatus(enOp::status status);
  bool IsDetectAreaSensor();
};