#include "ap.h"
#include "cnJob.h"



#define CN_JOB_STEP_RETRY_CNT_MAX                 3
#define CN_JOB_WAIT_TIMEOUT                       1000
#define CN_JOB_MOTOR_MOVE_TIMEOUT                 (1000*3)
#define CN_JOB_DRY_RUN_WAIT_TIME                  1000
#define CN_JOB_STEP_DELAY                         500

#define CN_JOB_BUFF_ASSIGN_MAX_CNT                m_stepBuffer[m_stepBuffer[0]+2]
#define CN_JOB_BUFF_ASSIGN_DATA_LENGTH            m_stepBuffer[0]
#define CN_JOB_BUFF_ASSIGN_INCREASE_NUM           m_stepBuffer[m_stepBuffer[0]+4]
#define CN_JOB_BUFF_ASSIGN_OFFSET                 m_stepBuffer[m_stepBuffer[0]+3]

#define CN_JOB_MONITOR_STEP_TEST_STANDBY          (uint8_t)0
#define CN_JOB_MONITOR_STEP_MOVE_START_VAC        (uint8_t)1
#define CN_JOB_MONITOR_STEP_PEEL_VINYL            (uint8_t)2
#define CN_JOB_MONITOR_STEP_PEEL_VINYL_CPLT       (uint8_t)3
#define CN_JOB_MONITOR_STEP_MOV_REATTACH          (uint8_t)4
#define CN_JOB_MONITOR_STEP_REATTACH              (uint8_t)5
#define CN_JOB_MONITOR_STEP_REATTACH_CPLT         (uint8_t)6
#define CN_JOB_MONITOR_STEP_TEST_CPLT             (uint8_t)7

#define PROC_ALARM(p_head, p_err)                 m_pAuto->AUTO_ALARM(p_head, p_err)


cnJob::cnJob() : m_pApReg(NULL)
{
  m_pApIo = NULL;
  m_pFm = NULL;
  m_pCyl = NULL;
  m_pVac = NULL;
  m_pOp = NULL;
  m_pApAxisDat = NULL;
  m_pApCylDat = NULL;
  m_pApVacDat = NULL;
  m_pApCfgDat = NULL;
  m_pApSeqDat = NULL;
  m_pAuto = NULL;
  //m_IsToDoState = false;
  m_pre_time = 0;
  m_step = {};
  m_retryCnt = 0;
  m_IsAutoReady = false;
  m_AutoSpeed = 0;
  m_prcStep = 0;
  memset(&m_stepBuffer, 0, CN_JOB_MAX_BUFFER_LENGTH);
}

cnJob::~cnJob()
{
  // TODO Auto-generated destructor stub
}

int cnJob::Init(cnJob::cfg_t *cfg)
{
  m_pApReg = cfg->p_apReg;
  m_pApIo = cfg->p_apIo;
  m_pFm = cfg->p_Fm;
  m_pCyl = cfg->p_Cyl;
  m_pVac = cfg->p_Vac;
  m_pOp = cfg->p_Op;
  m_pAuto = cfg->p_AutoManger;
  m_pApAxisDat = cfg->p_apAxisDat;
  m_pApCylDat = cfg->p_apCylDat;
  m_pApVacDat = cfg->p_apVacDat;
  m_pApCfgDat = cfg->p_apCfgDat;
  m_pApSeqDat = cfg->p_apSeqDat;
  m_pre_time = 0;
  //m_stepOp.SetStep(CN_JOB_STEP_INIT);
  m_step.SetStep(CN_JOB_STEP_INIT);
  memset(&m_stepBuffer, 0, CN_JOB_MAX_BUFFER_LENGTH);

  //
  return 0;
}


void cnJob::SetAutoSpeed(uint32_t speed)
{
  m_AutoSpeed = speed%101;
}

uint32_t cnJob::GetAutoSpeed() const
{
  return m_AutoSpeed;
}

uint8_t cnJob::GetStep() const
{
  return m_prcStep/*m_step.GetStep()*/;
}


void cnJob::doRunStep()
{
  if (m_pAuto->IsDetectAreaSensor())
    return;

  switch (m_step.GetStep())
  {
    case CN_JOB_STEP_INIT:
    {
      memset(&m_stepBuffer, 0, CN_JOB_MAX_BUFFER_LENGTH);
      m_step.SetStep(CN_JOB_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
       to do
      ######################################################*/
    case CN_JOB_STEP_TODO:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_JOB_STEP_PHONE_JIG_CLOSE);
    }
    break;

    /*######################################################
       phone jig open
      ######################################################*/
    case CN_JOB_STEP_PHONE_JIG_OPEN:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_JOB_STEP_PHONE_JIG_OPEN_START);
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_PHONE_JIG_OPEN_START:
    {
      //
      m_pCyl[AP_DEF_OBJ_CYLINDER_ID_PHONE_JIG].Open(true);
      m_step.SetStep(CN_JOB_STEP_PHONE_JIG_OPEN_WAIT);

     /* if(m_pApReg->IsAllCheckOk())
      {
        delay(CN_JOB_DRY_RUN_WAIT_TIME);
        m_step.SetStep(CN_JOB_STEP_PHONE_JIG_OPEN_END);
      }*/
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_PHONE_JIG_OPEN_WAIT:
    {
      if (millis() - m_pre_time >= CN_JOB_WAIT_TIMEOUT)
      {
        if (m_retryCnt++ > CN_JOB_STEP_RETRY_CNT_MAX)
        {
          m_retryCnt = 0;
          //m_pAuto->AlarmAuto(cnAuto::state_e::cyl_0_open_timeout);
          log_dat::head_t head_inf;
          head_inf.error_no = static_cast<uint8_t>(cnAuto::state_e::cyl_0_open_timeout);
          head_inf.obj_id = AP_DEF_OBJ_CYLINDER_ID_PHONE_JIG;
          head_inf.step_no = CN_JOB_STEP_PHONE_JIG_OPEN_WAIT;
          PROC_ALARM(&head_inf,"phone jig open timeout");
          m_step.SetStep(CN_JOB_STEP_PHONE_JIG_OPEN_START);
        }
        m_pre_time = millis();
      }

      if (m_pCyl[AP_DEF_OBJ_CYLINDER_ID_PHONE_JIG].IsOpen())// is open jig
      {
        m_step.SetStep(CN_JOB_STEP_PHONE_JIG_OPEN_END);
        m_pre_time = millis();
      }      
    }
    break;
    case CN_JOB_STEP_PHONE_JIG_OPEN_END:
    {
      if (millis() - m_pre_time < CN_JOB_STEP_DELAY)
        break;
      m_prcStep = CN_JOB_MONITOR_STEP_TEST_STANDBY;
      m_step.SetStep(CN_JOB_STEP_TODO);
      m_pre_time = millis();
    }
    break;

    /*######################################################
       phone jig close
      ######################################################*/
    case CN_JOB_STEP_PHONE_JIG_CLOSE:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_JOB_STEP_PHONE_JIG_CLOSE_START);
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_PHONE_JIG_CLOSE_START:
    {
      //
      m_pCyl[AP_DEF_OBJ_CYLINDER_ID_PHONE_JIG].Close(true);
      m_step.SetStep(CN_JOB_STEP_PHONE_JIG_CLOSE_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_PHONE_JIG_CLOSE_WAIT:
    {
      if (millis() - m_pre_time >= CN_JOB_WAIT_TIMEOUT)
      {
        if (m_retryCnt++ > CN_JOB_STEP_RETRY_CNT_MAX)
        {
          m_retryCnt = 0;
          //m_pAuto->AlarmAuto(cnAuto::state_e::cyl_0_close_timeout);
          log_dat::head_t head_inf;
          head_inf.error_no = static_cast<uint8_t>(cnAuto::state_e::cyl_0_close_timeout);
          head_inf.obj_id = GetCylObjId(AP_DEF_OBJ_CYLINDER_ID_PHONE_JIG);
          head_inf.step_no = CN_JOB_STEP_PHONE_JIG_CLOSE_WAIT;
          PROC_ALARM(&head_inf,"phone jig close timeout");
          m_step.SetStep(CN_JOB_STEP_PHONE_JIG_CLOSE_START);
        }
        m_pre_time = millis();
      }

      if (!m_pCyl[AP_DEF_OBJ_CYLINDER_ID_PHONE_JIG].IsOpen()) // is grip jig
      {
        m_step.SetStep(CN_JOB_STEP_PHONE_JIG_CLOSE_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_JOB_STEP_PHONE_JIG_CLOSE_END:
    {
      if (millis() - m_pre_time < CN_JOB_STEP_DELAY)
        break;
      m_step.SetStep(CN_JOB_STEP_POS_READY);
      m_pre_time = millis();
    }
    break;

#if 0  // not use in auto run
    /*######################################################
       axis pos get origin
      ######################################################*/
    case CN_JOB_STEP_POS_ORIGIN:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_JOB_STEP_POS_ORIGIN_START);
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_POS_ORIGIN_START:
    {
      //
      m_IsToDoState = false;
      m_step.SetStep(CN_JOB_STEP_POS_ORIGIN_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_POS_ORIGIN_WAIT:
    {
      if (millis() - m_pre_time >= CN_JOB_WAIT_TIMEOUT)
      {
        if (m_retryCnt++ < CN_JOB_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_JOB_STEP_POS_ORIGIN_START);
          m_pre_time = millis();
        }
        else
        {
          m_retryCnt = 0;
          m_stepOp.SetStep(CN_JOB_STEP_TIMEOUT);
          m_pre_time = millis();
        }
      }

      if (1)// is ok
      {
        m_step.SetStep(CN_JOB_STEP_POS_ORIGIN_END);
        m_pre_time = millis();
      } 
    }
    break;
    case CN_JOB_STEP_POS_ORIGIN_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
      m_step.SetStep(CN_JOB_STEP_TODO);
      m_pre_time = millis();
    }
    break;
#endif
    /*######################################################
       move axis position ready (process start)
      ######################################################*/
    case CN_JOB_STEP_POS_READY:
    {
      m_retryCnt = 0;
      bool is_interlock = false;
      //is_interlock |= m_pCyl[AP_DEF_OBJ_CYLINDER_ID_DRUM_Z_UP].IsDown();
      is_interlock |= m_pCyl[AP_DEF_OBJ_CYLINDER_ID_DRUM_UPDOWN].IsDown();
      if(is_interlock)
      {
        m_pAuto->AlarmAuto(cnAuto::state_e::cyl_interlock_State);
      }
      else
      {
        m_step.SetStep(CN_JOB_STEP_POS_READY_START);
      }
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_POS_READY_START:
    {
      axis_dat::dat_t pos_data = m_pApAxisDat->cmd_pos_dat[static_cast<uint8_t>(axis_dat::addr_e::ready_pos)];
      m_pFm->Move(pos_data.cmd_pos, (pos_data.cmd_vel * (m_pApSeqDat->GetMaxSpeed()/100)));

      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (m_pFm->IsInpose())
          break;

        if (millis()-pre_ms >= CN_JOB_WAIT_TIMEOUT)
        {
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAuto::state_e::axis_move_timeout);
          break;
        }
        delay(1);
      }

      m_step.SetStep(CN_JOB_STEP_POS_READY_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_POS_READY_WAIT:
    {
      if (millis() - m_pre_time >= CN_JOB_MOTOR_MOVE_TIMEOUT)
      {
        m_retryCnt = 0;
        //m_pAuto->AlarmAuto(cnAuto::state_e::axis_move_timeout);
        log_dat::head_t head_inf;
        head_inf.error_no = static_cast<uint8_t>(cnAuto::state_e::axis_move_timeout);
        head_inf.obj_id = AP_DEF_OBJ_MOTOR_ID_0;
        head_inf.step_no = CN_JOB_STEP_POS_READY_WAIT;
        PROC_ALARM(&head_inf,"ready_pos move timeout");
        m_step.SetStep(CN_JOB_STEP_POS_READY_START);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_JOB_STEP_POS_READY_END);
        m_pre_time = millis();
      } 
    }
    break;
    case CN_JOB_STEP_POS_READY_END:
    {
      if (millis() - m_pre_time < CN_JOB_STEP_DELAY)
        break;
      m_prcStep = CN_JOB_MONITOR_STEP_TEST_STANDBY;
      m_step.SetStep(CN_JOB_STEP_POS_CLEAN_DUST);
      m_pre_time = millis();
    }
    break;
    /*######################################################
       move axis position clean dust rolling
      ######################################################*/
    case CN_JOB_STEP_POS_CLEAN_DUST:
    {
      m_retryCnt = 0;
      bool is_interlock = false;
      if(is_interlock)
      {
        m_pAuto->AlarmAuto(cnAuto::state_e::cyl_interlock_State);
      }
      else
      {
        m_step.SetStep(CN_JOB_STEP_POS_CLEAN_DUST_START);
        m_pCyl[AP_DEF_OBJ_CYLINDER_ID_DRUM_STOP].Unlock(true);
      }
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_POS_CLEAN_DUST_START:
    {
      //
      axis_dat::dat_t pos_data = m_pApAxisDat->cmd_pos_dat[static_cast<uint8_t>(axis_dat::addr_e::pos_start_peel)];
      m_pFm->Move(pos_data.cmd_pos, (pos_data.cmd_vel * (m_AutoSpeed/100)));

      m_step.SetStep(CN_JOB_STEP_POS_CLEAN_DUST_WAIT);

      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (m_pFm->IsInpose())
          break;

        if (millis()-pre_ms >= CN_JOB_WAIT_TIMEOUT)
        {
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAuto::state_e::axis_move_timeout);
          break;
        }
        delay(1);
      }
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_POS_CLEAN_DUST_WAIT:
    {
      if (millis() - m_pre_time >= CN_JOB_MOTOR_MOVE_TIMEOUT)
      {
        m_retryCnt = 0;
        m_pAuto->AlarmAuto(cnAuto::state_e::axis_move_timeout);
        m_step.SetStep(CN_JOB_STEP_POS_CLEAN_DUST_START);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_JOB_STEP_POS_CLEAN_DUST_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_JOB_STEP_POS_CLEAN_DUST_END:
    {
      if (millis() - m_pre_time < CN_JOB_STEP_DELAY)
        break;

      m_retryCnt = 0;
      m_prcStep = CN_JOB_MONITOR_STEP_MOVE_START_VAC;
      m_step.SetStep(CN_JOB_STEP_DRUM_DOWN);
      m_pre_time = millis();
    }
    break;

    /*######################################################
       peel drum down  with vacuum on
      ######################################################*/
    case CN_JOB_STEP_DRUM_DOWN:
    {
      bool is_interlock = false;
      if(is_interlock)
      {
        m_pAuto->AlarmAuto(cnAuto::state_e::cyl_interlock_State);
      }
      else
      {
        m_step.SetStep(CN_JOB_STEP_DRUM_DOWN_START);
      }
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_DRUM_DOWN_START:
    {
      m_pCyl[AP_DEF_OBJ_CYLINDER_ID_DRUM_UPDOWN].Down(true);
      m_step.SetStep(CN_JOB_STEP_DRUM_DOWN_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_DRUM_DOWN_WAIT:
    {
      if (millis() - m_pre_time >= CN_JOB_WAIT_TIMEOUT * 3)
      {
        m_retryCnt = 0;
        m_pAuto->AlarmAuto(cnAuto::state_e::cyl_0_close_timeout);
        m_step.SetStep(CN_JOB_STEP_DRUM_DOWN_START);
        m_pre_time = millis();
      }

      if (m_pCyl[AP_DEF_OBJ_CYLINDER_ID_DRUM_UPDOWN].IsDown())
      {
        m_step.SetStep(CN_JOB_STEP_DRUM_DOWN_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_JOB_STEP_DRUM_DOWN_END:
    {
      if (millis() - m_pre_time < CN_JOB_STEP_DELAY)
        break;
      m_step.SetStep(CN_JOB_STEP_DRUM_UNLOCK);
      m_pre_time = millis();
    }
    break;

    /*######################################################
       peel unlock
      ######################################################*/
    case CN_JOB_STEP_DRUM_UNLOCK:
    {

      m_step.SetStep(CN_JOB_STEP_DRUM_UNLOCK_START);
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_DRUM_UNLOCK_START:
    {
      m_pCyl[AP_DEF_OBJ_CYLINDER_ID_DRUM_STOP].Unlock(true);
      m_step.SetStep(CN_JOB_STEP_DRUM_UNLOCK_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_DRUM_UNLOCK_WAIT:
    {
      if (millis() - m_pre_time >= CN_JOB_WAIT_TIMEOUT)
      {
        m_retryCnt = 0;
        m_pAuto->AlarmAuto(cnAuto::state_e::cyl_0_close_timeout);
        m_step.SetStep(CN_JOB_STEP_DRUM_UNLOCK_START);
        m_pre_time = millis();
      }

      if (m_pCyl[AP_DEF_OBJ_CYLINDER_ID_DRUM_STOP].IsUnlock())
      {
        m_step.SetStep(CN_JOB_STEP_DRUM_UNLOCK_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_JOB_STEP_DRUM_UNLOCK_END:
    {
      if (millis() - m_pre_time < CN_JOB_STEP_DELAY)
        break;

      if (m_pVac[AP_DEF_OBJ_VACUUM_ID_DRUM_HEAD].IsOn())
      {
        m_step.SetStep(CN_JOB_STEP_POS_REATTACH_RUN);
      }
      else
      {
        m_pVac[AP_DEF_OBJ_VACUUM_ID_DRUM_HEAD].On(true);
        m_step.SetStep(CN_JOB_STEP_POS_SUCTION_VINYL);
      }
      m_pre_time = millis();
    }
    break;


    /*######################################################
       move axis position vacuum suction to peel vinyl off
      ######################################################*/
    case CN_JOB_STEP_POS_SUCTION_VINYL:
    {
      bool is_interlock = false;
      is_interlock |= m_pCyl[AP_DEF_OBJ_CYLINDER_ID_DRUM_STOP].IsLock();
      if(is_interlock)
      {
        m_pAuto->AlarmAuto(cnAuto::state_e::cyl_interlock_State);
      }
      else
      {
        m_step.SetStep(CN_JOB_STEP_POS_SUCTION_VINYL_START);
      }
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_POS_SUCTION_VINYL_START:
    {
      seq_dat::dat_t seq_data = m_pApSeqDat->ReadData(seq_dat::addr_e::seq_peel_try);
      int dist_peel_try_mm = (int)seq_data.parm1;
      uint32_t speed_peel_try = (uint32_t)seq_data.parm2;
      m_pFm->RelMove(dist_peel_try_mm, speed_peel_try);
      m_step.SetStep(CN_JOB_STEP_POS_SUCTION_VINYL_WAIT);

      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (m_pFm->IsInpose())
          break;

        if (millis()-pre_ms >= CN_JOB_WAIT_TIMEOUT)
        {
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAuto::state_e::axis_move_timeout);
          break;
        }
        delay(1);
      }

      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_POS_SUCTION_VINYL_WAIT:
    {
      if (millis() - m_pre_time >= CN_JOB_MOTOR_MOVE_TIMEOUT)
      {
        m_retryCnt = 0;
        m_pAuto->AlarmAuto(cnAuto::state_e::axis_move_timeout);
        m_step.SetStep(CN_JOB_STEP_POS_SUCTION_VINYL_START);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_JOB_STEP_POS_SUCTION_VINYL_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_JOB_STEP_POS_SUCTION_VINYL_END:
    {
      if (millis() - m_pre_time < CN_JOB_STEP_DELAY)
        break;

      m_prcStep = CN_JOB_MONITOR_STEP_MOVE_START_VAC;
      m_step.SetStep(CN_JOB_STEP_CHECK_VACUUM_ON);
      m_pre_time = millis();
    }
    break;


    /*######################################################
       check vacuum on state and retry
      ######################################################*/
    case CN_JOB_STEP_CHECK_VACUUM_ON:
    {
      if (m_pVac[AP_DEF_OBJ_VACUUM_ID_DRUM_HEAD].IsOn() || IsDryRun())
      {
        m_step.SetStep(CN_JOB_STEP_POS_PEEL_VINYL);
      }
      else
      {
        m_step.SetStep(CN_JOB_STEP_CHECK_VACUUM_ON_RETRY);
        if (m_retryCnt++ > CN_JOB_STEP_RETRY_CNT_MAX)
        {
          m_retryCnt = 0;
          //m_pAuto->AlarmAuto(cnAuto::state_e::vac_0_on_timeout);
          log_dat::head_t head_inf;
          head_inf.error_no = static_cast<uint8_t>(cnAuto::state_e::vac_0_on_timeout);
          head_inf.obj_id = GetVacObjId(AP_DEF_OBJ_VACUUM_ID_DRUM_HEAD);
          head_inf.step_no = CN_JOB_STEP_CHECK_VACUUM_ON;
          PROC_ALARM(&head_inf,"vinyl vacuum on timeout");
          m_step.SetStep(CN_JOB_STEP_CHECK_VACUUM_ON);
        }
      }
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_CHECK_VACUUM_ON_RETRY:
    {
      //retry
      axis_dat::dat_t pos_data = m_pApAxisDat->cmd_pos_dat[static_cast<uint8_t>(axis_dat::addr_e::pos_start_peel)];
      m_pFm->Move(pos_data.cmd_pos, (pos_data.cmd_vel * (m_AutoSpeed/100)));
      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (millis()-pre_ms >= CN_JOB_WAIT_TIMEOUT)
        {
          break;
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAuto::state_e::axis_move_timeout);
        }
        delay(1);
      }

      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_CHECK_VACUUM_ON_END:
    {
      if (millis() - m_pre_time < CN_JOB_STEP_DELAY)
        break;

      m_step.SetStep(CN_JOB_STEP_POS_SUCTION_VINYL);
    }
    break;

    /*######################################################
       move axis position to peel vinyl off
      ######################################################*/
    case CN_JOB_STEP_POS_PEEL_VINYL:
    {
      m_retryCnt = 0;
      bool is_interlock = false;
      if(is_interlock)
      {
        m_pAuto->AlarmAuto(cnAuto::state_e::cyl_interlock_State);
      }
      else
      {
        m_step.SetStep(CN_JOB_STEP_POS_PEEL_VINYL_START);
        //m_pVac[AP_DEF_OBJ_VACUUM_ID_DRUM_TAIL].On(true);
      }
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_POS_PEEL_VINYL_START:
    {
      axis_dat::dat_t pos_data = m_pApAxisDat->cmd_pos_dat[static_cast<uint8_t>(axis_dat::addr_e::pos_vinyl_cplt)];
      m_pFm->Move(pos_data.cmd_pos, (pos_data.cmd_vel * (m_AutoSpeed/100)));
      m_step.SetStep(CN_JOB_STEP_POS_PEEL_VINYL_WAIT);

      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (m_pFm->IsInpose())
          break;

        if (millis()-pre_ms >= CN_JOB_WAIT_TIMEOUT)
        {
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAuto::state_e::axis_move_timeout);
          break;
        }
        delay(1);
      }
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_POS_PEEL_VINYL_WAIT:
    {
      if (millis() - m_pre_time >= CN_JOB_MOTOR_MOVE_TIMEOUT)
      {
        m_retryCnt = 0;
        m_pAuto->AlarmAuto(cnAuto::state_e::axis_move_timeout);
        m_step.SetStep(CN_JOB_STEP_POS_PEEL_VINYL_START);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_JOB_STEP_POS_PEEL_VINYL_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_JOB_STEP_POS_PEEL_VINYL_END:
    {
      m_prcStep = CN_JOB_MONITOR_STEP_PEEL_VINYL;
      m_step.SetStep(CN_JOB_STEP_POS_PEEL_VINYL_CPLT);
      m_pre_time = millis();
    }
    break;
    /*######################################################
       move axis position complete
      ######################################################*/
    case CN_JOB_STEP_POS_PEEL_VINYL_CPLT:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_JOB_STEP_POS_PEEL_VINYL_CPLT_START);
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_POS_PEEL_VINYL_CPLT_START:
    {
      //
      m_pCyl[AP_DEF_OBJ_CYLINDER_ID_DRUM_STOP].Lock(true);

      m_step.SetStep(CN_JOB_STEP_POS_PEEL_VINYL_CPLT_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_POS_PEEL_VINYL_CPLT_WAIT:
    {
      if (millis() - m_pre_time >= CN_JOB_WAIT_TIMEOUT)
      {
        m_retryCnt = 0;
        m_pAuto->AlarmAuto(cnAuto::state_e::cyl_0_close_timeout);
        m_step.SetStep(CN_JOB_STEP_POS_PEEL_VINYL_CPLT_START);
        m_pre_time = millis();
      }

      if (m_pCyl[AP_DEF_OBJ_CYLINDER_ID_DRUM_STOP].IsLock())
      {
        m_step.SetStep(CN_JOB_STEP_POS_PEEL_VINYL_CPLT_END);
        m_pCyl[AP_DEF_OBJ_CYLINDER_ID_DRUM_UPDOWN].Up(true);
        m_pre_time = millis();
      }
    }
    break;
    case CN_JOB_STEP_POS_PEEL_VINYL_CPLT_END:
    {
      if (millis() - m_pre_time >= CN_JOB_WAIT_TIMEOUT)
      {
        m_retryCnt = 0;
        m_pAuto->AlarmAuto(cnAuto::state_e::cyl_0_close_timeout);
        m_step.SetStep(CN_JOB_STEP_POS_PEEL_VINYL_CPLT_START);
        m_pre_time = millis();
      }

      if (m_pCyl[AP_DEF_OBJ_CYLINDER_ID_DRUM_UPDOWN].IsUp())
      {
        m_prcStep = CN_JOB_MONITOR_STEP_PEEL_VINYL_CPLT;
        m_step.SetStep(CN_JOB_STEP_WORK_COMPLETE);
        m_pre_time = millis();
      }
    }
    break;

    /*######################################################
       working complete
      ######################################################*/
    case CN_JOB_STEP_WORK_COMPLETE:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_JOB_STEP_WORK_COMPLETE_START);
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_WORK_COMPLETE_START:
    {
      //
      axis_dat::dat_t pos_data = m_pApAxisDat->cmd_pos_dat[static_cast<uint8_t>(axis_dat::addr_e::pos_destination)];
      m_pFm->Move(pos_data.cmd_pos, (pos_data.cmd_vel * (m_AutoSpeed/100)));

      m_step.SetStep(CN_JOB_STEP_WORK_COMPLETE_WAIT);
      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (m_pFm->IsInpose())
          break;

        if (millis()-pre_ms >= CN_JOB_WAIT_TIMEOUT)
        {
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAuto::state_e::axis_move_timeout);
          break;
        }
        delay(1);
      }
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_WORK_COMPLETE_WAIT:
    {
      if (millis() - m_pre_time >= CN_JOB_MOTOR_MOVE_TIMEOUT)
      {
        m_retryCnt = 0;
        m_pAuto->AlarmAuto(cnAuto::state_e::axis_move_timeout);
        m_step.SetStep(CN_JOB_STEP_WORK_COMPLETE_START);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_JOB_STEP_WORK_COMPLETE_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_JOB_STEP_WORK_COMPLETE_END:
    {
      if (millis() - m_pre_time < 1000*2)
        break;
      m_prcStep = CN_JOB_MONITOR_STEP_TEST_CPLT;
      m_step.SetStep(CN_JOB_STEP_POS_REATTACH);
      m_pre_time = millis();
    }
    break;
    /*######################################################
       move axis position start reattach vinyl
      ######################################################*/
    case CN_JOB_STEP_POS_REATTACH:
    {
      m_retryCnt = 0;

      bool is_interlock = false;
      if(is_interlock)
      {
        m_pAuto->AlarmAuto(cnAuto::state_e::cyl_interlock_State);
      }
      else
      {
        m_step.SetStep(CN_JOB_STEP_POS_REATTACH_START); //m_step.SetStep(CN_JOB_STEP_TODO);
      }
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_POS_REATTACH_START:
    {
      //
      axis_dat::dat_t pos_data = m_pApAxisDat->cmd_pos_dat[static_cast<uint8_t>(axis_dat::addr_e::pos_reattach)];
      m_pFm->Move(pos_data.cmd_pos, (pos_data.cmd_vel * (m_AutoSpeed/100)));

      m_step.SetStep(CN_JOB_STEP_POS_REATTACH_WAIT);

      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (m_pFm->IsInpose())
          break;

        if (millis()-pre_ms >= CN_JOB_WAIT_TIMEOUT)
        {
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAuto::state_e::axis_move_timeout);
          break;
        }
        delay(1);
      }
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_POS_REATTACH_WAIT:
    {
      if (millis() - m_pre_time >= CN_JOB_MOTOR_MOVE_TIMEOUT)
      {
        m_retryCnt = 0;
        m_pAuto->AlarmAuto(cnAuto::state_e::axis_move_timeout);
        m_step.SetStep(CN_JOB_STEP_POS_REATTACH_START);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_JOB_STEP_POS_REATTACH_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_JOB_STEP_POS_REATTACH_END:
    {
      if (millis() - m_pre_time < CN_JOB_STEP_DELAY)
        break;
      m_prcStep = CN_JOB_MONITOR_STEP_MOV_REATTACH;
      m_step.SetStep(CN_JOB_STEP_DRUM_DOWN);
      m_pre_time = millis();
    }
    break;
    /*######################################################
       move axis position run reattach vinyl on phone
      ######################################################*/
    case CN_JOB_STEP_POS_REATTACH_RUN:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_JOB_STEP_POS_REATTACH_RUN_START);
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_POS_REATTACH_RUN_START:
    {
      seq_dat::dat_t seq_data = m_pApSeqDat->ReadData(seq_dat::addr_e::seq_reattach);
      int dist_attach_try_mm = (int)seq_data.parm1;
      uint32_t speed_try = (uint32_t)seq_data.parm2;
      m_pFm->RelMove(dist_attach_try_mm, speed_try);
      m_step.SetStep(CN_JOB_STEP_POS_REATTACH_RUN_WAIT);

      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (m_pFm->IsInpose())
          break;

        if (millis()-pre_ms >= CN_JOB_WAIT_TIMEOUT)
        {
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAuto::state_e::axis_move_timeout);
          break;
        }
        delay(1);
      }

      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_POS_REATTACH_RUN_WAIT:
    {
      if (millis() - m_pre_time >= CN_JOB_MOTOR_MOVE_TIMEOUT)
      {
        m_retryCnt = 0;
        m_pAuto->AlarmAuto(cnAuto::state_e::axis_move_timeout);
        m_step.SetStep(CN_JOB_STEP_POS_REATTACH_RUN_START);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_JOB_STEP_POS_REATTACH_RUN_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_JOB_STEP_POS_REATTACH_RUN_END:
    {
      m_prcStep = CN_JOB_MONITOR_STEP_REATTACH;
      m_step.SetStep(CN_JOB_STEP_REATTACH_CPLT);
      m_pre_time = millis();
    }
    break;

    /*######################################################
       move reattach complete
      ######################################################*/
    case CN_JOB_STEP_REATTACH_CPLT:
    {
      m_retryCnt = 0;

      m_step.SetStep(CN_JOB_STEP_REATTACH_CPLT_START);
      m_pVac[AP_DEF_OBJ_VACUUM_ID_DRUM_HEAD].Off(true);

      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_REATTACH_CPLT_START:
    {
      axis_dat::dat_t pos_data = m_pApAxisDat->cmd_pos_dat[static_cast<uint8_t>(axis_dat::addr_e::pos_start_peel)];
      m_pFm->Move(pos_data.cmd_pos, (pos_data.cmd_vel * (m_AutoSpeed/100)));

      m_step.SetStep(CN_JOB_STEP_REATTACH_CPLT_WAIT);

      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (m_pFm->IsInpose())
          break;

        if (millis()-pre_ms >= CN_JOB_WAIT_TIMEOUT)
        {
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAuto::state_e::axis_move_timeout);
          break;
        }
        delay(1);
      }
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_REATTACH_CPLT_WAIT:
    {
      if (millis() - m_pre_time >= CN_JOB_MOTOR_MOVE_TIMEOUT)
      {
        m_retryCnt = 0;
        m_pAuto->AlarmAuto(cnAuto::state_e::axis_move_timeout);
        m_step.SetStep(CN_JOB_STEP_REATTACH_CPLT_START);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_JOB_STEP_REATTACH_CPLT_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_JOB_STEP_REATTACH_CPLT_END:
    {
      if (millis() - m_pre_time < CN_JOB_STEP_DELAY)
        break;
      m_prcStep = CN_JOB_MONITOR_STEP_REATTACH;
      m_step.SetStep(CN_JOB_STEP_ROLLER_ORIGIN);
      m_pre_time = millis();
    }
    break;
    /*######################################################
       peel drum unit get origin position
      ######################################################*/
    case CN_JOB_STEP_ROLLER_ORIGIN:
    {
      m_retryCnt = 0;

      m_pCyl[AP_DEF_OBJ_CYLINDER_ID_DRUM_STOP].Lock(true);
      m_step.SetStep(CN_JOB_STEP_ROLLER_ORIGIN_START);
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_ROLLER_ORIGIN_START:
    {
      //
      if (millis() - m_pre_time >= CN_JOB_WAIT_TIMEOUT)
      {
        m_retryCnt = 0;
        m_pAuto->AlarmAuto(cnAuto::state_e::cyl_0_close_timeout);
        m_step.SetStep(CN_JOB_STEP_ROLLER_ORIGIN);
        m_pre_time = millis();
      }

      if (!m_pCyl[AP_DEF_OBJ_CYLINDER_ID_DRUM_STOP].IsLock())
        break;

      m_pCyl[AP_DEF_OBJ_CYLINDER_ID_DRUM_UPDOWN].Up(true);
      m_step.SetStep(CN_JOB_STEP_ROLLER_ORIGIN_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_ROLLER_ORIGIN_WAIT:
    {
      if (millis() - m_pre_time >= CN_JOB_WAIT_TIMEOUT)
      {
        if (m_retryCnt++ > CN_JOB_STEP_RETRY_CNT_MAX)
        {
          m_retryCnt = 0;
          m_pAuto->AlarmAuto(cnAuto::state_e::seq_initial_timeout);
          m_step.SetStep(CN_JOB_STEP_ROLLER_ORIGIN_START);
        }
        m_pre_time = millis();
      }

      if (!m_pCyl[AP_DEF_OBJ_CYLINDER_ID_DRUM_UPDOWN].IsUp()) // not
        break;

      m_step.SetStep(CN_JOB_STEP_ROLLER_ORIGIN_END);
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_ROLLER_ORIGIN_END:
    {
      if (millis() - m_pre_time < CN_JOB_STEP_DELAY)
        break;
      m_prcStep = CN_JOB_MONITOR_STEP_TEST_CPLT;
      m_step.SetStep(CN_JOB_STEP_POS_READY);
      m_pre_time = millis();
    }
    break;
#if 0
    /*######################################################
       axis move relative position
      ######################################################*/
    case CN_JOB_STEP_REL_MOVE:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_JOB_STEP_REL_MOVE_START);
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_REL_MOVE_START:
    {
      int cmd_pos = 0;
      cmd_pos = utilDwToInt(&m_stepBuffer[0]);
      uint32_t cmd_vel = 0;
      cmd_vel = utilDwToUint(&m_stepBuffer[4]);
      m_pFm->RelMove(cmd_pos, (cmd_vel * (m_AutoSpeed/100)));

      m_step.SetStep(CN_JOB_STEP_POS_REATTACH_RUN_WAIT);
      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (m_pFm->IsAxisDone())
          break;

        if (millis()-pre_ms >= CN_JOB_WAIT_TIMEOUT)
        {
          break;
          m_step.SetStep(CN_JOB_STEP_TIMEOUT);
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAuto::state_e::axis_move_timeout);
        }
        delay(1);
      }
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_REL_MOVE_WAIT:
    {
      if (millis() - m_pre_time >= CN_JOB_MOTOR_MOVE_TIMEOUT)
      {
        m_retryCnt = 0;
        m_pAuto->AlarmAuto(cnAuto::state_e::axis_move_timeout);
        m_step.SetStep(CN_JOB_STEP_TIMEOUT);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {
        m_step..SetStep(CN_JOB_STEP_REL_MOVE_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_JOB_STEP_REL_MOVE_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
      m_step.SetStep(CN_JOB_STEP_TODO);
      m_pre_time = millis();
    }
    break;

#endif
    /*######################################################
       task timeout
      ######################################################*/
    case CN_JOB_STEP_TIMEOUT:
    {
      if (millis() - m_pre_time < 5)
        break;

      m_retryCnt = 0;
      m_pAuto->AlarmAuto(cnAuto::state_e::error_stop);
      m_step.SetStep(CN_JOB_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    case CN_JOB_STEP_END:
    {

    }
    break;
    default:
      break;
  }//switch
}

void cnJob::ThreadJob()
{
  switch (m_pAuto->GetOPMode())
  {
  case enOp::mode::STOP:
    m_IsAutoReady = false;
    if (millis() - m_pre_time >= 1000 * 1)
    {
      m_step.SetStep(CN_JOB_STEP_TODO);
    }
    break;

  case enOp::mode::DRY_RUN:
  case enOp::mode::AUTORUN:
  case enOp::mode::READY:
  {
    switch (m_pAuto->GetOPStatus())
    {
    case enOp::status::ERR_STOP:
      m_IsAutoReady = false;
      break;

    case enOp::status::RUN_READY:
      m_IsAutoReady = true;
      break;

    case enOp::status::RUN:
      doRunStep();
      break;

    case enOp::status::STEP_STOP:
    default:
    {
    }
    break;
    } // switch(m_pAuto->GetOPStatus())
  }
  break;
  } // switch(m_pAuto->GetOPMode())
}




bool cnJob::IsDryRun()
{
  return (m_pOp->GetMode() == enOp::mode::DRY_RUN);
}
