/*
 * cnProcess.cpp
 *
 *  Created on: 2022. 3. 1.
 *      Author: gns2l
 */
#include "ap_def.h"
#include <control/cnProcess.h>

#define CN_PROCESS_STEP_RETRY_CNT_MAX                 3
#define CN_PROCESS_TIMEOUT                            100

cnProcess::cnProcess ():m_pApReg(NULL)
{
  m_pApIo = NULL;
  m_pFm = NULL;
  m_pApAxisDat = NULL;
  m_pApCylDat = NULL;
  m_pApVacDat = NULL;
  m_pApCfgDat = NULL;
  m_pApSeqDat = NULL;
  m_pAuto = NULL;
  m_IsBusy = false;
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
      m_IsBusy = true;
      m_step.SetStep(CN_PROCESS_STEP_PHONE_JIG_OPEN_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_PHONE_JIG_OPEN_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_TIMEOUT)
      {
        if (m_retryCnt++ < CN_PROCESS_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_PROCESS_STEP_PHONE_JIG_OPEN_START);
          m_pre_time = millis();
        }
        else
        {
          m_retryCnt = 0;
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          m_pre_time = millis();
        }
      }

      if (1)// is open jig
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
      m_IsBusy = true;
      m_step.SetStep(CN_PROCESS_STEP_PHONE_JIG_CLOSE_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_PHONE_JIG_CLOSE_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_TIMEOUT)
      {
        if (m_retryCnt++ < CN_PROCESS_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_PROCESS_STEP_PHONE_JIG_CLOSE_START);
          m_pre_time = millis();
        }
        else
        {
          m_retryCnt = 0;
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          m_pre_time = millis();
        }
      }

      if (1) // is close jig
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
      m_IsBusy = true;
      m_step.SetStep(CN_PROCESS_STEP_POS_ORIGIN_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_ORIGIN_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_TIMEOUT)
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
      m_IsBusy = true;
      m_step.SetStep(CN_PROCESS_STEP_POS_READY_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_READY_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_TIMEOUT)
      {
        if (m_retryCnt++ < CN_PROCESS_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_PROCESS_STEP_POS_READY_START);
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
      m_IsBusy = true;
      m_step.SetStep(CN_PROCESS_STEP_POS_CLEAN_DUST_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_CLEAN_DUST_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_TIMEOUT)
      {
        if (m_retryCnt++ < CN_PROCESS_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_PROCESS_STEP_POS_CLEAN_DUST_START);
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
      m_IsBusy = true;
      m_step.SetStep(CN_PROCESS_STEP_POS_SUCTION_VINYL_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_SUCTION_VINYL_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_TIMEOUT)
      {
        if (m_retryCnt++ < CN_PROCESS_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_PROCESS_STEP_POS_SUCTION_VINYL_START);
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
      //
      m_IsBusy = true;
      m_step.SetStep(CN_PROCESS_STEP_POS_PEEL_VINYL_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_PEEL_VINYL_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_TIMEOUT)
      {
        if (m_retryCnt++ < CN_PROCESS_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_PROCESS_STEP_POS_PEEL_VINYL_START);
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
        m_step.SetStep(CN_PROCESS_STEP_POS_PEEL_VINYL_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_PROCESS_STEP_POS_PEEL_VINYL_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
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
      m_IsBusy = true;
      m_step.SetStep(CN_PROCESS_STEP_POS_PEEL_VINYL_CPLT_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_PEEL_VINYL_CPLT_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_TIMEOUT)
      {
        if (m_retryCnt++ < CN_PROCESS_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_PROCESS_STEP_POS_PEEL_VINYL_CPLT_START);
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
        m_step.SetStep(CN_PROCESS_STEP_POS_PEEL_VINYL_CPLT_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_PROCESS_STEP_POS_PEEL_VINYL_CPLT_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
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
      m_IsBusy = true;
      m_step.SetStep(CN_PROCESS_STEP_POS_REATTACH_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_REATTACH_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_TIMEOUT)
      {
        if (m_retryCnt++ < CN_PROCESS_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_PROCESS_STEP_POS_REATTACH_START);
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
      m_IsBusy = true;
      m_step.SetStep(CN_PROCESS_STEP_POS_REATTACH_RUN_WAIT);      
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_REATTACH_RUN_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_TIMEOUT)
      {
        if (m_retryCnt++ < CN_PROCESS_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_PROCESS_STEP_POS_REATTACH_RUN_START);
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
        m_step.SetStep(CN_PROCESS_STEP_POS_REATTACH_RUN_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_PROCESS_STEP_POS_REATTACH_RUN_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
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
      m_IsBusy = true;
      m_step.SetStep(CN_PROCESS_STEP_ROLLER_ORIGIN_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_ROLLER_ORIGIN_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_TIMEOUT)
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
  // cylinder
  // motor
  // state
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
      m_pFm->MotorOnOff(true);
      m_IsBusy = true;
      m_step.SetStep(CN_PROCESS_STEP_MOTOR_ON_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_MOTOR_ON_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_TIMEOUT)
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
      m_pFm->MotorOnOff(false);
      m_IsBusy = true;
      m_step.SetStep(CN_PROCESS_STEP_MOTOR_OFF_WAIT);      
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_MOTOR_OFF_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_TIMEOUT)
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
      m_pFm->Stop();
      m_IsBusy = true;
      m_step.SetStep(CN_PROCESS_STEP_MOTOR_STOP_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_MOTOR_STOP_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_TIMEOUT)
      {
        if (m_retryCnt++ < CN_PROCESS_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_PROCESS_STEP_MOTOR_STOP_START);
          m_pre_time = millis();
        }
        else
        {
          m_retryCnt = 0;
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          m_pre_time = millis();
        }
      }

      if (!m_pFm->IsBusy())
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
      int cmd_pos = 0;
      cmd_pos = utilDwToInt(&m_stepBuffer[0]);
      uint32_t cmd_vel = 0;
      cmd_vel = utilDwToUint(&m_stepBuffer[4]);
      uint32_t acc = 0;
      acc = utilDwToUint(&m_stepBuffer[8]);
      uint32_t decel = 0;
      decel = utilDwToUint(&m_stepBuffer[12]);

      m_pFm->Move(cmd_pos, cmd_vel, acc, decel);

      //m_pFm->Stop();
      m_IsBusy = true;
      m_step.SetStep(CN_PROCESS_STEP_MOTOR_MOVE_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_MOTOR_MOVE_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_TIMEOUT)
      {
        if (m_retryCnt++ < CN_PROCESS_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_PROCESS_STEP_MOTOR_MOVE_START);
          m_pre_time = millis();
        }
        else
        {
          m_retryCnt = 0;
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          m_pre_time = millis();
        }
      }

      if (!m_pFm->IsBusy())
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
      m_IsBusy = true;
      m_step.SetStep(CN_PROCESS_STEP_PHONE_JIG_OPEN_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_PHONE_JIG_OPEN_WAIT:
    {

      if (millis() - m_pre_time >= CN_PROCESS_TIMEOUT)
      {
        if (m_retryCnt++ < CN_PROCESS_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_PROCESS_STEP_PHONE_JIG_OPEN_START);
          m_pre_time = millis();
        }
        else
        {
          m_retryCnt = 0;
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          m_pre_time = millis();
        }
      }

      if (1) // is open jig
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
      m_IsBusy = true;
      m_step.SetStep(CN_PROCESS_STEP_PHONE_JIG_CLOSE_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_PHONE_JIG_CLOSE_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_TIMEOUT)
      {
        if (m_retryCnt++ < CN_PROCESS_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_PROCESS_STEP_PHONE_JIG_CLOSE_START);
          m_pre_time = millis();
        }
        else
        {
          m_retryCnt = 0;
          m_step.SetStep(CN_PROCESS_STEP_TIMEOUT);
          m_pre_time = millis();
        }
      }

      if (1) // is close jig
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
      m_IsBusy = true;
      m_step.SetStep(CN_PROCESS_STEP_POS_ORIGIN_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_ORIGIN_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_TIMEOUT)
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
      m_IsBusy = true;
      m_step.SetStep(CN_PROCESS_STEP_POS_READY_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_READY_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_TIMEOUT)
      {
        if (m_retryCnt++ < CN_PROCESS_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_PROCESS_STEP_POS_READY_START);
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
      m_IsBusy = true;
      m_step.SetStep(CN_PROCESS_STEP_POS_CLEAN_DUST_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_CLEAN_DUST_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_TIMEOUT)
      {
        if (m_retryCnt++ < CN_PROCESS_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_PROCESS_STEP_POS_CLEAN_DUST_START);
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
      m_IsBusy = true;
      m_step.SetStep(CN_PROCESS_STEP_POS_SUCTION_VINYL_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_SUCTION_VINYL_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_TIMEOUT)
      {
        if (m_retryCnt++ < CN_PROCESS_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_PROCESS_STEP_POS_SUCTION_VINYL_START);
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
      m_IsBusy = true;
      m_step.SetStep(CN_PROCESS_STEP_POS_PEEL_VINYL_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_PEEL_VINYL_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_TIMEOUT)
      {
        if (m_retryCnt++ < CN_PROCESS_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_PROCESS_STEP_POS_PEEL_VINYL_START);
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
        m_step.SetStep(CN_PROCESS_STEP_POS_PEEL_VINYL_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_PROCESS_STEP_POS_PEEL_VINYL_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
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
      m_IsBusy = true;
      m_step.SetStep(CN_PROCESS_STEP_POS_PEEL_VINYL_CPLT_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_PEEL_VINYL_CPLT_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_TIMEOUT)
      {
        if (m_retryCnt++ < CN_PROCESS_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_PROCESS_STEP_POS_PEEL_VINYL_CPLT_START);
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
        m_step.SetStep(CN_PROCESS_STEP_POS_PEEL_VINYL_CPLT_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_PROCESS_STEP_POS_PEEL_VINYL_CPLT_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
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
      m_IsBusy = true;
      m_step.SetStep(CN_PROCESS_STEP_POS_REATTACH_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_REATTACH_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_TIMEOUT)
      {
        if (m_retryCnt++ < CN_PROCESS_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_PROCESS_STEP_POS_REATTACH_START);
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
      m_IsBusy = true;
      m_step.SetStep(CN_PROCESS_STEP_POS_REATTACH_RUN_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_POS_REATTACH_RUN_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_TIMEOUT)
      {
        if (m_retryCnt++ < CN_PROCESS_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_PROCESS_STEP_POS_REATTACH_RUN_START);
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
        m_step.SetStep(CN_PROCESS_STEP_POS_REATTACH_RUN_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_PROCESS_STEP_POS_REATTACH_RUN_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
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
      m_IsBusy = true;
      m_step.SetStep(CN_PROCESS_STEP_ROLLER_ORIGIN_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_PROCESS_STEP_ROLLER_ORIGIN_WAIT:
    {
      if (millis() - m_pre_time >= CN_PROCESS_TIMEOUT)
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

      m_IsBusy = true;
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
    axis_dat::dat_t send_data;
    send_data.cmd_pos = utilDwToInt(&m_stepBuffer[4 + 1]);
    send_data.cmd_vel = utilDwToUint(&m_stepBuffer[8 + 1]);
    axis_dat::addr_e addr = static_cast<axis_dat::addr_e>(utilDwToUint(&m_stepBuffer[0 + 1]));
    m_pApAxisDat->WriteData(addr, send_data);

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

    //m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_MOTOR_POS);
    m_pre_time = millis();
  }
  break;

  /*######################################################
    CN_PROCESS_STEP_WRITE_AP_CFG
    ######################################################*/
  case CN_PROCESS_STEP_WRITE_AP_CFG:
  {
    m_retryCnt = 0;
    m_stepBuffer[m_stepBuffer[0]+2] =  m_stepBuffer[0] / 4 ;
    m_stepBuffer[m_stepBuffer[0]+3] = 1;
    m_stepBuffer[m_stepBuffer[0]+4] = 0;
    m_step.SetStep(CN_PROCESS_STEP_WRITE_AP_CFG_START);
    m_pre_time = millis();
  }
  break;
  case CN_PROCESS_STEP_WRITE_AP_CFG_START:
  {
    uint8_t i = m_stepBuffer[m_stepBuffer[0] + 4];
    uint8_t idx = m_stepBuffer[m_stepBuffer[0] + 3];
    ap_dat::dat_t send_data;
    send_data.parm1 = m_stepBuffer[idx + 0] | (m_stepBuffer[idx + 1] << 8);
    send_data.parm2 = m_stepBuffer[idx + 2] | (m_stepBuffer[idx + 3] << 8);
    idx = idx + 4;
    ap_dat::addr_e addr = static_cast<ap_dat::addr_e>(i);
    m_pApCfgDat->WriteData(addr, send_data);

    m_stepBuffer[m_stepBuffer[0] + 3] += 4;
    m_stepBuffer[m_stepBuffer[0] + 4]++;

    m_step.SetStep(CN_PROCESS_STEP_WRITE_AP_CFG_WAIT);
    m_pre_time = millis();
  }
  break;
  case CN_PROCESS_STEP_WRITE_AP_CFG_WAIT:
  {
    if (millis() - m_pre_time < 10)
      break;

    if (m_stepBuffer[m_stepBuffer[0] + 4] < m_stepBuffer[0]/4)
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
    //if (millis() - m_pre_time < 5)
    //  break;

    //m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_AP_REG);
    m_pre_time = millis();
  }
  break;

  /*######################################################
    CN_PROCESS_STEP_WRITE_CYL_DATA
    ######################################################*/
  case CN_PROCESS_STEP_WRITE_CYL_DATA:
  {
    m_retryCnt = 0;

    m_stepBuffer[m_stepBuffer[0] + 2] = m_stepBuffer[0] / 4;
    m_stepBuffer[m_stepBuffer[0] + 3] = 1;
    m_stepBuffer[m_stepBuffer[0] + 4] = 0;
    m_step.SetStep(CN_PROCESS_STEP_WRITE_CYL_DATA_START);
    m_pre_time = millis();
  }
  break;
  case CN_PROCESS_STEP_WRITE_CYL_DATA_START:
  {
    uint8_t i = m_stepBuffer[m_stepBuffer[0] + 4];
    uint8_t idx = m_stepBuffer[m_stepBuffer[0] + 3];
    cyl_dat::dat_t send_data;
    send_data.timeout = m_stepBuffer[idx + 0] | (m_stepBuffer[idx + 1] << 8);
    send_data.on_settling = m_stepBuffer[idx + 2] | (m_stepBuffer[idx + 3] << 8);
    idx = idx + 4;
    cyl_dat::addr_e addr = static_cast<cyl_dat::addr_e>(i);
    m_pApCylDat->WriteData(addr, send_data);

    m_stepBuffer[m_stepBuffer[0] + 3] += 4;
    m_stepBuffer[m_stepBuffer[0] + 4]++;

    m_step.SetStep(CN_PROCESS_STEP_WRITE_CYL_DATA_WAIT);
    m_pre_time = millis();
  }
  break;
  case CN_PROCESS_STEP_WRITE_CYL_DATA_WAIT:
  {
    if (millis() - m_pre_time < 10)
      break;

    if (m_stepBuffer[m_stepBuffer[0] + 4] < m_stepBuffer[0]/4)
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
    // if (millis() - m_pre_time < 5)
    //   break;

    // m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_AP_REG);
    m_pre_time = millis();
  }
  break;

  /*######################################################
    CN_PROCESS_STEP_WRITE_VAC_DATA
    ######################################################*/
  case CN_PROCESS_STEP_WRITE_VAC_DATA:
  {
    m_retryCnt = 0;
    m_step.SetStep(CN_PROCESS_STEP_WRITE_VAC_DATA_START);
    m_pre_time = millis();
  }
  break;
  case CN_PROCESS_STEP_WRITE_VAC_DATA_START:
  {
    vac_dat::dat_t send_data;
    uint16_t length = m_stepBuffer[0];
    uint8_t idx = 1;
    for (uint8_t i = 0; i < (length / 4); i++)
    {
      send_data.timeout = m_stepBuffer[idx + 0] | (m_stepBuffer[idx + 1] << 8);
      send_data.on_settling = m_stepBuffer[idx + 2] | (m_stepBuffer[idx + 3] << 8);
      idx = idx + 4;
      vac_dat::addr_e addr = static_cast<vac_dat::addr_e>(i);
      m_pApVacDat->WriteData(addr, send_data);

      delay(5);
    }

    m_step.SetStep(CN_PROCESS_STEP_WRITE_VAC_DATA_WAIT);
    m_pre_time = millis();
  }
  break;
  case CN_PROCESS_STEP_WRITE_VAC_DATA_WAIT:
  {
    // if (millis() - m_pre_time < 20)
    //   break;

    m_step.SetStep(CN_PROCESS_STEP_WRITE_VAC_DATA_END);
    m_pre_time = millis();
  }
  break;
  case CN_PROCESS_STEP_WRITE_VAC_DATA_END:
  {
    // if (millis() - m_pre_time < 5)
    //   break;

    // m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_AP_REG);
    m_pre_time = millis();
  }
  break;
  /*######################################################
    CN_PROCESS_STEP_WRITE_SEQ_DATA
    ######################################################*/
  case CN_PROCESS_STEP_WRITE_SEQ_DATA:
  {
    m_retryCnt = 0;

    m_stepBuffer[m_stepBuffer[0]+2] =  m_stepBuffer[0] / 4 ;
    m_stepBuffer[m_stepBuffer[0]+3] = 1;
    m_stepBuffer[m_stepBuffer[0]+4] = 0;
    m_step.SetStep(CN_PROCESS_STEP_WRITE_SEQ_DATA_START);
    m_pre_time = millis();
  }
  break;
  case CN_PROCESS_STEP_WRITE_SEQ_DATA_START:
  {
    uint8_t i = m_stepBuffer[m_stepBuffer[0]+4];
    uint8_t idx = m_stepBuffer[m_stepBuffer[0] + 3];
    seq_dat::dat_t send_data;
    send_data.parm1 = m_stepBuffer[idx + 0] | (m_stepBuffer[idx + 1] << 8);
    send_data.parm2 = m_stepBuffer[idx + 2] | (m_stepBuffer[idx + 3] << 8);
    seq_dat::addr_e addr = static_cast<seq_dat::addr_e>(i);
    m_pApSeqDat->WriteData(addr, send_data);

    m_stepBuffer[m_stepBuffer[0] + 3] += 4;
    m_stepBuffer[m_stepBuffer[0] + 4]++;

    m_step.SetStep(CN_PROCESS_STEP_WRITE_SEQ_DATA_WAIT);
    m_pre_time = millis();
  }
  break;
  case CN_PROCESS_STEP_WRITE_SEQ_DATA_WAIT:
  {
    if (millis() - m_pre_time < 10)
      break;

    if (m_stepBuffer[m_stepBuffer[0] + 4] < m_stepBuffer[0]/4)
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
    // if (millis() - m_pre_time < 5)
    //   break;

    // m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_AP_REG);
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
        m_IsBusy = false;
      }
      if (m_IsBusy)
        return;

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



void cnProcess::MotorOnOff(bool on_off)
{
  if (m_IsBusy)
    return;

  m_pre_time = millis();
  if (on_off)
    m_step.SetStep(CN_PROCESS_STEP_MOTOR_ON);
  else
    m_step.SetStep(CN_PROCESS_STEP_MOTOR_OFF);

}


void cnProcess::MotorStop()
{
  if (m_IsBusy)
    return;

  m_pre_time = millis();
  m_step.SetStep(CN_PROCESS_STEP_MOTOR_STOP);
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
  if (m_step.GetStep() == CN_PROCESS_STEP_TODO)
  {
    m_pre_time = millis();
    m_step.SetStep(CN_PROCESS_STEP_WRITE_MOTOR_DATA);
  }
}
void cnProcess::WriteApCfgData(uint8_t *p_data, uint8_t length)
{
  memcpy(&m_stepBuffer[0], p_data, length);
  if (m_step.GetStep() == CN_PROCESS_STEP_TODO)
  {
    m_pre_time = millis();
    m_step.SetStep(CN_PROCESS_STEP_WRITE_AP_CFG);
  }
}
void cnProcess::WriteCylData(uint8_t *p_data, uint8_t length)
{
  memcpy(&m_stepBuffer[0], p_data, length);
  if (m_step.GetStep() == CN_PROCESS_STEP_TODO)
  {
    m_pre_time = millis();
    m_step.SetStep(CN_PROCESS_STEP_WRITE_CYL_DATA);
  }
}
void cnProcess::WriteVacData(uint8_t *p_data, uint8_t length)
{
  memcpy(&m_stepBuffer[0], p_data, length);
  if (m_step.GetStep() == CN_PROCESS_STEP_TODO)
  {
    m_pre_time = millis();
    m_step.SetStep(CN_PROCESS_STEP_WRITE_VAC_DATA);
  }
}

void cnProcess::WriteSeqData(uint8_t *p_data, uint8_t length)
{
  memcpy(&m_stepBuffer[0], p_data, length);
  if (m_step.GetStep() == CN_PROCESS_STEP_TODO)
  {
    m_pre_time = millis();
    m_step.SetStep(CN_PROCESS_STEP_WRITE_SEQ_DATA);
  }
}
