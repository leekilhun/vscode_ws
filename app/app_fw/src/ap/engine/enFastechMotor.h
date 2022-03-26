/*
 * enFastechMotor.h
 *
 *  Created on: 2022. 2. 22.
 *      Author: gns2l
 */

#ifndef SRC_AP_ENGINE_ENFASTECHMOTOR_H_
#define SRC_AP_ENGINE_ENFASTECHMOTOR_H_

#define DEF_ENGINE_FM_IO_USER_OUT_1         0x00008000
#define DEF_ENGINE_FM_IO_USER_OUT_2         0x00010000
#define DEF_ENGINE_FM_IO_USER_OUT_3         0x00020000
#define DEF_ENGINE_FM_IO_USER_OUT_4         0x00040000
#define DEF_ENGINE_FM_IO_USER_OUT_5         0x00080000
#define DEF_ENGINE_FM_IO_USER_OUT_6         0x00100000
#define DEF_ENGINE_FM_IO_USER_OUT_7         0x00200000
#define DEF_ENGINE_FM_IO_USER_OUT_8         0x00400000
#define DEF_ENGINE_FM_IO_USER_OUT_9         0x00800000
#define DEF_ENGINE_FM_IO_USER_OUT(x)        1<<x


#define DEF_FM_BANK_ERR_01          (uint8_t)0
#define DEF_FM_BANK_ERR_02          (uint8_t)1
#define DEF_FM_BANK_STATE_01        (uint8_t)2
#define DEF_FM_BANK_STATE_02        (uint8_t)3
#define DEF_FM_BANK_MAX             (uint8_t)4

/* fastech motor axis status*/
#define FM_REG_ERR_ALL               REG_BIT(0)
#define FM_REG_ERR_HW_PLUS_LIMIT     REG_BIT(1)
#define FM_REG_ERR_HW_MINUS_LIMIT    REG_BIT(2)
#define FM_REG_ERR_SW_PLUS_LIMIT     REG_BIT(3)
#define FM_REG_ERR_SW_MINUS_LIMIT    REG_BIT(4)
#define FM_REG_ERR_RESERVED          REG_BIT(5)
#define FM_REG_ERR_POS_CNT_OVER      REG_BIT(6)
#define FM_REG_ERR_SERVO_ALARM       REG_BIT(7)

#define FM_REG_ERR_OVER_CURRENT      REG_BIT(0)
#define FM_REG_ERR_OVER_SPEED        REG_BIT(1)
#define FM_REG_ERR_STEP_OUT          REG_BIT(2)
#define FM_REG_ERR_OVER_LOAD         REG_BIT(3)
#define FM_REG_ERR_OVER_HEAT         REG_BIT(4)
#define FM_REG_ERR_BACK_EMF          REG_BIT(5)
#define FM_REG_ERR_MOTOR_POWER       REG_BIT(6)
#define FM_REG_ERR_INPOSITION        REG_BIT(7)

#define FM_REG_EMG_STOP              REG_BIT(0)
#define FM_REG_SLOW_STOP             REG_BIT(1)
#define FM_REG_ORG_RETURNING         REG_BIT(2)
#define FM_REG_INPOSIOTION           REG_BIT(3)
#define FM_REG_SERVO_ON              REG_BIT(4)
#define FM_REG_ALARM_RESET           REG_BIT(5)
#define FM_REG_PT_STOPPED            REG_BIT(6)
#define FM_REG_ORIGIN_SENSOR         REG_BIT(7)

#define FM_REG_Z_PLUSE               REG_BIT(0)
#define FM_REG_ORG_RETURN_OK         REG_BIT(1)
#define FM_REG_MOTION_DIR            REG_BIT(2)
#define FM_REG_MOTIONING             REG_BIT(3)
#define FM_REG_MOTION_PAUSE          REG_BIT(4)
#define FM_REG_MOTION_ACCEL          REG_BIT(5)
#define FM_REG_MOTION_DECEL          REG_BIT(6)
#define FM_REG_MOTION_CONSTANT       REG_BIT(7)




class enFastechMotor:public IAxis,public IIO
{
  /****************************************************
   *  data
   ****************************************************/
  enum class cmd_t {
    motor_onoff,
    _max
  };

public:
  struct cfg_t  {
    Ap_reg* p_apReg;
    IIO* p_apIo;
    axis_dat* p_apDat;
    uint8_t ch;
    uint32_t baud;
  };

  enum class in_def {
    limit_plus, limit_minus,
    origin, clear_pos,
    pt_a0, pt_a1, pt_a2, pt_a3,

    pt_a4, pt_a5_user, pt_a6_user, pt_a7_user,
    pt_start, stop,  jog_plus, jog_minus,

    alarm_reset, servo_on, pause, origin_search,
    teaching, e_stop, jpt_in_0, jpt_in_1,

    jpt_in_2, jpt_start, user_in_0, user_in_1,
    user_in_2, user_in_3, user_in_4, user_in_5,

    user_in_6, user_in_7, user_in_8,
    _max
  };

  enum class out_def {
    compare_out,inposition,alarm, moving, acc_dec,ack,end,alarm_blink,

    org_search_ok,servo_ready,reserved,brake,
    pt_out0, pt_out1, pt_out2,    user_out_0,

    user_out_1, user_out_2, user_out_3, user_out_4,
    user_out_5, user_out_6, user_out_7, user_out_8,

    _max
  };


  enum class err_type {
    error_all,
    hw_plus_limit,
    hw_minus_limit,
    sw_plus_limit,
    sw_minus_limit,
    reserved_05,
    pos_cnt_over,
    servo_alarm,

    over_current,
    over_speed,
    step_out,
    over_load,
    over_heat,
    back_EMF,
    motor_power,
    inposition,
    _max
  };
#define FM_ERR(x)          static_cast<uint8_t>(jL_fastechMotor::err_type::x)


  struct motor_state_t
  {
    uint8_t   id{};
    uint8_t   sync_no{};
    uint8_t   is_motor_on{};
    uint16_t  alarm{};
    uint16_t  encoder{};
    _Regist   axis_status{4};
    _Regist    io_in{5};  // 모터 인풋 레지스터는 5바이트
    _Regist    io_out{4}; // 모터 아우풋 레지스터는 4바이트
    int       act_pos{};
    int       cmd_pos{};
    uint32_t  act_vel{};
    int       pos_err{};
    uint32_t  pt_item{};
  };

  uint8_t m_AxisId;
  motor_state_t m_AxisState;
  fastech_t m_Packet;
  bool m_IsConnected;

private:
  Ap_reg*   m_pApReg;
  IIO* m_pApIo;
  axis_dat* m_pApDat;

  /****************************************************
   *  Constructor
   ****************************************************/
public:
  enFastechMotor (uint8_t id);
  virtual  ~enFastechMotor ();


  /****************************************************
   *  func
   ****************************************************/
private:
  static void updateMotorReg(void* obj);


  int update_AxisState();

  void set_UserOutput(uint32_t mask_on, uint32_t mask_off);

public:
  /* Axis  함수*/
  int Init(enFastechMotor::cfg_t* cfg);
  void ProcessCmd();
  void CommRecovery();
  void SendGetAllStatus();
  bool IsBusy();
  bool IsOriginOK();
  int MotorOnOff(bool on_off);
  int JogMove(uint32_t cmd_vel = 1000, bool is_cw = true);
  int JogMove(uint32_t jog_pos, uint32_t cmd_vel = 1000, bool is_cw = true);

  int Move (int cmd_pos, uint32_t cmd_vel = 100, uint32_t acc = 100, uint32_t decel = 100);
  //축을 정지한다. (한개의 축에 대한 정지)
  int Stop ();
  //축을 등속이동에 대해 정지한다.
  int VStop ();
  // 축을 비상정지한다.
  int EStop ();



  bool IsMotorOn();
  int WaitDone(uint8_t mode);
  bool IsAxisDone();
  int  ClearState();

  int ClearPos();

  /* 함수*/
  inline Ap_reg* GetApReg() const{
    return m_pApReg;
  }

  /* IO  함수*/
  inline void SetBank_Out(uint8_t bank, uint8_t val){
    m_AxisState.io_out[bank] = val;
  }

  inline uint8_t GetBank_Out(uint8_t bank) {
    return m_AxisState.io_out[bank].get();
  }

  inline uint8_t GetBank_In(uint8_t bank) {
    return m_AxisState.io_in[bank].get();
  }

  /* IO  control function */
  inline bool IsOn(uint32_t addr) {
    bool ret = false;
    if (between(addr, 0, static_cast<uint32_t>(in_def::_max))) {

      div_t div_result = div(addr,8);
      ret = m_AxisState.io_in[(uint8_t)div_result.quot][REG_BIT(div_result.rem)];
    }
    return ret;
  }

  inline bool IsOff(uint32_t addr) {
    bool ret = false;
    if (between(addr, 0, static_cast<uint32_t>(in_def::_max))) {
      ret = IsOn(addr);
    }
    return ret;
  }

  inline int OutputOn(uint32_t addr) {
    int ret = -1;
    if (between(addr, static_cast<uint32_t>(out_def::user_out_0), static_cast<uint32_t>(out_def::_max))) {

      set_UserOutput(DEF_ENGINE_FM_IO_USER_OUT(addr),0);

      div_t div_result = div(addr,8);
      m_AxisState.io_out[(uint8_t)div_result.quot][REG_BIT(div_result.rem)] = true;
      ret = 0;
    }
    return ret;
  }


  inline int OutputOff(uint32_t addr) {
    int ret = -1;
    if (between(addr, static_cast<uint32_t>(out_def::user_out_0), static_cast<uint32_t>(out_def::_max))) {

      set_UserOutput(0,DEF_ENGINE_FM_IO_USER_OUT(addr));

      div_t div_result = div(addr,8);
      m_AxisState.io_out[(uint8_t)div_result.quot][REG_BIT(div_result.rem)] = false;
      ret = 0;
    }
    return ret;
  }

  inline int OutputToggle(uint32_t addr) {
    int ret = -1;
    if (between(addr, static_cast<uint32_t>(out_def::user_out_0), static_cast<uint32_t>(out_def::_max))) {
      div_t div_result = div(addr,8);
      if (m_AxisState.io_out[(uint8_t)div_result.quot][REG_BIT(div_result.rem)]){
        set_UserOutput(0,DEF_ENGINE_FM_IO_USER_OUT(addr));
        m_AxisState.io_out[(uint8_t)div_result.quot][REG_BIT(div_result.rem)] = false;
      }
      else{
        set_UserOutput(DEF_ENGINE_FM_IO_USER_OUT(addr),0);
        m_AxisState.io_out[(uint8_t)div_result.quot][REG_BIT(div_result.rem)] = true;
      }
      ret = 0;
    }
    return ret;
  }


};

#endif /* SRC_AP_ENGINE_ENFASTECHMOTOR_H_ */
