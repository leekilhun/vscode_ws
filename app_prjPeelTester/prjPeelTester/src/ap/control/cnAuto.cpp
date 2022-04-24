#include "ap.h"
#include "cnAuto.h"


#define CN_AUTO_ARG_TBL_CNT_MAX   10

#define CN_AUTO_MSG_STR_MAX        20
#define CN_AUTO_FILE_STR_MAX       40

#define OP_SW(x)                   static_cast<uint8_t>(sw_e::x)

cnAuto::cnAuto ():m_pApReg(NULL),m_pOP(NULL)
{
  // TODO Auto-generated constructor stub
  m_pApLog = NULL;
  m_pApIo = NULL;

  m_OpMode = enOp::mode::STOP;
  m_OpStatus=enOp::status::INIT;
  m_checkReady = false;
  m_step.SetStep(CN_AUTO_STEP_INIT);
  m_pre_time = 0;
  m_retryCnt =0;
  m_FlagStartSw =0;

  m_pushSw[OP_SW(start)].SetType(sw_e::start);
  m_pushSw[OP_SW(stop)].SetType(sw_e::stop);
  m_pushSw[OP_SW(reset)].SetType(sw_e::reset);

}

cnAuto::~cnAuto ()
{
  // TODO Auto-generated destructor stub
}

void cnAuto::Init(cnAuto::cfg_t &cfg)
{
  m_pApReg = cfg.p_apReg;
  m_pApIo = cfg.p_ApIo;
  m_pApLog = cfg.p_apLog;
  m_pOP = cfg.p_op;
}

enOp::mode cnAuto::GetOPMode()
{
  return m_OpMode;
}

enOp::status cnAuto::GetOPStatus()
{
  return m_OpStatus;
}

void cnAuto::SetOPMode(enOp::mode mode)
{
  m_pOP->SetMode(mode);
  m_OpMode = mode;
}
void cnAuto::SetOPStatus(enOp::status status)
{
  m_pOP->SetStatus(status);
  m_OpStatus = status;
}

void cnAuto::StopSw()
{
  SetOPMode(enOp::mode::STOP);
  SetOPStatus(enOp::status::STEP_STOP);
  m_checkReady = false;
}

void cnAuto::ResetSw()
{
  SetOPMode(enOp::mode::STOP);
  SetOPStatus(enOp::status::STEP_STOP);
  m_pApReg->ClearAlarmState();
  m_checkReady = false;
  m_IsDetectedPauseSensor = false;
}

void cnAuto::PauseStop()
{
  m_IsDetectedPauseSensor = true;
}

bool cnAuto::IsDetectAreaSensor()
{
  bool ret = false;
  if (!m_pApReg->IsSkipAreaSensor())//not
  {
    ret = m_pApIo->IsOff(static_cast<uint32_t>(ap_io::in_e::mcu_area_sensor));
  }
  ret |=m_IsDetectedPauseSensor;
 return ret;
}
void cnAuto::UiStarSw()
{
  m_FlagStartSw = true;
  m_checkReady = true;
}

void cnAuto::StartSw()
{
  if (m_pushSw[OP_SW(start)].is_press == false)
  {
    m_pushSw[OP_SW(start)].is_press = true;
    m_pushSw[OP_SW(start)].delta_cnt = 0;
    m_pushSw[OP_SW(start)].pre_ms = millis();
  }
  else
  {
    m_pushSw[OP_SW(start)].pre_ms = millis();
  }
}


void cnAuto::AlarmAuto(
    log_dat::head_t* p_head,
    const char* file,
    const char* func,
    const int line,
    const char* msg)
{
  cnAuto::state_e err = static_cast<cnAuto::state_e>(p_head->error_no);

  char tmp_str[CN_AUTO_FILE_STR_MAX] = { 0, };
  strcpy(tmp_str, file);
  char* arg_tbl[CN_AUTO_ARG_TBL_CNT_MAX] = {};
  uint8_t arg_cnt = trans::SplitArgs(tmp_str, arg_tbl, "/", CN_AUTO_ARG_TBL_CNT_MAX);
  //memset(tmp_str,0x00,CN_AUTO_FILE_STR_MAX);
 /* if (arg_tbl[arg_cnt - 1] != 0)
  {
    strcpy(tmp_str, arg_tbl[arg_cnt - 1]);
  }*/
  m_pApLog->apLogWrite(p_head, "file[%s],func[%s],msg[%s]",arg_tbl[arg_cnt - 1],func,msg);

  AlarmAuto(err);
}


void cnAuto::AlarmAuto(cnAuto::state_e err)
{
  switch (err)
  {
    case cnAuto::state_e::mcu_unit_err:
    case cnAuto::state_e::seq_initial_timeout:
    case cnAuto::state_e::emg_stop:
    case cnAuto::state_e::error_stop:
      break;

    case cnAuto::state_e::servo_on_err:
    case cnAuto::state_e::servo_off_err:
    case cnAuto::state_e::axis_origin_err:
      m_pApReg->status[AP_REG_BANK_ERR_H][AP_REG_ERR_MOTOR_NOT_READY]=true;
      break;

    case cnAuto::state_e::axis_move_timeout:
    case cnAuto::state_e::axis_stop_timeout:
      m_pApReg->status[AP_REG_BANK_ERR_L][AP_REG_ERR_MOVE_TIMEOUT]=true;
      break;

    case cnAuto::state_e::cyl_0_open_timeout:
    case cnAuto::state_e::cyl_1_open_timeout:
    case cnAuto::state_e::cyl_2_open_timeout:
    case cnAuto::state_e::cyl_3_open_timeout:
    case cnAuto::state_e::cyl_0_close_timeout:
    case cnAuto::state_e::cyl_1_close_timeout:
    case cnAuto::state_e::cyl_2_close_timeout:
    case cnAuto::state_e::cyl_3_close_timeout:
      m_pApReg->status[AP_REG_BANK_ERR_L][AP_REG_ERR_CYL_TIMEOUT]=true;
      break;

    case cnAuto::state_e::vac_0_on_timeout:
    case cnAuto::state_e::vac_1_on_timeout:
    case cnAuto::state_e::vac_2_on_timeout:
    case cnAuto::state_e::vac_0_off_timeout:
    case cnAuto::state_e::vac_1_off_timeout:
    case cnAuto::state_e::vac_2_off_timeout:
      m_pApReg->status[AP_REG_BANK_ERR_L][AP_REG_ERR_VAC_TIMEOUT]=true;
      break;
    default:
      break;
  }
  SetOPStatus(enOp::status::ERR_STOP);
}


int cnAuto::AutoReady()
{
  cnAuto::state_e state =  checkStartRunCondition();

  /*if (m_pApReg->status[AP_REG_BANK_RUN_STATE][AP_REG_ALARM_STATUS])
  {
    return ;
  }*/

  switch (state)
  {
    case cnAuto::state_e::ready:
    {
      if (m_FlagStartSw == true
          || m_pushSw[OP_SW(start)].state == sw_event::short_key)
      {
        m_FlagStartSw = false;
        if (m_OpStatus == enOp::status::RUN_READY)
        {
          if (m_pApReg->IsUseDryRun())
          {
            SetOPMode(enOp::mode::DRY_RUN);
          }
          else
          {
            SetOPMode(enOp::mode::AUTORUN);
          }
          SetOPStatus(enOp::status::RUN);
        }
        else if (m_OpStatus != enOp::status::RUN)
        {
          SetOPMode(enOp::mode::READY);
          SetOPStatus(enOp::status::RUN_READY);
        }
      }
    }
    break;

    case cnAuto::state_e::error_stop:
      AlarmAuto(state);
      break;
    case cnAuto::state_e::servo_on_err:
      AlarmAuto(state);
      break;
    case cnAuto::state_e::axis_origin_err:
      AlarmAuto(state);
      break;
    case cnAuto::state_e::mcu_unit_err:
      AlarmAuto(state);
      break;
    default:
      break;
  }

  return static_cast<int>(state);
}


cnAuto::state_e cnAuto::checkStartRunCondition()
{
  cnAuto::state_e ret = cnAuto::state_e::ready;

  if (m_pApReg->status[AP_REG_BANK_ERR_H][REG_BIT(AP_REG_ERR_CLEAR)])
  {
    // state commication, axis 등의 구성품의 상태가 문제가 없다면
    // 구동 중 발생되는 e-stop, error stop 에러 상태는 구성품 상태는 정상
    // 구동 중 발생되는 위 상태는 alarm 비트 활성화로 통합하여 표시
    if(m_pApReg->status[AP_REG_BANK_RUN_STATE][AP_REG_ALARM_STATUS])
    {
      if(m_pApReg->status[AP_REG_BANK_RUN_STATE][AP_REG_EMG_STOP])
      {
        ret = cnAuto::state_e::emg_stop;
      }
      else if(m_pApReg->status[AP_REG_BANK_RUN_STATE][AP_REG_ERROR_STOP])
      {
        ret = cnAuto::state_e::error_stop;
      }
    }
    else if (!m_pApReg->status[AP_REG_BANK_RUN_STATE][AP_REG_MOTOR_ON]) // not
    {
      ret =cnAuto::state_e::servo_on_err;
    }
    else if (!m_pApReg->status[AP_REG_BANK_RUN_STATE][AP_REG_ORG_COMPLETED]) // not
    {
      ret =cnAuto::state_e::axis_origin_err;
    }
  }
  else
  {
    ret =cnAuto::state_e::mcu_unit_err;
  }

  return ret;
}


void cnAuto::pushSW(sw_t& sw)
{
  if (millis() - sw.pre_ms < 1)
  {
    ++sw.delta_cnt;
  }
  else if (millis() - sw.pre_ms < 50)
  {
    //
    if (m_step.GetStep() == CN_AUTO_STEP_SW_CHECK_DOUBLE_WAIT)
    {
      sw.is_press = false;
      sw.state = sw_event::double_key;
      sw.init();
    }
    else
    {
      if (millis() - sw.pre_ms >500)//double event check timeout
        sw.init();
      else
      {
        if(sw.type == cnAuto::sw_e::start)
        {
          m_checkReady = true;
        }
        sw.state = sw_event::short_key;
        sw.is_press = false;
        m_step.SetStep(CN_AUTO_STEP_SW_CHECK_DOUBLE);
      }
    }

  }
  else
  {
    if (sw.delta_cnt >3000)
    {
      sw.state = sw_event::long_key;
    }
    else
    {
      sw.state = sw_event::short_key;
    }

    sw.init();
  }
}


void cnAuto::updateSw()
{
  m_pushSw[OP_SW(start)].eventClear();
  m_pushSw[OP_SW(stop)].eventClear();
  m_pushSw[OP_SW(reset)].eventClear();

  if(!(m_pushSw[OP_SW(start)].is_press
    || m_pushSw[OP_SW(stop)].is_press
    || m_pushSw[OP_SW(reset)].is_press)) //not
  {
    return;
  }

  if (m_pushSw[OP_SW(start)].is_press)
  {
    pushSW(m_pushSw[OP_SW(start)]);
  }
  if (m_pushSw[OP_SW(stop)].is_press)
  {
    pushSW(m_pushSw[OP_SW(stop)]);
  }
  if (m_pushSw[OP_SW(reset)].is_press)
  {
    pushSW(m_pushSw[OP_SW(reset)]);
  }
}

void cnAuto::ThreadJob()
{
  updateSw();
  doRunStep();

  if (m_checkReady)
  {
    AutoReady();
  }
}

void cnAuto::doRunStep()
{

  switch (m_step.GetStep())
  {
    case CN_AUTO_STEP_INIT:
    {
      m_step.SetStep(CN_AUTO_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
       to do
      ######################################################*/
    case CN_AUTO_STEP_TODO:
    {
      m_retryCnt = 0;
      //m_pushSw = cnAuto::sw_e::none;
    }
    break;

    /*######################################################
       CN_AUTO_STEP_SW_CHECK_DOUBLE
      ######################################################*/
    case CN_AUTO_STEP_SW_CHECK_DOUBLE:
    {
      m_step.SetStep(CN_AUTO_STEP_SW_CHECK_DOUBLE_START);
      m_pre_time = millis();
    }
    break;
    case CN_AUTO_STEP_SW_CHECK_DOUBLE_START:
    {
      if (millis()-m_pre_time < 5)
        break;

      m_step.SetStep(CN_AUTO_STEP_SW_CHECK_DOUBLE_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_AUTO_STEP_SW_CHECK_DOUBLE_WAIT:
    {
      if (millis()-m_pre_time < 100)
        break;

      if (millis()-m_pre_time >= 1000)
      {
        m_step.SetStep(CN_AUTO_STEP_TIMEOUT);
        m_pre_time = millis();
      }
    }
    break;
    case CN_AUTO_STEP_SW_CHECK_DOUBLE_END:
    {
      m_step.SetStep(CN_AUTO_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
       task timeout
      ######################################################*/
    case CN_AUTO_STEP_TIMEOUT:
    {
      if (millis()-m_pre_time < 5)
        break;

      m_retryCnt = 0 ;
      m_step.SetStep(CN_AUTO_STEP_TODO);
    }
    break;
    case CN_AUTO_STEP_END:
    {

    }
    break;
    default:
      break;
  }//switch

}
