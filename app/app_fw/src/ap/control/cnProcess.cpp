/*
 * cnProcess.cpp
 *
 *  Created on: 2022. 3. 1.
 *      Author: gns2l
 */
#include "ap_def.h"
#include <control/cnProcess.h>

#define CN_PROCESS_STEP_RETRY_CNT_MAX                 3
#define CN_PROCESS_WAIT_TIMEOUT                       100
#define CN_PROCESS_MOTOR_MOVE_TIMEOUT                 (1000*3)

#define CN_PROCESS_BUFF_ASSIGN_MAX_CNT                m_stepBuffer[m_stepBuffer[0]+2]
#define CN_PROCESS_BUFF_ASSIGN_DATA_LENGTH            m_stepBuffer[0]
#define CN_PROCESS_BUFF_ASSIGN_INCREASE_NUM           m_stepBuffer[m_stepBuffer[0]+4]
#define CN_PROCESS_BUFF_ASSIGN_OFFSET                 m_stepBuffer[m_stepBuffer[0]+3]

cnProcess::cnProcess ():m_pApReg(NULL)
{
  m_pApIo = NULL;
  m_pFm = NULL;
  m_pCyl = NULL;
  m_pVac = NULL;
  m_pApAxisDat = NULL;
  m_pApCylDat = NULL;
  m_pApVacDat = NULL;
  m_pApCfgDat = NULL;
  m_pApSeqDat = NULL;
  m_pAuto = NULL;
  m_IsToDoState = false;
  m_pre_time = 0;
  m_step = {};
  m_retryCnt = 0;
  m_IsAutoReady = false;
  memset(&m_stepBuffer,0,CN_PROCESS_MAX_BUFFER_LENGTH);

}

cnProcess::~cnProcess ()
{
  // TODO Auto-generated destructor stub
}


int cnProcess::Init(cnProcess::cfg_t* cfg)
{
  m_pApReg = cfg->p_apReg;
  m_pApIo = cfg->p_apIo;
  m_pFm = cfg->p_Fm;
  m_pCyl = cfg->p_Cyl;
  m_pVac = cfg->p_Vac;
  m_pAuto = cfg->p_AutoManger;
  m_pApAxisDat = cfg->p_apAxisDat;
  m_pApCylDat = cfg->p_apCylDat;
  m_pApVacDat = cfg->p_apVacDat;
  m_pApCfgDat = cfg->p_apCfgDat;
  m_pApSeqDat = cfg->p_apSeqDat;
  m_pre_time = 0;
  m_step.SetStep(CN_PROCESS_STEP_INIT);
  memset(&m_stepBuffer,0,CN_PROCESS_MAX_BUFFER_LENGTH);


  //
  return 0;
}

void cnProcess::doRunStep()
{

  switch (m_step.GetStep())
  {
    case CN_PROCESS_STEP_INIT:
    {
      memset(&m_stepBuffer, 0, CN_PROCESS_MAX_BUFFER_LENGTH);
      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
       to do
      ######################################################*/
    case CN_PROCESS_STEP_TODO:
    {
      m_retryCnt = 0;
      m_IsToDoState = true;
    }
    break;

    /*######################################################
       phone jig open
      ######################################################*/
    case CN_PROCESS_STEP_PHONE_JIG_OPEN:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_PROCESS_STEP_PHONE_JIG_OPEN_START);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_PHONE_JIG_OPEN_START:
    {
      //
      m_pCyl[AP_DEF_OBJ_CYLINDER_ID_PHONE_JIG].Open(true);
      m_step.SetStep(CN_PROCESS_STEP_PHONE_JIG_OPEN_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_PHONE_JIG_OPEN_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_WAIT_TIMEOUT)
      {
        if (m_retryCnt++ < CN_PROCESS_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_PROCESS_STEP_PHONE_JIG_OPEN_START);
          m_pre_time = millis();
        }
        else
        {
          m_retryCnt = 0;
          m_pAuto->AlarmAuto(cnAutoManager::state_e::cyl_0_open_timeout);
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          m_pre_time = millis();
        }
      }

      if (m_pCyl[AP_DEF_OBJ_CYLINDER_ID_PHONE_JIG].IsOpen())// is open jig
      {
        m_step.SetStep(CN_PROCESS_STEP_PHONE_JIG_OPEN_END);
        m_pre_time = millis();
      }      
    }
    break;
    case CN_PROCESS_STEP_PHONE_JIG_OPEN_END:
    {
      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;

    /*######################################################
       phone jig close
      ######################################################*/
    case CN_PROCESS_STEP_PHONE_JIG_CLOSE:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_PROCESS_STEP_PHONE_JIG_CLOSE_START);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_PHONE_JIG_CLOSE_START:
    {
      //
      m_pCyl[AP_DEF_OBJ_CYLINDER_ID_PHONE_JIG].Close(true);
      m_step.SetStep(CN_PROCESS_STEP_PHONE_JIG_CLOSE_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_PHONE_JIG_CLOSE_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_WAIT_TIMEOUT)
      {
        if (m_retryCnt++ < CN_PROCESS_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_PROCESS_STEP_PHONE_JIG_CLOSE_START);
          m_pre_time = millis();
        }
        else
        {
          m_retryCnt = 0;
          m_pAuto->AlarmAuto(cnAutoManager::state_e::cyl_0_close_timeout);
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          m_pre_time = millis();
        }
      }

      if (m_pCyl[AP_DEF_OBJ_CYLINDER_ID_PHONE_JIG].IsClose()) // is close jig
      {
        m_step.SetStep(CN_PROCESS_STEP_PHONE_JIG_CLOSE_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_PROCESS_STEP_PHONE_JIG_CLOSE_END:
    {
      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;

#if 0  // not use in auto run
    /*######################################################
       axis pos get origin
      ######################################################*/
    case CN_PROCESS_STEP_POS_ORIGIN:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_PROCESS_STEP_POS_ORIGIN_START);      
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_ORIGIN_START:
    {
      //
      m_IsToDoState = false;
      m_step.SetStep(CN_PROCESS_STEP_POS_ORIGIN_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_ORIGIN_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_WAIT_TIMEOUT)
      {
        if (m_retryCnt++ < CN_PROCESS_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_PROCESS_STEP_POS_ORIGIN_START);
          m_pre_time = millis();
        }
        else
        {
          m_retryCnt = 0;
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          m_pre_time = millis();
        }
      }

      if (1)// is ok
      {
        m_step.SetStep(CN_PROCESS_STEP_POS_ORIGIN_END);
        m_pre_time = millis();
      } 
    }
    break;
    case CN_PROCESS_STEP_POS_ORIGIN_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;
#endif
    /*######################################################
       move axis position ready (process start)
      ######################################################*/
    case CN_PROCESS_STEP_POS_READY:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_PROCESS_STEP_POS_READY_START);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_READY_START:
    {
      //
      axis_dat::dat_t pos_data = m_pApAxisDat->cmd_pos_dat[static_cast<uint8_t>(axis_dat::addr_e::ready_pos)];
      m_pFm->Move(pos_data.cmd_pos, pos_data.cmd_vel);

      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (m_pFm->IsAxisDone())
          break;

        if (millis()-pre_ms >= CN_PROCESS_WAIT_TIMEOUT)
        {
          break;
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAutoManager::state_e::axis_move_timeout);
        }
        delay(1);
      }

      m_step.SetStep(CN_PROCESS_STEP_POS_READY_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_READY_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_MOTOR_MOVE_TIMEOUT)
      {
        m_retryCnt = 0;
        m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_PROCESS_STEP_POS_READY_END);
        m_pre_time = millis();
      } 
    }
    break;
    case CN_PROCESS_STEP_POS_READY_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
       move axis position clean dust rolling
      ######################################################*/
    case CN_PROCESS_STEP_POS_CLEAN_DUST:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_PROCESS_STEP_POS_CLEAN_DUST_START);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_CLEAN_DUST_START:
    {
      //
      axis_dat::dat_t pos_data = m_pApAxisDat->cmd_pos_dat[static_cast<uint8_t>(axis_dat::addr_e::pos_clean_dust)];
      m_pFm->Move(pos_data.cmd_pos, pos_data.cmd_vel);

      m_step.SetStep(CN_PROCESS_STEP_POS_CLEAN_DUST_WAIT);

      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (m_pFm->IsAxisDone())
          break;

        if (millis()-pre_ms >= CN_PROCESS_WAIT_TIMEOUT)
        {
          break;
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAutoManager::state_e::axis_move_timeout);
        }
        delay(1);
      }
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_CLEAN_DUST_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_MOTOR_MOVE_TIMEOUT)
      {
        m_retryCnt = 0;
        m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_PROCESS_STEP_POS_CLEAN_DUST_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_PROCESS_STEP_POS_CLEAN_DUST_END:
    {
      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
       move axis position vacuum suction to peel vinyl off
      ######################################################*/
    case CN_PROCESS_STEP_POS_SUCTION_VINYL:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_PROCESS_STEP_POS_SUCTION_VINYL_START);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_SUCTION_VINYL_START:
    {
      //
      axis_dat::dat_t pos_data = m_pApAxisDat->cmd_pos_dat[static_cast<uint8_t>(axis_dat::addr_e::pos_vinyl_suction)];
      m_pFm->Move(pos_data.cmd_pos, pos_data.cmd_vel);

      m_step.SetStep(CN_PROCESS_STEP_POS_SUCTION_VINYL_WAIT);

      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (m_pFm->IsAxisDone())
          break;

        if (millis()-pre_ms >= CN_PROCESS_WAIT_TIMEOUT)
        {
          break;
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAutoManager::state_e::axis_move_timeout);
        }
        delay(1);
      }
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_SUCTION_VINYL_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_MOTOR_MOVE_TIMEOUT)
      {
        m_retryCnt = 0;
        m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_PROCESS_STEP_POS_SUCTION_VINYL_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_PROCESS_STEP_POS_SUCTION_VINYL_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
       move axis position to peel vinyl off
      ######################################################*/
    case CN_PROCESS_STEP_POS_PEEL_VINYL:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_PROCESS_STEP_POS_PEEL_VINYL_START);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_PEEL_VINYL_START:
    {
      axis_dat::dat_t pos_data = m_pApAxisDat->cmd_pos_dat[static_cast<uint8_t>(axis_dat::addr_e::pos_vinyl_peel)];
      m_pFm->Move(pos_data.cmd_pos, pos_data.cmd_vel);

      m_step.SetStep(CN_PROCESS_STEP_POS_PEEL_VINYL_WAIT);

      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (m_pFm->IsAxisDone())
          break;

        if (millis()-pre_ms >= CN_PROCESS_WAIT_TIMEOUT)
        {
          break;
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAutoManager::state_e::axis_move_timeout);
        }
        delay(1);
      }
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_PEEL_VINYL_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_MOTOR_MOVE_TIMEOUT)
      {
        m_retryCnt = 0;
        m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_PROCESS_STEP_POS_PEEL_VINYL_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_PROCESS_STEP_POS_PEEL_VINYL_END:
    {
      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
       move axis position complete
      ######################################################*/
    case CN_PROCESS_STEP_POS_PEEL_VINYL_CPLT:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_PROCESS_STEP_POS_PEEL_VINYL_CPLT_START);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_PEEL_VINYL_CPLT_START:
    {
      //
      axis_dat::dat_t pos_data = m_pApAxisDat->cmd_pos_dat[static_cast<uint8_t>(axis_dat::addr_e::pos_peel_cplt)];
      m_pFm->Move(pos_data.cmd_pos, pos_data.cmd_vel);

      m_step.SetStep(CN_PROCESS_STEP_POS_PEEL_VINYL_CPLT_WAIT);

      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (m_pFm->IsAxisDone())
          break;

        if (millis()-pre_ms >= CN_PROCESS_WAIT_TIMEOUT)
        {
          break;
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAutoManager::state_e::axis_move_timeout);
        }
        delay(1);
      }
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_PEEL_VINYL_CPLT_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_MOTOR_MOVE_TIMEOUT)
      {
        m_retryCnt = 0;
        m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_PROCESS_STEP_POS_PEEL_VINYL_CPLT_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_PROCESS_STEP_POS_PEEL_VINYL_CPLT_END:
    {
      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;

    /*######################################################
       move axis position start reattach vinyl
      ######################################################*/
    case CN_PROCESS_STEP_POS_REATTACH:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_PROCESS_STEP_POS_REATTACH_START);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_REATTACH_START:
    {
      //
      axis_dat::dat_t pos_data = m_pApAxisDat->cmd_pos_dat[static_cast<uint8_t>(axis_dat::addr_e::pos_vinyl_reattach)];
      m_pFm->Move(pos_data.cmd_pos, pos_data.cmd_vel);

      m_step.SetStep(CN_PROCESS_STEP_POS_REATTACH_WAIT);

      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (m_pFm->IsAxisDone())
          break;

        if (millis()-pre_ms >= CN_PROCESS_WAIT_TIMEOUT)
        {
          break;
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAutoManager::state_e::axis_move_timeout);
        }
        delay(1);
      }
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_REATTACH_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_MOTOR_MOVE_TIMEOUT)
      {
        m_retryCnt = 0;
        m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_PROCESS_STEP_POS_REATTACH_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_PROCESS_STEP_POS_REATTACH_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
       move axis position run reattach vinyl on phone
      ######################################################*/
    case CN_PROCESS_STEP_POS_REATTACH_RUN:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_PROCESS_STEP_POS_REATTACH_RUN_START);      
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_REATTACH_RUN_START:
    {
      //
      axis_dat::dat_t pos_data = m_pApAxisDat->cmd_pos_dat[static_cast<uint8_t>(axis_dat::addr_e::pos_5)];
      m_pFm->Move(pos_data.cmd_pos, pos_data.cmd_vel);

      m_step.SetStep(CN_PROCESS_STEP_POS_REATTACH_RUN_WAIT);

      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (m_pFm->IsAxisDone())
          break;

        if (millis()-pre_ms >= CN_PROCESS_WAIT_TIMEOUT)
        {
          break;
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAutoManager::state_e::axis_move_timeout);
        }
        delay(1);
      }
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_REATTACH_RUN_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_MOTOR_MOVE_TIMEOUT)
      {
        m_retryCnt = 0;
        m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_PROCESS_STEP_POS_REATTACH_RUN_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_PROCESS_STEP_POS_REATTACH_RUN_END:
    {
      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
       peel drum unit get origin position
      ######################################################*/
    case CN_PROCESS_STEP_ROLLER_ORIGIN:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_PROCESS_STEP_ROLLER_ORIGIN_START);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_ROLLER_ORIGIN_START:
    {
      //
      m_IsToDoState = false;
      m_step.SetStep(CN_PROCESS_STEP_ROLLER_ORIGIN_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_ROLLER_ORIGIN_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_WAIT_TIMEOUT)
      {
        if (m_retryCnt++ < CN_PROCESS_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_PROCESS_STEP_ROLLER_ORIGIN_START);
          m_pre_time = millis();
        }
        else
        {
          m_retryCnt = 0;
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          m_pre_time = millis();
        }
      }

      if (1) // is ok
      {
        m_step.SetStep(CN_PROCESS_STEP_ROLLER_ORIGIN_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_PROCESS_STEP_ROLLER_ORIGIN_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;

    /*######################################################
       task timeout
      ######################################################*/
    case CN_PROCESS_STEP_TIMEOUT:
    {
      if (millis() - m_pre_time < 5)
        break;

      m_retryCnt = 0;
      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_END:
    {

    }
    break;
    default:
      break;
  }//switch
}

int cnProcess::Initialize()
{
  // vacuum
  m_step.SetStep(CN_PROCESS_STEP_SEQ_INITIAL);
  m_pre_time = millis();
  return 0;
}

void cnProcess::doOpJob()
{

  switch (m_step.GetStep())
  {
    case CN_PROCESS_STEP_INIT:
    {
      memset(&m_stepBuffer, 0, CN_PROCESS_MAX_BUFFER_LENGTH);
      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
         to do
      ######################################################*/
    case CN_PROCESS_STEP_TODO:
    {
      m_retryCnt = 0;
      m_IsToDoState = true;
    }
    break;
    /*######################################################
        operating sequence initial
      ######################################################*/
    case CN_PROCESS_STEP_SEQ_INITIAL:
    {
      m_retryCnt = 0;
      m_IsToDoState = false;
      m_step.SetStep(CN_PROCESS_STEP_SEQ_INITIAL_START);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_SEQ_INITIAL_START:
    {
      // vacuum
      {
        m_pVac[AP_DEF_OBJ_VACUUM_ID_DRUM_HEAD].Off(true);
        m_pVac[AP_DEF_OBJ_VACUUM_ID_DRUM_TAIL].Off(true);
        m_pVac[AP_DEF_OBJ_VACUUM_ID_PHONE_JIG].Off(true);
      }

      // cylinder
      {
        m_pCyl[AP_DEF_OBJ_CYLINDER_ID_DRUM_UPDOWN].Up(true);
        m_pCyl[AP_DEF_OBJ_CYLINDER_ID_DRUM_Z_UP].Up(true);
        m_pCyl[AP_DEF_OBJ_CYLINDER_ID_DRUM_STOP].Unlock(true);
      }

      // motor
      {
        axis_dat::dat_t pos_data =  m_pApAxisDat->cmd_pos_dat[static_cast<uint8_t>(axis_dat::addr_e::ready_pos)];
        m_pFm->Move(pos_data.cmd_pos, pos_data.cmd_vel);
      }

      m_step.SetStep(CN_PROCESS_STEP_SEQ_INITIAL_WAIT);
      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (millis()-pre_ms >=100)
        {
          break;
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAutoManager::state_e::seq_initial_timeout);
        }
      }
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_SEQ_INITIAL_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_WAIT_TIMEOUT*100)
      {
        m_retryCnt = 0;
        m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {

        m_pCyl[AP_DEF_OBJ_CYLINDER_ID_PHONE_JIG].Open(true);
        m_step.SetStep(CN_PROCESS_STEP_SEQ_INITIAL_END);
        m_pre_time = millis();
      }

    }
    break;
    case CN_PROCESS_STEP_SEQ_INITIAL_END:
    {
      // state
      m_pApReg->SetRunState(AP_REG_ORG_COMPLETED, true);

      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
         operating motor enable
      ######################################################*/
    case CN_PROCESS_STEP_MOTOR_ON:
    {
      if (m_pFm->IsMotorOn())
      {
        m_step.SetStep(CN_PROCESS_STEP_MOTOR_ON_END);
        break;
      }
      m_retryCnt = 0;
      m_step.SetStep(CN_PROCESS_STEP_MOTOR_ON_START);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_MOTOR_ON_START:
    {
      if (m_pFm->IsMotorOn())
      {
        m_step.SetStep(CN_PROCESS_STEP_MOTOR_ON_END);
        break;
      }
      m_IsToDoState = false;

      m_pFm->MotorOnOff(true);
      m_step.SetStep(CN_PROCESS_STEP_MOTOR_ON_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_MOTOR_ON_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_WAIT_TIMEOUT)
      {
        if (m_retryCnt++ < CN_PROCESS_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_PROCESS_STEP_MOTOR_ON_START);
          m_pre_time = millis();
        }
        else
        {
          m_retryCnt = 0;
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          m_pAuto->AlarmAuto(cnAutoManager::state_e::servo_on_err);
          m_pre_time = millis();
        }
      }

      if (m_pFm->IsMotorOn())
      {
        m_step.SetStep(CN_PROCESS_STEP_MOTOR_ON_END);
        m_pre_time = millis();
      }
      
    }
    break;
    case CN_PROCESS_STEP_MOTOR_ON_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
         operating motor disable
      ######################################################*/
    case CN_PROCESS_STEP_MOTOR_OFF:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_PROCESS_STEP_MOTOR_OFF_START);      
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_MOTOR_OFF_START:
    {
      m_IsToDoState = false;

      m_pFm->MotorOnOff(false);
      m_step.SetStep(CN_PROCESS_STEP_MOTOR_OFF_WAIT);      
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_MOTOR_OFF_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_WAIT_TIMEOUT)
      {
        if (m_retryCnt++ < CN_PROCESS_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_PROCESS_STEP_MOTOR_OFF_START);
          m_pre_time = millis();
        }
        else
        {
          m_retryCnt = 0;
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          m_pAuto->AlarmAuto(cnAutoManager::state_e::servo_on_err);
          m_pre_time = millis();
        }
      }

      if (!m_pFm->IsMotorOn())
      {
        m_step.SetStep(CN_PROCESS_STEP_MOTOR_OFF_END);
        m_pre_time = millis();
      }

    }
    break;
    case CN_PROCESS_STEP_MOTOR_OFF_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
         operating motor move stop
      ######################################################*/
    case CN_PROCESS_STEP_MOTOR_STOP:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_PROCESS_STEP_MOTOR_STOP_START);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_MOTOR_STOP_START:
    {
      m_IsToDoState = false;

      m_pFm->Stop();
      m_step.SetStep(CN_PROCESS_STEP_MOTOR_STOP_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_MOTOR_STOP_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_WAIT_TIMEOUT)
      {
        if (m_retryCnt++ < CN_PROCESS_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_PROCESS_STEP_MOTOR_STOP_START);
          m_pre_time = millis();
        }
        else
        {
          m_retryCnt = 0;
          m_pAuto->AlarmAuto(cnAutoManager::state_e::axis_stop_timeout);
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          m_pre_time = millis();
        }
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_PROCESS_STEP_MOTOR_STOP_END);
        m_pre_time = millis();
      }

    }
    break;
    case CN_PROCESS_STEP_MOTOR_STOP_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;

    /*######################################################
         operating motor move move
      ######################################################*/
    case CN_PROCESS_STEP_MOTOR_MOVE:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_PROCESS_STEP_MOTOR_MOVE_START);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_MOTOR_MOVE_START:
    {
      m_IsToDoState = false;

      int cmd_pos = 0;
      cmd_pos = utilDwToInt(&m_stepBuffer[0]);
      uint32_t cmd_vel = 0;
      cmd_vel = utilDwToUint(&m_stepBuffer[4]);
      uint32_t acc = 0;
      acc = utilDwToUint(&m_stepBuffer[8]);
      uint32_t decel = 0;
      decel = utilDwToUint(&m_stepBuffer[12]);

      m_pFm->Move(cmd_pos, cmd_vel, acc, decel);

      m_step.SetStep(CN_PROCESS_STEP_MOTOR_MOVE_WAIT);
      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (m_pFm->IsAxisDone())
          break;

        if (millis()-pre_ms >= CN_PROCESS_WAIT_TIMEOUT)
        {
          break;
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAutoManager::state_e::axis_move_timeout);
        }
        delay(1);
      }
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_MOTOR_MOVE_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_MOTOR_MOVE_TIMEOUT)
      {
        m_retryCnt = 0;
        m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_PROCESS_STEP_MOTOR_MOVE_END);
        m_pre_time = millis();
      }
      
    }
    break;
    case CN_PROCESS_STEP_MOTOR_MOVE_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;

    /*######################################################
         operating phonejig open
      ######################################################*/
    case CN_PROCESS_STEP_PHONE_JIG_OPEN:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_PROCESS_STEP_PHONE_JIG_OPEN_START);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_PHONE_JIG_OPEN_START:
    {
      //
      m_pCyl[AP_DEF_OBJ_CYLINDER_ID_PHONE_JIG].Open(true);
      m_step.SetStep(CN_PROCESS_STEP_PHONE_JIG_OPEN_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_PHONE_JIG_OPEN_WAIT:
    {

      if (millis() - m_pre_time >= CN_PROCESS_WAIT_TIMEOUT)
      {
        if (m_retryCnt++ < CN_PROCESS_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_PROCESS_STEP_PHONE_JIG_OPEN_START);
          m_pre_time = millis();
        }
        else
        {
          m_retryCnt = 0;
          m_pAuto->AlarmAuto(cnAutoManager::state_e::cyl_0_open_timeout);
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          m_pre_time = millis();
        }
      }

      if (m_pCyl[AP_DEF_OBJ_CYLINDER_ID_PHONE_JIG].IsOpen()) // is close jig
      {
        m_step.SetStep(CN_PROCESS_STEP_PHONE_JIG_OPEN_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_PROCESS_STEP_PHONE_JIG_OPEN_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
         operating phonejig close
      ######################################################*/
    case CN_PROCESS_STEP_PHONE_JIG_CLOSE:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_PROCESS_STEP_PHONE_JIG_CLOSE_START);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_PHONE_JIG_CLOSE_START:
    {
      //
      m_pCyl[AP_DEF_OBJ_CYLINDER_ID_PHONE_JIG].Close(true);
      m_step.SetStep(CN_PROCESS_STEP_PHONE_JIG_CLOSE_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_PHONE_JIG_CLOSE_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_WAIT_TIMEOUT)
      {
        if (m_retryCnt++ < CN_PROCESS_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_PROCESS_STEP_PHONE_JIG_CLOSE_START);
          m_pre_time = millis();
        }
        else
        {
          m_retryCnt = 0;
          m_pAuto->AlarmAuto(cnAutoManager::state_e::cyl_0_close_timeout);
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          m_pre_time = millis();
        }
      }

      if (m_pCyl[AP_DEF_OBJ_CYLINDER_ID_PHONE_JIG].IsClose()) // is close jig
      {
        m_step.SetStep(CN_PROCESS_STEP_PHONE_JIG_CLOSE_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_PROCESS_STEP_PHONE_JIG_CLOSE_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
         operating get axis origin
      ######################################################*/
    case CN_PROCESS_STEP_POS_ORIGIN:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_PROCESS_STEP_POS_ORIGIN_START);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_ORIGIN_START:
    {
      //
      m_pFm->MoveOrigin();
      m_IsToDoState = false;
      m_step.SetStep(CN_PROCESS_STEP_POS_ORIGIN_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_ORIGIN_WAIT:
    {
      if (millis() - m_pre_time >= 1000*10)
      {
        m_retryCnt = 0;
        m_pAuto->AlarmAuto(cnAutoManager::state_e::axis_origin_err);
        m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
        m_pre_time = millis();
      }

      if (m_pFm->IsOriginOK())// is ok
      {
        m_step.SetStep(CN_PROCESS_STEP_POS_ORIGIN_END);
        m_pre_time = millis();
      } 
    }
    break;
    case CN_PROCESS_STEP_POS_ORIGIN_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
         operating axis move ready position
      ######################################################*/
    case CN_PROCESS_STEP_POS_READY:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_PROCESS_STEP_POS_READY_START);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_READY_START:
    {
      //
      axis_dat::dat_t pos_data = m_pApAxisDat->cmd_pos_dat[static_cast<uint8_t>(axis_dat::addr_e::ready_pos)];
      m_pFm->Move(pos_data.cmd_pos, pos_data.cmd_vel);

      m_step.SetStep(CN_PROCESS_STEP_POS_READY_WAIT);

      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (m_pFm->IsAxisDone())
          break;

        if (millis()-pre_ms >= CN_PROCESS_WAIT_TIMEOUT)
        {
          break;
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAutoManager::state_e::axis_move_timeout);
        }
        delay(1);
      }
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_READY_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_MOTOR_MOVE_TIMEOUT)
      {
        m_retryCnt = 0;
        m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_PROCESS_STEP_POS_READY_END);
        m_pre_time = millis();
      } 
    }
    break;
    case CN_PROCESS_STEP_POS_READY_END:
    {
      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
         operating axis move clean dust position
      ######################################################*/
    case CN_PROCESS_STEP_POS_CLEAN_DUST:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_PROCESS_STEP_POS_CLEAN_DUST_START);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_CLEAN_DUST_START:
    {
      //
      axis_dat::dat_t pos_data = m_pApAxisDat->cmd_pos_dat[static_cast<uint8_t>(axis_dat::addr_e::pos_clean_dust)];
      m_pFm->Move(pos_data.cmd_pos, pos_data.cmd_vel);

      m_step.SetStep(CN_PROCESS_STEP_POS_CLEAN_DUST_WAIT);

      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (m_pFm->IsAxisDone())
          break;

        if (millis()-pre_ms >= CN_PROCESS_WAIT_TIMEOUT)
        {
          break;
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAutoManager::state_e::axis_move_timeout);
        }
        delay(1);
      }
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_CLEAN_DUST_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_MOTOR_MOVE_TIMEOUT)
      {
        m_retryCnt = 0;
        m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_PROCESS_STEP_POS_CLEAN_DUST_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_PROCESS_STEP_POS_CLEAN_DUST_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
         operating axis move vacuum suction position
      ######################################################*/
    case CN_PROCESS_STEP_POS_SUCTION_VINYL:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_PROCESS_STEP_POS_SUCTION_VINYL_START);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_SUCTION_VINYL_START:
    {
      //
      axis_dat::dat_t pos_data = m_pApAxisDat->cmd_pos_dat[static_cast<uint8_t>(axis_dat::addr_e::pos_vinyl_suction)];
      m_pFm->Move(pos_data.cmd_pos, pos_data.cmd_vel);

      m_step.SetStep(CN_PROCESS_STEP_POS_SUCTION_VINYL_WAIT);

      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (m_pFm->IsAxisDone())
          break;

        if (millis()-pre_ms >= CN_PROCESS_WAIT_TIMEOUT)
        {
          break;
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAutoManager::state_e::axis_move_timeout);
        }
        delay(1);
      }
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_SUCTION_VINYL_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_MOTOR_MOVE_TIMEOUT)
      {
        m_retryCnt = 0;
        m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_PROCESS_STEP_POS_SUCTION_VINYL_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_PROCESS_STEP_POS_SUCTION_VINYL_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
         operating axis move peel vinyl off position
      ######################################################*/
    case CN_PROCESS_STEP_POS_PEEL_VINYL:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_PROCESS_STEP_POS_PEEL_VINYL_START);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_PEEL_VINYL_START:
    {
      //
      axis_dat::dat_t pos_data = m_pApAxisDat->cmd_pos_dat[static_cast<uint8_t>(axis_dat::addr_e::pos_vinyl_peel)];
      m_pFm->Move(pos_data.cmd_pos, pos_data.cmd_vel);

      m_step.SetStep(CN_PROCESS_STEP_POS_PEEL_VINYL_WAIT);

      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (m_pFm->IsAxisDone())
          break;

        if (millis()-pre_ms >= CN_PROCESS_WAIT_TIMEOUT)
        {
          break;
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAutoManager::state_e::axis_move_timeout);
        }
        delay(1);
      }
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_PEEL_VINYL_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_MOTOR_MOVE_TIMEOUT)
      {
        m_retryCnt = 0;
        m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_PROCESS_STEP_POS_PEEL_VINYL_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_PROCESS_STEP_POS_PEEL_VINYL_END:
    {
      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
         operating axis move peel complete position
      ######################################################*/
    case CN_PROCESS_STEP_POS_PEEL_VINYL_CPLT:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_PROCESS_STEP_POS_PEEL_VINYL_CPLT_START);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_PEEL_VINYL_CPLT_START:
    {
      //
      axis_dat::dat_t pos_data = m_pApAxisDat->cmd_pos_dat[static_cast<uint8_t>(axis_dat::addr_e::pos_peel_cplt)];
      m_pFm->Move(pos_data.cmd_pos, pos_data.cmd_vel);

      m_step.SetStep(CN_PROCESS_STEP_POS_PEEL_VINYL_CPLT_WAIT);

      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (m_pFm->IsAxisDone())
          break;

        if (millis()-pre_ms >= CN_PROCESS_WAIT_TIMEOUT)
        {
          break;
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAutoManager::state_e::axis_move_timeout);
        }
        delay(1);
      }

      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_PEEL_VINYL_CPLT_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_MOTOR_MOVE_TIMEOUT)
      {
        m_retryCnt = 0;
        m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_PROCESS_STEP_POS_PEEL_VINYL_CPLT_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_PROCESS_STEP_POS_PEEL_VINYL_CPLT_END:
    {
      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
       operating axis move reattach nivyl position
      ######################################################*/
    case CN_PROCESS_STEP_POS_REATTACH:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_PROCESS_STEP_POS_REATTACH_START);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_REATTACH_START:
    {
      //
      axis_dat::dat_t pos_data = m_pApAxisDat->cmd_pos_dat[static_cast<uint8_t>(axis_dat::addr_e::pos_vinyl_reattach)];
      m_pFm->Move(pos_data.cmd_pos, pos_data.cmd_vel);

      m_step.SetStep(CN_PROCESS_STEP_POS_REATTACH_WAIT);

      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (m_pFm->IsAxisDone())
          break;

        if (millis()-pre_ms >= CN_PROCESS_WAIT_TIMEOUT)
        {
          break;
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAutoManager::state_e::axis_move_timeout);
        }
        delay(1);
      }
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_REATTACH_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_MOTOR_MOVE_TIMEOUT)
      {
        m_retryCnt = 0;
        m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_PROCESS_STEP_POS_REATTACH_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_PROCESS_STEP_POS_REATTACH_END:
    {
      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
       operating axis move reattach position
      ######################################################*/
    case CN_PROCESS_STEP_POS_REATTACH_RUN:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_PROCESS_STEP_POS_REATTACH_RUN_START);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_REATTACH_RUN_START:
    {
      //
      axis_dat::dat_t pos_data = m_pApAxisDat->cmd_pos_dat[static_cast<uint8_t>(axis_dat::addr_e::pos_5)];
      m_pFm->Move(pos_data.cmd_pos, pos_data.cmd_vel);

      m_step.SetStep(CN_PROCESS_STEP_POS_REATTACH_RUN_WAIT);

      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (m_pFm->IsAxisDone())
          break;

        if (millis()-pre_ms >= CN_PROCESS_WAIT_TIMEOUT)
        {
          break;
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAutoManager::state_e::axis_move_timeout);
        }
        delay(1);
      }
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_REATTACH_RUN_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_MOTOR_MOVE_TIMEOUT)
      {
        m_retryCnt = 0;
        m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_PROCESS_STEP_POS_REATTACH_RUN_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_PROCESS_STEP_POS_REATTACH_RUN_END:
    {
      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
       operating peel drum origin
      ######################################################*/
    case CN_PROCESS_STEP_ROLLER_ORIGIN:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_PROCESS_STEP_ROLLER_ORIGIN_START);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_ROLLER_ORIGIN_START:
    {
      //
      m_IsToDoState = false;
      m_step.SetStep(CN_PROCESS_STEP_ROLLER_ORIGIN_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_ROLLER_ORIGIN_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_WAIT_TIMEOUT)
      {
        if (m_retryCnt++ < CN_PROCESS_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_PROCESS_STEP_ROLLER_ORIGIN_START);
          m_pre_time = millis();
        }
        else
        {
          m_retryCnt = 0;
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          m_pre_time = millis();
        }
      }

      if (1) // is ok
      {
        m_step.SetStep(CN_PROCESS_STEP_ROLLER_ORIGIN_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_PROCESS_STEP_ROLLER_ORIGIN_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
       data clear
      ######################################################*/
    case CN_PROCESS_STEP_DATA_CLEAR:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_PROCESS_STEP_DATA_CLEAR_START);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_DATA_CLEAR_START:
    {
      m_pApAxisDat->ClearRomData();
      m_pApCfgDat->ClearRomData();

      m_pApCylDat->ClearRomData();
      m_pApVacDat->ClearRomData();
      m_pApSeqDat->ClearRomData();

      m_IsToDoState = false;
      m_step.SetStep(CN_PROCESS_STEP_DATA_CLEAR_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_DATA_CLEAR_WAIT:
    {
      if(millis()-m_pre_time < 10)
        break;

      /* delay 10ms */
      m_step.SetStep(CN_PROCESS_STEP_DATA_CLEAR_END);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_DATA_CLEAR_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
      m_step.SetStep(CN_PROCESS_STEP_TODO);
      m_pre_time = millis();
    }
    break;
  
  /*######################################################
    CN_PROCESS_STEP_WRITE_MOTOR_DATA
    ######################################################*/
  case CN_PROCESS_STEP_WRITE_MOTOR_DATA:
  {
    m_retryCnt = 0;
    m_step.SetStep(CN_PROCESS_STEP_WRITE_MOTOR_DATA_START);
    m_pre_time = millis();
  }
  break;
  case CN_PROCESS_STEP_WRITE_MOTOR_DATA_START:
  {
    axis_dat::dat_t data;
    data.cmd_pos = utilDwToInt(&m_stepBuffer[4]);
    data.cmd_vel = utilDwToUint(&m_stepBuffer[8]);
    axis_dat::addr_e addr = static_cast<axis_dat::addr_e>(utilDwToUint(&m_stepBuffer[0]));
    m_pApAxisDat->WriteData(addr, data);

    m_step.SetStep(CN_PROCESS_STEP_WRITE_MOTOR_DATA_WAIT);
    m_pre_time = millis();
  }
  break;
  case CN_PROCESS_STEP_WRITE_MOTOR_DATA_WAIT:
  {
    if (millis() - m_pre_time < 20)
      break;

    m_step.SetStep(CN_PROCESS_STEP_WRITE_MOTOR_DATA_END);
    m_pre_time = millis();
  }
  break;
  case CN_PROCESS_STEP_WRITE_MOTOR_DATA_END:
  {
    //if (millis() - m_pre_time < 5)
    //  break;

    m_step.SetStep(CN_PROCESS_STEP_TODO);
    m_pre_time = millis();
  }
  break;

  /*######################################################
    CN_PROCESS_STEP_WRITE_AP_CFG
    ######################################################*/
  case CN_PROCESS_STEP_WRITE_AP_CFG:
  {
    m_retryCnt = 0;
    /* # buffer data struct
     *   m_stepBuffer[0] ~ m_stepBuffer[data length] + assign data...
     *
     * # data assign
     *   m_stepBuffer[0] = data length
     *   m_stepBuffer[m_stepBuffer[0]+2] = data max count
     *   m_stepBuffer[m_stepBuffer[0]+4] = increase number
     *   m_stepBuffer[m_stepBuffer[0]+3] = offset
     */
    CN_PROCESS_BUFF_ASSIGN_MAX_CNT =  CN_PROCESS_BUFF_ASSIGN_DATA_LENGTH / sizeof(ap_dat::dat_t) ;
    CN_PROCESS_BUFF_ASSIGN_OFFSET = 1;
    CN_PROCESS_BUFF_ASSIGN_INCREASE_NUM = 0;
    m_step.SetStep(CN_PROCESS_STEP_WRITE_AP_CFG_START);
    m_pre_time = millis();
  }
  break;
  case CN_PROCESS_STEP_WRITE_AP_CFG_START:
  {
    uint8_t idx = CN_PROCESS_BUFF_ASSIGN_OFFSET + (sizeof(ap_dat::dat_t)* CN_PROCESS_BUFF_ASSIGN_INCREASE_NUM) ;
    ap_dat::dat_t data;
    data.parm1 = m_stepBuffer[idx + 0] | (m_stepBuffer[idx + 1] << 8);
    data.parm2 = m_stepBuffer[idx + 2] | (m_stepBuffer[idx + 3] << 8);
    ap_dat::addr_e addr = static_cast<ap_dat::addr_e>(CN_PROCESS_BUFF_ASSIGN_INCREASE_NUM++);
    m_pApCfgDat->WriteData(addr, data);

    m_step.SetStep(CN_PROCESS_STEP_WRITE_AP_CFG_WAIT);
    m_pre_time = millis();
  }
  break;
  case CN_PROCESS_STEP_WRITE_AP_CFG_WAIT:
  {
    if (millis() - m_pre_time < 10)
      break;

    if (CN_PROCESS_BUFF_ASSIGN_INCREASE_NUM < CN_PROCESS_BUFF_ASSIGN_MAX_CNT)
    {
      m_step.SetStep(CN_PROCESS_STEP_WRITE_AP_CFG_START);
    }
    else
    {
      m_step.SetStep(CN_PROCESS_STEP_WRITE_AP_CFG_END);
    }
    m_pre_time = millis();
  }
  break;
  case CN_PROCESS_STEP_WRITE_AP_CFG_END:
  {
    m_step.SetStep(CN_PROCESS_STEP_TODO);
    m_pre_time = millis();
  }
  break;

  /*######################################################
    CN_PROCESS_STEP_WRITE_CYL_DATA
    ######################################################*/
  case CN_PROCESS_STEP_WRITE_CYL_DATA:
  {
    m_retryCnt = 0;

    CN_PROCESS_BUFF_ASSIGN_MAX_CNT = CN_PROCESS_BUFF_ASSIGN_DATA_LENGTH / sizeof(cyl_dat::dat_t);
    CN_PROCESS_BUFF_ASSIGN_OFFSET = 1;
    CN_PROCESS_BUFF_ASSIGN_INCREASE_NUM = 0;
    m_step.SetStep(CN_PROCESS_STEP_WRITE_CYL_DATA_START);
    m_pre_time = millis();
  }
  break;
  case CN_PROCESS_STEP_WRITE_CYL_DATA_START:
  {
    uint8_t idx = CN_PROCESS_BUFF_ASSIGN_OFFSET + (sizeof(cyl_dat::dat_t)* CN_PROCESS_BUFF_ASSIGN_INCREASE_NUM) ;
    cyl_dat::dat_t data;
    data.timeout = m_stepBuffer[idx + 0] | (m_stepBuffer[idx + 1] << 8);
    data.on_settling = m_stepBuffer[idx + 2] | (m_stepBuffer[idx + 3] << 8);
    cyl_dat::addr_e addr = static_cast<cyl_dat::addr_e>(CN_PROCESS_BUFF_ASSIGN_INCREASE_NUM++);
    m_pApCylDat->WriteData(addr, data);

    m_step.SetStep(CN_PROCESS_STEP_WRITE_CYL_DATA_WAIT);
    m_pre_time = millis();
  }
  break;
  case CN_PROCESS_STEP_WRITE_CYL_DATA_WAIT:
  {
    if (millis() - m_pre_time < 10)
      break;

    if (CN_PROCESS_BUFF_ASSIGN_INCREASE_NUM < CN_PROCESS_BUFF_ASSIGN_MAX_CNT)
    {
      m_step.SetStep(CN_PROCESS_STEP_WRITE_CYL_DATA_START);
    }
    else
    {
      m_step.SetStep(CN_PROCESS_STEP_WRITE_CYL_DATA_END);
    }
    m_pre_time = millis();
  }
  break;
  case CN_PROCESS_STEP_WRITE_CYL_DATA_END:
  {
    m_step.SetStep(CN_PROCESS_STEP_TODO);
    m_pre_time = millis();
  }
  break;

  /*######################################################
    CN_PROCESS_STEP_WRITE_VAC_DATA
    ######################################################*/
  case CN_PROCESS_STEP_WRITE_VAC_DATA:
  {
    m_retryCnt = 0;
    CN_PROCESS_BUFF_ASSIGN_MAX_CNT =  CN_PROCESS_BUFF_ASSIGN_DATA_LENGTH / sizeof(vac_dat::dat_t) ;
    CN_PROCESS_BUFF_ASSIGN_OFFSET = 1;
    CN_PROCESS_BUFF_ASSIGN_INCREASE_NUM = 0;
    m_step.SetStep(CN_PROCESS_STEP_WRITE_VAC_DATA_START);
    m_pre_time = millis();
  }
  break;
  case CN_PROCESS_STEP_WRITE_VAC_DATA_START:
  {
    uint8_t idx = CN_PROCESS_BUFF_ASSIGN_OFFSET + (sizeof(vac_dat::dat_t)* CN_PROCESS_BUFF_ASSIGN_INCREASE_NUM) ;
    vac_dat::dat_t data;
    data.timeout = m_stepBuffer[idx + 0] | (m_stepBuffer[idx + 1] << 8);
    data.on_settling = m_stepBuffer[idx + 2] | (m_stepBuffer[idx + 3] << 8);
    vac_dat::addr_e addr = static_cast<vac_dat::addr_e>(CN_PROCESS_BUFF_ASSIGN_INCREASE_NUM++);
    m_pApVacDat->WriteData(addr, data);

    m_step.SetStep(CN_PROCESS_STEP_WRITE_VAC_DATA_WAIT);
    m_pre_time = millis();
  }
  break;
  case CN_PROCESS_STEP_WRITE_VAC_DATA_WAIT:
  {
    if (millis() - m_pre_time < 10)
      break;

    if (CN_PROCESS_BUFF_ASSIGN_INCREASE_NUM < CN_PROCESS_BUFF_ASSIGN_MAX_CNT)
    {
      m_step.SetStep(CN_PROCESS_STEP_WRITE_VAC_DATA_START);
    }
    else
    {
      m_step.SetStep(CN_PROCESS_STEP_WRITE_VAC_DATA_END);
    }
    m_pre_time = millis();
  }
  break;
  case CN_PROCESS_STEP_WRITE_VAC_DATA_END:
  {
    m_step.SetStep(CN_PROCESS_STEP_TODO);
    m_pre_time = millis();
  }
  break;
  /*######################################################
    CN_PROCESS_STEP_WRITE_SEQ_DATA
    ######################################################*/
  case CN_PROCESS_STEP_WRITE_SEQ_DATA:
  {
    m_retryCnt = 0;
    CN_PROCESS_BUFF_ASSIGN_MAX_CNT = CN_PROCESS_BUFF_ASSIGN_DATA_LENGTH / sizeof(seq_dat::dat_t);
    CN_PROCESS_BUFF_ASSIGN_OFFSET = 1;
    CN_PROCESS_BUFF_ASSIGN_INCREASE_NUM = 0;
    m_step.SetStep(CN_PROCESS_STEP_WRITE_SEQ_DATA_START);
    m_pre_time = millis();
  }
  break;
  case CN_PROCESS_STEP_WRITE_SEQ_DATA_START:
  {
    uint8_t idx = CN_PROCESS_BUFF_ASSIGN_OFFSET + (sizeof(seq_dat::dat_t)* CN_PROCESS_BUFF_ASSIGN_INCREASE_NUM) ;
    seq_dat::dat_t data;
    data.parm1 = m_stepBuffer[idx + 0] | (m_stepBuffer[idx + 1] << 8);
    data.parm2 = m_stepBuffer[idx + 2] | (m_stepBuffer[idx + 3] << 8);
    seq_dat::addr_e addr = static_cast<seq_dat::addr_e>(CN_PROCESS_BUFF_ASSIGN_INCREASE_NUM++);
    m_pApSeqDat->WriteData(addr, data);

    m_step.SetStep(CN_PROCESS_STEP_WRITE_SEQ_DATA_WAIT);
    m_pre_time = millis();
  }
  break;
  case CN_PROCESS_STEP_WRITE_SEQ_DATA_WAIT:
  {
    if (millis() - m_pre_time < 10)
      break;

    if (CN_PROCESS_BUFF_ASSIGN_INCREASE_NUM < CN_PROCESS_BUFF_ASSIGN_MAX_CNT)
    {
      m_step.SetStep(CN_PROCESS_STEP_WRITE_SEQ_DATA_START);
    }
    else
    {
      m_step.SetStep(CN_PROCESS_STEP_WRITE_SEQ_DATA_END);
    }    
    m_pre_time = millis();
  }
  break;
  case CN_PROCESS_STEP_WRITE_SEQ_DATA_END:
  {
    m_step.SetStep(CN_PROCESS_STEP_TODO);
    m_pre_time = millis();
  }
  break;
  /*######################################################
       timeout
    ######################################################*/
  case CN_PROCESS_STEP_TIMEOUT:
  {
    if (millis() - m_pre_time < 5)
      break;
    m_retryCnt = 0;
    m_step.SetStep(CN_PROCESS_STEP_TODO);
    m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_END:
    {

    }
    break;
    default:
      break;
  }//switch
}

void cnProcess::ThreadJob()
{

  switch(m_pAuto->GetOPMode())
  {
    case OP_MODE_STOP:
      m_IsAutoReady = false;
      if (millis()-m_pre_time >= 1000*1)
      {
        m_step.SetStep(CN_PROCESS_STEP_TODO);
        m_IsToDoState = true;
      }
        doOpJob();
      break;

    case OP_MODE_DRY_RUN:
      break;
    case OP_MODE_AUTORUN:
    case OP_MODE_READY:
    {
      switch(m_pAuto->GetOPStatus())
      {
        case OP_ERROR_STOP:
          m_IsAutoReady = false;
          break;

        case OP_START_RUN:
          m_IsAutoReady = true;
          break;

        case OP_RUN:
          doRunStep();
          break;

        case OP_STEP_STOP:
        default:
        {
        }
        break;
      }//switch(m_pAuto->GetOPStatus())
    }
    break;
  }//switch(m_pAuto->GetOPMode())


}


bool cnProcess::IsCylOpen(uint8_t obj_id)
{
  return m_pCyl[obj_id].IsOpen();
}

bool cnProcess::IsCylClose(uint8_t obj_id)
{
  return m_pCyl[obj_id].IsClose();
}

int cnProcess::CylOpen(uint8_t obj_id, bool skip_sensor )
{
  int ret = 0;
  ret = m_pCyl[obj_id].Open(skip_sensor);
  if (ret != 0)
  {
    uint8_t err = static_cast<uint8_t>(cnAutoManager::state_e::cyl_0_open_timeout)+obj_id;
    m_pAuto->AlarmAuto(static_cast<cnAutoManager::state_e>(err));
  }
  return ret;
}
int cnProcess::CylClose(uint8_t obj_id, bool skip_sensor )
{
  int ret = 0;
  ret = m_pCyl[obj_id].Close(skip_sensor);
  if (ret != 0)
  {
    uint8_t err = static_cast<uint8_t>(cnAutoManager::state_e::cyl_0_close_timeout)+obj_id;
    m_pAuto->AlarmAuto(static_cast<cnAutoManager::state_e>(err));
  }
  return ret;
}

bool cnProcess::IsVacOn(uint8_t obj_id)
{
  return m_pVac[obj_id].IsOn();
}

bool cnProcess::IsVacOff(uint8_t obj_id)
{
  return m_pVac[obj_id].IsOff();
}

int cnProcess::VacOn(uint8_t obj_id, bool skip_sensor )
{
  int ret = 0;
  ret = m_pVac[obj_id].On(skip_sensor);
  if (ret != 0)
  {
    uint8_t err = static_cast<uint8_t>(cnAutoManager::state_e::vac_0_on_timeout)+obj_id;
    m_pAuto->AlarmAuto(static_cast<cnAutoManager::state_e>(err));
  }
  return ret;
}
int cnProcess::VacOff(uint8_t obj_id, bool skip_sensor )
{
  int ret = 0;
  ret = m_pVac[obj_id].Off(skip_sensor);
  if (ret != 0)
  {
    uint8_t err = static_cast<uint8_t>(cnAutoManager::state_e::vac_0_off_timeout)+obj_id;
    m_pAuto->AlarmAuto(static_cast<cnAutoManager::state_e>(err));
  }
  return ret;
}


bool cnProcess::IsMotorOn()
{
  return m_pFm->IsMotorOn();
}

bool cnProcess::IsMotorRun()
{
  return m_pFm->IsBusy();
}

bool cnProcess::IsMotorMoveCplt()
{
  return m_pFm->IsAxisDone();
}

void cnProcess::MotorOrigin()
{
  if (!IsMotorOn())
    return ;

  m_step.SetStep(CN_PROCESS_STEP_POS_ORIGIN);
  m_pre_time = millis();
}

void cnProcess::MotorOnOff(bool on_off)
{
  if (!m_IsToDoState)
    return;

  if (on_off)
    m_step.SetStep(CN_PROCESS_STEP_MOTOR_ON);
  else
    m_step.SetStep(CN_PROCESS_STEP_MOTOR_OFF);
  m_pre_time = millis();
}


void cnProcess::MotorStop()
{
  m_step.SetStep(CN_PROCESS_STEP_MOTOR_STOP);
  m_pre_time = millis();
}


void cnProcess::MotorRun(int cmd_pos, uint32_t cmd_vel, uint32_t acc, uint32_t decel)
{
  m_stepBuffer[0] = (uint8_t)(cmd_pos >> 0);
  m_stepBuffer[1] = (uint8_t)(cmd_pos >> 8);
  m_stepBuffer[2] = (uint8_t)(cmd_pos >> 16);
  m_stepBuffer[3] = (uint8_t)(cmd_pos >> 24);

  m_stepBuffer[4] = (uint8_t)(cmd_vel >> 0);
  m_stepBuffer[5] = (uint8_t)(cmd_vel >> 8);
  m_stepBuffer[6] = (uint8_t)(cmd_vel >> 16);
  m_stepBuffer[7] = (uint8_t)(cmd_vel >> 24);

  m_stepBuffer[8] = (uint8_t)(acc >> 0);
  m_stepBuffer[9] = (uint8_t)(acc >> 8);
  m_stepBuffer[10] = (uint8_t)(acc >> 16);
  m_stepBuffer[11] = (uint8_t)(acc >> 24);

  m_stepBuffer[12] = (uint8_t)(decel >> 0);
  m_stepBuffer[13] = (uint8_t)(decel >> 8);
  m_stepBuffer[14] = (uint8_t)(decel >> 16);
  m_stepBuffer[15] = (uint8_t)(decel >> 24);

  m_pre_time = millis();
  m_step.SetStep(CN_PROCESS_STEP_MOTOR_MOVE);
}

void cnProcess::WriteMotorData(uint8_t *p_data, uint8_t length)
{
  memcpy(&m_stepBuffer[0], p_data, length);
  if(m_step.GetStep() == CN_PROCESS_STEP_TODO)
  {
    m_step.SetStep(CN_PROCESS_STEP_WRITE_MOTOR_DATA);
    m_pre_time = millis();
  }
}
void cnProcess::WriteApCfgData(uint8_t *p_data, uint8_t length)
{
  memcpy(&m_stepBuffer[0], p_data, length);
  if(m_step.GetStep() == CN_PROCESS_STEP_TODO)
  {
    m_step.SetStep(CN_PROCESS_STEP_WRITE_AP_CFG);
    m_pre_time = millis();
  }
}
void cnProcess::WriteCylData(uint8_t *p_data, uint8_t length)
{
  memcpy(&m_stepBuffer[0], p_data, length);
  if(m_step.GetStep() == CN_PROCESS_STEP_TODO)
  {
    m_step.SetStep(CN_PROCESS_STEP_WRITE_CYL_DATA);
    m_pre_time = millis();
  }
}
void cnProcess::WriteVacData(uint8_t *p_data, uint8_t length)
{
  memcpy(&m_stepBuffer[0], p_data, length);
  if(m_step.GetStep() == CN_PROCESS_STEP_TODO)
  {
    m_step.SetStep(CN_PROCESS_STEP_WRITE_VAC_DATA);
    m_pre_time = millis();
  }
}

void cnProcess::WriteSeqData(uint8_t *p_data, uint8_t length)
{
  memcpy(&m_stepBuffer[0], p_data, length);
  if(m_step.GetStep() == CN_PROCESS_STEP_TODO)
  {
    m_step.SetStep(CN_PROCESS_STEP_WRITE_SEQ_DATA);
    m_pre_time = millis();
  }
}
