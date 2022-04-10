/*
 * cnAutoManager.cpp
 *
 *  Created on: 2022. 2. 23.
 *      Author: gns2l
 */
#include "ap_def.h"
#include <control/cnAutoManager.h>

cnAutoManager::cnAutoManager ():m_pApReg(NULL)
{
  // TODO Auto-generated constructor stub
  m_OpMode = OP_MODE_STOP;
  m_OpStatus=OP_STEP_STOP;
  m_checkReady = false;
  m_step.SetStep(CN_AUTOMANAGER_STEP_INIT);
  m_pre_time = 0;
  m_retryCnt =0;
  m_FlagStartSw =0;

  m_pushSw[static_cast<uint8_t>(sw_e::start)].SetType(sw_e::start);
  m_pushSw[static_cast<uint8_t>(sw_e::stop)].SetType(sw_e::stop);
  m_pushSw[static_cast<uint8_t>(sw_e::reset)].SetType(sw_e::reset);

}

cnAutoManager::~cnAutoManager ()
{
  // TODO Auto-generated destructor stub
}

void cnAutoManager::Init(cnAutoManager::cfg_t &cfg)
{
  m_pApReg = cfg.p_apReg;
}

opMode cnAutoManager::GetOPMode()
{
  return m_OpMode;
}

opStatus cnAutoManager::GetOPStatus()
{
  return m_OpStatus;
}

void cnAutoManager::SetOPMode(opMode mode)
{
  opPanel_SetMode(mode);
  m_OpMode = mode;
}
void cnAutoManager::SetOPStatus(opStatus status)
{
  opPanel_SetStatus(status);
  m_OpStatus = status;
}

void cnAutoManager::StopSw()
{
  SetOPMode(OP_MODE_STOP);
  SetOPStatus(OP_STEP_STOP);
  m_checkReady = false;
}

void cnAutoManager::ResetSw()
{
  SetOPMode(OP_MODE_STOP);
  SetOPStatus(OP_STEP_STOP);
  m_pApReg->ClearAlarmState();
  m_checkReady = false;
}

void cnAutoManager::UiStarSw()
{
  m_FlagStartSw = true;
  m_checkReady = true;
}

void cnAutoManager::StartSw()
{

  if (m_pushSw[static_cast<uint8_t>(sw_e::start)].is_press == false)
  {
    m_pushSw[static_cast<uint8_t>(sw_e::start)].is_press = true;
    m_pushSw[static_cast<uint8_t>(sw_e::start)].delta_cnt = 0;
    m_pushSw[static_cast<uint8_t>(sw_e::start)].pre_ms = millis();
  }
  else
  {
    m_pushSw[static_cast<uint8_t>(sw_e::start)].pre_ms = millis();
  }

  //  m_checkReady ^= 1;
}


void cnAutoManager::AlarmAuto(cnAutoManager::state_e err)
{
  //m_pApReg->status[AP_REG_BANK_RUN_STATE][AP_REG_ALARM_STATUS]=true;
  //m_pApReg->SetRunState(AP_REG_ALARM_STATUS);
  switch (err)
  {
    case cnAutoManager::state_e::mcu_unit_err:
    case cnAutoManager::state_e::seq_initial_timeout:
    case cnAutoManager::state_e::emg_stop:
    case cnAutoManager::state_e::error_stop:
      break;

    case cnAutoManager::state_e::servo_on_err:
    case cnAutoManager::state_e::servo_off_err:
    case cnAutoManager::state_e::axis_origin_err:
      break;
    case cnAutoManager::state_e::axis_move_timeout:
    case cnAutoManager::state_e::axis_stop_timeout:
      break;
    case cnAutoManager::state_e::cyl_0_open_timeout:
    case cnAutoManager::state_e::cyl_1_open_timeout:
    case cnAutoManager::state_e::cyl_2_open_timeout:
    case cnAutoManager::state_e::cyl_3_open_timeout:
    case cnAutoManager::state_e::cyl_0_close_timeout:
    case cnAutoManager::state_e::cyl_1_close_timeout:
    case cnAutoManager::state_e::cyl_2_close_timeout:
    case cnAutoManager::state_e::cyl_3_close_timeout:
      m_pApReg->status[AP_REG_BANK_ERR_L][AP_REG_ERR_CYL_TIMEOUT]=true;
      break;
    case cnAutoManager::state_e::vac_0_on_timeout:
    case cnAutoManager::state_e::vac_1_on_timeout:
    case cnAutoManager::state_e::vac_2_on_timeout:
    case cnAutoManager::state_e::vac_0_off_timeout:
    case cnAutoManager::state_e::vac_1_off_timeout:
    case cnAutoManager::state_e::vac_2_off_timeout:
      m_pApReg->status[AP_REG_BANK_ERR_L][AP_REG_ERR_VAC_TIMEOUT]=true;
      break;
    default:
      break;
  }
  //m_pApReg->SetRunState(AP_REG_ALARM_STATUS, true);
}


int cnAutoManager::AutoReady()
{
  cnAutoManager::state_e state =  checkStartRunCondition();

  /*if (m_pApReg->status[AP_REG_BANK_RUN_STATE][AP_REG_ALARM_STATUS])
  {
    return ;
  }*/

  switch (state)
  {
    case cnAutoManager::state_e::ready:
    {
      if (m_FlagStartSw == true
          || m_pushSw[static_cast<uint8_t>(sw_e::start)].state == sw_event::short_key)
      {
        m_FlagStartSw = false;
        if (m_OpStatus == OP_START_RUN)
        {
          SetOPMode(OP_MODE_AUTORUN);
          SetOPStatus(OP_RUN);
        }
        else if (m_OpStatus != OP_RUN)
        {
          SetOPMode(OP_MODE_READY);
          SetOPStatus(OP_START_RUN);
        }
      }
    }
    break;

    case cnAutoManager::state_e::error_stop:
      AlarmAuto(state);
      break;
    case cnAutoManager::state_e::servo_on_err:
      AlarmAuto(state);
      break;
    case cnAutoManager::state_e::axis_origin_err:
      AlarmAuto(state);
      break;
    case cnAutoManager::state_e::mcu_unit_err:
      AlarmAuto(state);
      break;
    default:
      break;
  }

  return static_cast<int>(state);
}


cnAutoManager::state_e cnAutoManager::checkStartRunCondition()
{
  cnAutoManager::state_e ret = cnAutoManager::state_e::ready;

  if (m_pApReg->status[AP_REG_BANK_ERR_H][REG_BIT(AP_REG_ERR_CLEAR)])
  {
    // state commication, axis 등의 구성품의 상태가 문제가 없다면
    // 구동 중 발생되는 e-stop, error stop 에러 상태는 구성품 상태는 정상
    // 구동 중 발생되는 위 상태는 alarm 비트 활성화로 통합하여 표시
    if(m_pApReg->status[AP_REG_BANK_RUN_STATE][AP_REG_ALARM_STATUS])
    {
      if(m_pApReg->status[AP_REG_BANK_RUN_STATE][AP_REG_EMG_STOP])
      {
        ret = cnAutoManager::state_e::emg_stop;
      }
      else if(m_pApReg->status[AP_REG_BANK_RUN_STATE][AP_REG_ERROR_STOP])
      {
        ret = cnAutoManager::state_e::error_stop;
      }
    }
    else if (!m_pApReg->status[AP_REG_BANK_RUN_STATE][AP_REG_MOTOR_ON]) // not
    {
      ret =cnAutoManager::state_e::servo_on_err;
    }
    else if (!m_pApReg->status[AP_REG_BANK_RUN_STATE][AP_REG_ORG_COMPLETED]) // not
    {
      ret =cnAutoManager::state_e::axis_origin_err;
    }
    /*
    else
    {
      ret = cnAutoManager::state_e::ready;
    }*/
  }
  else
  {
    ret =cnAutoManager::state_e::mcu_unit_err;
  }

  return ret;
}


void cnAutoManager::pushSW(sw_t& sw)
{
  if (millis() - sw.pre_ms < 1)
  {
    ++sw.delta_cnt;
  }
  else if (millis() - sw.pre_ms < 50)
  {
    //
    if (m_step.GetStep() == CN_AUTOMANAGER_STEP_SW_CHECK_DOUBLE_WAIT)
    {
      sw.state = sw_event::double_key;
      sw.init();
    }
    else
    {
      if (millis() - sw.pre_ms >500)//double event check timeout
        sw.init();
      else
      {
        if(sw.type == cnAutoManager::sw_e::start)
        {
          m_checkReady = true;
        }
        sw.state = sw_event::short_key;
        m_step.SetStep(CN_AUTOMANAGER_STEP_SW_CHECK_DOUBLE);
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


void cnAutoManager::updateSw()
{
  m_pushSw[static_cast<uint8_t>(sw_e::start)].eventClear();
  m_pushSw[static_cast<uint8_t>(sw_e::stop)].eventClear();
  m_pushSw[static_cast<uint8_t>(sw_e::reset)].eventClear();

  if(!(m_pushSw[static_cast<uint8_t>(sw_e::start)].is_press
    || m_pushSw[static_cast<uint8_t>(sw_e::stop)].is_press
    || m_pushSw[static_cast<uint8_t>(sw_e::reset)].is_press)) //not
  {
    return;
  }

  if (m_pushSw[static_cast<uint8_t>(sw_e::start)].is_press)
    pushSW(m_pushSw[static_cast<uint8_t>(sw_e::start)]);

  if (m_pushSw[static_cast<uint8_t>(sw_e::stop)].is_press)
    pushSW(m_pushSw[static_cast<uint8_t>(sw_e::stop)]);

  if (m_pushSw[static_cast<uint8_t>(sw_e::reset)].is_press)
    pushSW(m_pushSw[static_cast<uint8_t>(sw_e::reset)]);
}

void cnAutoManager::ThreadJob()
{
  updateSw();
  doRunStep();

  if (m_checkReady)
  {
    AutoReady();
  }
}

void cnAutoManager::doRunStep()
{

  switch (m_step.GetStep())
  {
    case CN_AUTOMANAGER_STEP_INIT:
    {
      m_step.SetStep(CN_AUTOMANAGER_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
       to do
      ######################################################*/
    case CN_AUTOMANAGER_STEP_TODO:
    {
      m_retryCnt = 0;
      //m_pushSw = cnAutoManager::sw_e::none;
    }
    break;

    /*######################################################
       CN_AUTOMANAGER_STEP_SW_CHECK_DOUBLE
      ######################################################*/
    case CN_AUTOMANAGER_STEP_SW_CHECK_DOUBLE:
    {
      m_step.SetStep(CN_AUTOMANAGER_STEP_SW_CHECK_DOUBLE_START);
      m_pre_time = millis();
    }
    break;
    case CN_AUTOMANAGER_STEP_SW_CHECK_DOUBLE_START:
    {
      if (millis()-m_pre_time < 5)
        break;

      m_step.SetStep(CN_AUTOMANAGER_STEP_SW_CHECK_DOUBLE_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_AUTOMANAGER_STEP_SW_CHECK_DOUBLE_WAIT:
    {
      if (millis()-m_pre_time < 100)
        break;

      if (millis()-m_pre_time >= 1000)
      {
        m_step.SetStep(CN_AUTOMANAGER_STEP_TIMEOUT);
        m_pre_time = millis();
      }
    }
    break;
    case CN_AUTOMANAGER_STEP_SW_CHECK_DOUBLE_END:
    {
      m_step.SetStep(CN_AUTOMANAGER_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
       task timeout
      ######################################################*/
    case CN_AUTOMANAGER_STEP_TIMEOUT:
    {
      if (millis()-m_pre_time < 5)
        break;

      m_retryCnt = 0 ;
      m_step.SetStep(CN_AUTOMANAGER_STEP_TODO);
    }
    break;
    case CN_AUTOMANAGER_STEP_END:
    {

    }
    break;
    default:
      break;
  }//switch

}
