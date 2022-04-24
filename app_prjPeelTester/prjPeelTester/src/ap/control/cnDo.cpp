/*
 * cnDo.cpp
 *
 *  Created on: 2022. 4. 20.
 *      Author: gns2l
 */
#include "ap.h"
#include "cnDo.h"

#define CN_DO_STEP_RETRY_CNT_MAX                 3
#define CN_DO_WAIT_TIMEOUT                       100
#define CN_DO_MOTOR_MOVE_TIMEOUT                 (1000*3)
#define CN_DO_DRY_RUN_WAIT_TIME                  1000

#define CN_DO_BUFF_ASSIGN_MAX_CNT                m_stepBuffer[m_stepBuffer[0]+2]
#define CN_DO_BUFF_ASSIGN_DATA_LENGTH            m_stepBuffer[0]
#define CN_DO_BUFF_ASSIGN_INCREASE_NUM           m_stepBuffer[m_stepBuffer[0]+4]
#define CN_DO_BUFF_ASSIGN_OFFSET                 m_stepBuffer[m_stepBuffer[0]+3]

#define CN_DO_MONITOR_STEP_TEST_STANDBY          (uint8_t)0
#define CN_DO_MONITOR_STEP_MOVE_START_VAC        (uint8_t)1
#define CN_DO_MONITOR_STEP_PEEL_VINYL            (uint8_t)2
#define CN_DO_MONITOR_STEP_PEEL_VINYL_CPLT       (uint8_t)3
#define CN_DO_MONITOR_STEP_MOV_REATTACH          (uint8_t)4
#define CN_DO_MONITOR_STEP_REATTACH              (uint8_t)5
#define CN_DO_MONITOR_STEP_REATTACH_CPLT         (uint8_t)6
#define CN_DO_MONITOR_STEP_TEST_CPLT             (uint8_t)7

#define CN_DO_STEP_INIT                        0
#define CN_DO_STEP_TODO                        1

#define CN_DO_STEP_SEQ_INITIAL                 98
#define CN_DO_STEP_SEQ_INITIAL_START           99
#define CN_DO_STEP_SEQ_INITIAL_WAIT           100
#define CN_DO_STEP_SEQ_INITIAL_END            111

#define CN_DO_STEP_MOTOR_ON                    2
#define CN_DO_STEP_MOTOR_ON_START              3
#define CN_DO_STEP_MOTOR_ON_WAIT               4
#define CN_DO_STEP_MOTOR_ON_END                5

#define CN_DO_STEP_MOTOR_OFF                   6
#define CN_DO_STEP_MOTOR_OFF_START             7
#define CN_DO_STEP_MOTOR_OFF_WAIT              8
#define CN_DO_STEP_MOTOR_OFF_END               9

#define CN_DO_STEP_MOTOR_STOP                  10
#define CN_DO_STEP_MOTOR_STOP_START            11
#define CN_DO_STEP_MOTOR_STOP_WAIT             12
#define CN_DO_STEP_MOTOR_STOP_END              13

#define CN_DO_STEP_MOTOR_MOVE                  14
#define CN_DO_STEP_MOTOR_MOVE_START            15
#define CN_DO_STEP_MOTOR_MOVE_WAIT             16
#define CN_DO_STEP_MOTOR_MOVE_END              17

#define CN_DO_STEP_PHONE_JIG_OPEN              18
#define CN_DO_STEP_PHONE_JIG_OPEN_START        19
#define CN_DO_STEP_PHONE_JIG_OPEN_WAIT         20
#define CN_DO_STEP_PHONE_JIG_OPEN_END          21

#define CN_DO_STEP_PHONE_JIG_CLOSE             22
#define CN_DO_STEP_PHONE_JIG_CLOSE_START       23
#define CN_DO_STEP_PHONE_JIG_CLOSE_WAIT        24
#define CN_DO_STEP_PHONE_JIG_CLOSE_END         25

#define CN_DO_STEP_POS_ORIGIN                  26
#define CN_DO_STEP_POS_ORIGIN_START            27
#define CN_DO_STEP_POS_ORIGIN_WAIT             28
#define CN_DO_STEP_POS_ORIGIN_END              29

#define CN_DO_STEP_POS_READY                   30
#define CN_DO_STEP_POS_READY_START             31
#define CN_DO_STEP_POS_READY_WAIT              32
#define CN_DO_STEP_POS_READY_END               33

#define CN_DO_STEP_POS_CLEAN_DUST              34
#define CN_DO_STEP_POS_CLEAN_DUST_START        35
#define CN_DO_STEP_POS_CLEAN_DUST_WAIT         36
#define CN_DO_STEP_POS_CLEAN_DUST_END          37

#define CN_DO_STEP_POS_SUCTION_VINYL           38
#define CN_DO_STEP_POS_SUCTION_VINYL_START     39
#define CN_DO_STEP_POS_SUCTION_VINYL_WAIT      40
#define CN_DO_STEP_POS_SUCTION_VINYL_END       41

#define CN_DO_STEP_POS_PEEL_VINYL              42
#define CN_DO_STEP_POS_PEEL_VINYL_START        43
#define CN_DO_STEP_POS_PEEL_VINYL_WAIT         44
#define CN_DO_STEP_POS_PEEL_VINYL_END          45

#define CN_DO_STEP_POS_PEEL_VINYL_CPLT         46
#define CN_DO_STEP_POS_PEEL_VINYL_CPLT_START   47
#define CN_DO_STEP_POS_PEEL_VINYL_CPLT_WAIT    48
#define CN_DO_STEP_POS_PEEL_VINYL_CPLT_END     49

#define CN_DO_STEP_POS_REATTACH                50
#define CN_DO_STEP_POS_REATTACH_START          51
#define CN_DO_STEP_POS_REATTACH_WAIT           52
#define CN_DO_STEP_POS_REATTACH_END            53

#define CN_DO_STEP_POS_REATTACH_RUN            54
#define CN_DO_STEP_POS_REATTACH_RUN_START      55
#define CN_DO_STEP_POS_REATTACH_RUN_WAIT       56
#define CN_DO_STEP_POS_REATTACH_RUN_END        57

#define CN_DO_STEP_ROLLER_ORIGIN               70
#define CN_DO_STEP_ROLLER_ORIGIN_START         71
#define CN_DO_STEP_ROLLER_ORIGIN_WAIT          72
#define CN_DO_STEP_ROLLER_ORIGIN_END           73

#define CN_DO_STEP_DATA_CLEAR                  74
#define CN_DO_STEP_DATA_CLEAR_START            75
#define CN_DO_STEP_DATA_CLEAR_WAIT             76
#define CN_DO_STEP_DATA_CLEAR_END              77

#define CN_DO_STEP_WRITE_MOTOR_DATA            78
#define CN_DO_STEP_WRITE_MOTOR_DATA_START      79
#define CN_DO_STEP_WRITE_MOTOR_DATA_WAIT       80
#define CN_DO_STEP_WRITE_MOTOR_DATA_END        81

#define CN_DO_STEP_WRITE_AP_CFG                82
#define CN_DO_STEP_WRITE_AP_CFG_START          83
#define CN_DO_STEP_WRITE_AP_CFG_WAIT           84
#define CN_DO_STEP_WRITE_AP_CFG_END            85

#define CN_DO_STEP_WRITE_CYL_DATA              86
#define CN_DO_STEP_WRITE_CYL_DATA_START        87
#define CN_DO_STEP_WRITE_CYL_DATA_WAIT         88
#define CN_DO_STEP_WRITE_CYL_DATA_END          89

#define CN_DO_STEP_WRITE_VAC_DATA              90
#define CN_DO_STEP_WRITE_VAC_DATA_START        91
#define CN_DO_STEP_WRITE_VAC_DATA_WAIT         92
#define CN_DO_STEP_WRITE_VAC_DATA_END          93

#define CN_DO_STEP_WRITE_SEQ_DATA              94
#define CN_DO_STEP_WRITE_SEQ_DATA_START        95
#define CN_DO_STEP_WRITE_SEQ_DATA_WAIT         96
#define CN_DO_STEP_WRITE_SEQ_DATA_END          97

#define CN_DO_STEP_REL_MOVE                   112
#define CN_DO_STEP_REL_MOVE_START             113
#define CN_DO_STEP_REL_MOVE_WAIT              114
#define CN_DO_STEP_REL_MOVE_END               115

#define CN_DO_STEP_REATTACH_CPLT              116
#define CN_DO_STEP_REATTACH_CPLT_START        117
#define CN_DO_STEP_REATTACH_CPLT_WAIT         118
#define CN_DO_STEP_REATTACH_CPLT_END          119

#define CN_DO_STEP_WORK_COMPLETE              120
#define CN_DO_STEP_WORK_COMPLETE_START        121
#define CN_DO_STEP_WORK_COMPLETE_WAIT         122
#define CN_DO_STEP_WORK_COMPLETE_END          123

#define CN_DO_STEP_CHECK_VACUUM_ON            124
#define CN_DO_STEP_CHECK_VACUUM_ON_RETRY      125

#define CN_DO_STEP_TIMEOUT                    254
#define CN_DO_STEP_END                        255



cnDo::cnDo ()
{
  m_pApReg = NULL;
  m_pApIo = NULL;
  m_pFm = NULL;
  m_pCyl = NULL;
  m_pVac = NULL;
  m_pOp = NULL;
  m_pAuto = NULL;
  m_pApAxisDat = NULL;
  m_pApCylDat = NULL;
  m_pApVacDat = NULL;
  m_pApCfgDat = NULL;
  m_pApSeqDat = NULL;
  m_pre_time = 0;
  m_retryCnt = 0;
  m_step = {};

  memset(&m_stepBuffer, 0, CN_DO_MAX_BUFFER_LENGTH);

}

cnDo::~cnDo ()
{
  // TODO Auto-generated destructor stub
}

int cnDo::Init(cnDo::cfg_t *cfg)
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
  m_step.SetStep(0);

  memset(&m_stepBuffer, 0, CN_DO_MAX_BUFFER_LENGTH);

  //
  return 0;
}


void cnDo::ThreadJob()
{
  doRunStep();
}

void cnDo::doRunStep()
{

  switch (m_step.GetStep())
  {
    case CN_DO_STEP_INIT:
    {
      memset(&m_stepBuffer, 0, CN_DO_MAX_BUFFER_LENGTH);
      m_step.SetStep(CN_DO_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
         to do
      ######################################################*/
    case CN_DO_STEP_TODO:
    {
      m_retryCnt = 0;
    }
    break;
    /*######################################################
        operating sequence initial
      ######################################################*/
    case CN_DO_STEP_SEQ_INITIAL:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_DO_STEP_SEQ_INITIAL_START);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_SEQ_INITIAL_START:
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

      m_step.SetStep(CN_DO_STEP_SEQ_INITIAL_WAIT);
      if (m_pApReg->IsAllCheckOk())
      {
        m_step.SetStep(CN_DO_STEP_SEQ_INITIAL_END);
        break;
      }
      /*
      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (millis()-pre_ms >= CN_DO_WAIT_TIMEOUT)
        {
          m_step.SetStep(CN_DO_STEP_TIMEOUT);
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAuto::state_e::axis_move_timeout);
          break;
        }
      }*/
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_SEQ_INITIAL_WAIT:
    {
      if (millis() - m_pre_time >= CN_DO_WAIT_TIMEOUT*100)
      {
        m_retryCnt = 0;
        m_pAuto->AlarmAuto(cnAuto::state_e::seq_initial_timeout);
        m_step.SetStep(CN_DO_STEP_TIMEOUT);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_DO_STEP_SEQ_INITIAL_END);
        m_pre_time = millis();
      }

    }
    break;
    case CN_DO_STEP_SEQ_INITIAL_END:
    {
      // state
      m_pCyl[AP_DEF_OBJ_CYLINDER_ID_PHONE_JIG].Open(true);
      m_pApReg->SetRunState(AP_REG_ORG_COMPLETED, true);

      m_step.SetStep(CN_DO_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
         operating motor enable
      ######################################################*/
    case CN_DO_STEP_MOTOR_ON:
    {
      if (m_pFm->IsMotorOn())
      {
        m_step.SetStep(CN_DO_STEP_MOTOR_ON_END);
        break;
      }
      m_retryCnt = 0;
      m_step.SetStep(CN_DO_STEP_MOTOR_ON_START);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_MOTOR_ON_START:
    {
      if (m_pFm->IsMotorOn())
      {
        m_step.SetStep(CN_DO_STEP_MOTOR_ON_END);
        break;
      }

      m_pFm->MotorOnOff(true);
      m_step.SetStep(CN_DO_STEP_MOTOR_ON_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_MOTOR_ON_WAIT:
    {
      if (millis() - m_pre_time >= (CN_DO_WAIT_TIMEOUT*10))
      {
        if (m_retryCnt++ < CN_DO_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_DO_STEP_MOTOR_ON_START);
          m_pre_time = millis();
        }
        else
        {
          m_retryCnt = 0;
          m_step.SetStep(CN_DO_STEP_TIMEOUT);
          m_pAuto->AlarmAuto(cnAuto::state_e::servo_on_err);
          m_pre_time = millis();
        }
      }

      if (m_pFm->IsMotorOn())
      {
        m_step.SetStep(CN_DO_STEP_MOTOR_ON_END);
        m_pre_time = millis();
      }

    }
    break;
    case CN_DO_STEP_MOTOR_ON_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
      m_step.SetStep(CN_DO_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
         operating motor disable
      ######################################################*/
    case CN_DO_STEP_MOTOR_OFF:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_DO_STEP_MOTOR_OFF_START);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_MOTOR_OFF_START:
    {
      m_pFm->MotorOnOff(false);
      m_step.SetStep(CN_DO_STEP_MOTOR_OFF_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_MOTOR_OFF_WAIT:
    {
      if (millis() - m_pre_time >= (CN_DO_WAIT_TIMEOUT*10))
      {
        if (m_retryCnt++ < CN_DO_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_DO_STEP_MOTOR_OFF_START);
          m_pre_time = millis();
        }
        else
        {
          m_retryCnt = 0;
          m_step.SetStep(CN_DO_STEP_TIMEOUT);
          m_pAuto->AlarmAuto(cnAuto::state_e::servo_on_err);
          m_pre_time = millis();
        }
      }

      if (!m_pFm->IsMotorOn())
      {
        m_step.SetStep(CN_DO_STEP_MOTOR_OFF_END);
        m_pre_time = millis();
      }

    }
    break;
    case CN_DO_STEP_MOTOR_OFF_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
      m_step.SetStep(CN_DO_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
         operating motor move stop
      ######################################################*/
    case CN_DO_STEP_MOTOR_STOP:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_DO_STEP_MOTOR_STOP_START);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_MOTOR_STOP_START:
    {
      m_pFm->Stop();
      m_step.SetStep(CN_DO_STEP_MOTOR_STOP_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_MOTOR_STOP_WAIT:
    {
      if (millis() - m_pre_time >= CN_DO_WAIT_TIMEOUT)
      {
        if (m_retryCnt++ < CN_DO_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_DO_STEP_MOTOR_STOP_START);
          m_pre_time = millis();
        }
        else
        {
          m_retryCnt = 0;
          m_pAuto->AlarmAuto(cnAuto::state_e::axis_stop_timeout);
          m_step.SetStep(CN_DO_STEP_TIMEOUT);
          m_pre_time = millis();
        }
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_DO_STEP_MOTOR_STOP_END);
        m_pre_time = millis();
      }

    }
    break;
    case CN_DO_STEP_MOTOR_STOP_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
      m_step.SetStep(CN_DO_STEP_TODO);
      m_pre_time = millis();
    }
    break;

    /*######################################################
         operating motor move move
      ######################################################*/
    case CN_DO_STEP_MOTOR_MOVE:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_DO_STEP_MOTOR_MOVE_START);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_MOTOR_MOVE_START:
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

      m_step.SetStep(CN_DO_STEP_MOTOR_MOVE_WAIT);
      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (m_pFm->IsAxisDone())
          break;

        if (millis()-pre_ms >= CN_DO_WAIT_TIMEOUT)
        {
          break;
          m_step.SetStep(CN_DO_STEP_TIMEOUT);
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAuto::state_e::axis_move_timeout);
        }
        delay(1);
      }
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_MOTOR_MOVE_WAIT:
    {
      if (millis() - m_pre_time >= CN_DO_MOTOR_MOVE_TIMEOUT)
      {
        m_retryCnt = 0;
        m_step.SetStep(CN_DO_STEP_TIMEOUT);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_DO_STEP_MOTOR_MOVE_END);
        m_pre_time = millis();
      }

    }
    break;
    case CN_DO_STEP_MOTOR_MOVE_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
      m_step.SetStep(CN_DO_STEP_TODO);
      m_pre_time = millis();
    }
    break;

    /*######################################################
         operating phonejig open
      ######################################################*/
    case CN_DO_STEP_PHONE_JIG_OPEN:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_DO_STEP_PHONE_JIG_OPEN_START);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_PHONE_JIG_OPEN_START:
    {
      //
      m_pCyl[AP_DEF_OBJ_CYLINDER_ID_PHONE_JIG].Open(true);
      m_step.SetStep(CN_DO_STEP_PHONE_JIG_OPEN_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_PHONE_JIG_OPEN_WAIT:
    {

      if (millis() - m_pre_time >= CN_DO_WAIT_TIMEOUT)
      {
        if (m_retryCnt++ < CN_DO_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_DO_STEP_PHONE_JIG_OPEN_START);
          m_pre_time = millis();
        }
        else
        {
          m_retryCnt = 0;
          m_pAuto->AlarmAuto(cnAuto::state_e::cyl_0_open_timeout);
          m_step.SetStep(CN_DO_STEP_TIMEOUT);
          m_pre_time = millis();
        }
      }

      if (m_pCyl[AP_DEF_OBJ_CYLINDER_ID_PHONE_JIG].IsOpen()) // is close jig
      {
        m_step.SetStep(CN_DO_STEP_PHONE_JIG_OPEN_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_DO_STEP_PHONE_JIG_OPEN_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
      m_step.SetStep(CN_DO_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
         operating phonejig close
      ######################################################*/
    case CN_DO_STEP_PHONE_JIG_CLOSE:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_DO_STEP_PHONE_JIG_CLOSE_START);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_PHONE_JIG_CLOSE_START:
    {
      //
      m_pCyl[AP_DEF_OBJ_CYLINDER_ID_PHONE_JIG].Close(true);
      m_step.SetStep(CN_DO_STEP_PHONE_JIG_CLOSE_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_PHONE_JIG_CLOSE_WAIT:
    {
      if (millis() - m_pre_time >= CN_DO_WAIT_TIMEOUT)
      {
        if (m_retryCnt++ < CN_DO_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_DO_STEP_PHONE_JIG_CLOSE_START);
          m_pre_time = millis();
        }
        else
        {
          m_retryCnt = 0;
          m_pAuto->AlarmAuto(cnAuto::state_e::cyl_0_close_timeout);
          m_step.SetStep(CN_DO_STEP_TIMEOUT);
          m_pre_time = millis();
        }
      }

      if (m_pCyl[AP_DEF_OBJ_CYLINDER_ID_PHONE_JIG].IsClose()) // is close jig
      {
        m_step.SetStep(CN_DO_STEP_PHONE_JIG_CLOSE_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_DO_STEP_PHONE_JIG_CLOSE_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
      m_step.SetStep(CN_DO_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
         operating get axis origin
      ######################################################*/
    case CN_DO_STEP_POS_ORIGIN:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_DO_STEP_POS_ORIGIN_START);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_POS_ORIGIN_START:
    {
      //
      m_pFm->MoveOrigin();
      m_step.SetStep(CN_DO_STEP_POS_ORIGIN_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_POS_ORIGIN_WAIT:
    {
      if (millis() - m_pre_time >= 1000*10)
      {
        m_retryCnt = 0;
        m_pAuto->AlarmAuto(cnAuto::state_e::axis_origin_err);
        m_step.SetStep(CN_DO_STEP_TIMEOUT);
        m_pre_time = millis();
      }

      if (m_pFm->IsOriginOK())// is ok
      {
        m_step.SetStep(CN_DO_STEP_POS_ORIGIN_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_DO_STEP_POS_ORIGIN_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
      m_step.SetStep(CN_DO_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
         operating axis move ready position
      ######################################################*/
    case CN_DO_STEP_POS_READY:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_DO_STEP_POS_READY_START);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_POS_READY_START:
    {
      //
      axis_dat::dat_t pos_data = m_pApAxisDat->cmd_pos_dat[static_cast<uint8_t>(axis_dat::addr_e::ready_pos)];
      m_pFm->Move(pos_data.cmd_pos, pos_data.cmd_vel);

      m_step.SetStep(CN_DO_STEP_POS_READY_WAIT);

      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (m_pFm->IsAxisDone())
          break;

        if (millis()-pre_ms >= CN_DO_WAIT_TIMEOUT)
        {
          break;
          m_step.SetStep(CN_DO_STEP_TIMEOUT);
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAuto::state_e::axis_move_timeout);
        }
        delay(1);
      }
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_POS_READY_WAIT:
    {
      if (millis() - m_pre_time >= CN_DO_MOTOR_MOVE_TIMEOUT)
      {
        m_retryCnt = 0;
        m_step.SetStep(CN_DO_STEP_TIMEOUT);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_DO_STEP_POS_READY_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_DO_STEP_POS_READY_END:
    {
      m_step.SetStep(CN_DO_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
         operating axis move clean dust position
      ######################################################*/
    case CN_DO_STEP_POS_CLEAN_DUST:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_DO_STEP_POS_CLEAN_DUST_START);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_POS_CLEAN_DUST_START:
    {
      //
      axis_dat::dat_t pos_data = m_pApAxisDat->cmd_pos_dat[static_cast<uint8_t>(axis_dat::addr_e::pos_start_peel)];
      m_pFm->Move(pos_data.cmd_pos, pos_data.cmd_vel);

      m_step.SetStep(CN_DO_STEP_POS_CLEAN_DUST_WAIT);

      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (m_pFm->IsAxisDone())
          break;

        if (millis()-pre_ms >= CN_DO_WAIT_TIMEOUT)
        {
          m_step.SetStep(CN_DO_STEP_TIMEOUT);
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAuto::state_e::axis_move_timeout);
          break;
        }
        delay(1);
      }
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_POS_CLEAN_DUST_WAIT:
    {
      if (millis() - m_pre_time >= CN_DO_MOTOR_MOVE_TIMEOUT)
      {
        m_retryCnt = 0;
        m_step.SetStep(CN_DO_STEP_TIMEOUT);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_DO_STEP_POS_CLEAN_DUST_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_DO_STEP_POS_CLEAN_DUST_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
      m_step.SetStep(CN_DO_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
         operating axis move vacuum suction position
      ######################################################*/
    case CN_DO_STEP_POS_SUCTION_VINYL:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_DO_STEP_POS_SUCTION_VINYL_START);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_POS_SUCTION_VINYL_START:
    {
      //
      axis_dat::dat_t pos_data = m_pApAxisDat->cmd_pos_dat[static_cast<uint8_t>(axis_dat::addr_e::pos_vinyl_cplt)];
      m_pFm->Move(pos_data.cmd_pos, pos_data.cmd_vel);

      m_step.SetStep(CN_DO_STEP_POS_SUCTION_VINYL_WAIT);

      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (m_pFm->IsAxisDone())
          break;

        if (millis()-pre_ms >= CN_DO_WAIT_TIMEOUT)
        {
          m_step.SetStep(CN_DO_STEP_TIMEOUT);
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAuto::state_e::axis_move_timeout);
          break;
        }
        delay(1);
      }
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_POS_SUCTION_VINYL_WAIT:
    {
      if (millis() - m_pre_time >= CN_DO_MOTOR_MOVE_TIMEOUT)
      {
        m_retryCnt = 0;
        m_step.SetStep(CN_DO_STEP_TIMEOUT);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_DO_STEP_POS_SUCTION_VINYL_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_DO_STEP_POS_SUCTION_VINYL_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
      m_step.SetStep(CN_DO_STEP_TODO);
      m_pre_time = millis();
    }
    break;


    /*######################################################
       operating peel drum origin
      ######################################################*/
    case CN_DO_STEP_ROLLER_ORIGIN:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_DO_STEP_ROLLER_ORIGIN_START);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_ROLLER_ORIGIN_START:
    {
      //
      m_step.SetStep(CN_DO_STEP_ROLLER_ORIGIN_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_ROLLER_ORIGIN_WAIT:
    {
      if (millis() - m_pre_time >= CN_DO_WAIT_TIMEOUT)
      {
        if (m_retryCnt++ < CN_DO_STEP_RETRY_CNT_MAX)
        {
          m_step.SetStep(CN_DO_STEP_ROLLER_ORIGIN_START);
          m_pre_time = millis();
        }
        else
        {
          m_retryCnt = 0;
          m_step.SetStep(CN_DO_STEP_TIMEOUT);
          m_pre_time = millis();
        }
      }

      if (1) // is ok
      {
        m_step.SetStep(CN_DO_STEP_ROLLER_ORIGIN_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_DO_STEP_ROLLER_ORIGIN_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
      m_step.SetStep(CN_DO_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
       data clear
      ######################################################*/
    case CN_DO_STEP_DATA_CLEAR:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_DO_STEP_DATA_CLEAR_START);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_DATA_CLEAR_START:
    {
      m_pApAxisDat->ClearRomData();
      m_pApCfgDat->ClearRomData();
      m_pApCylDat->ClearRomData();
      m_pApVacDat->ClearRomData();
      m_pApSeqDat->ClearRomData();

      m_step.SetStep(CN_DO_STEP_DATA_CLEAR_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_DATA_CLEAR_WAIT:
    {
      if(millis()-m_pre_time < 10)
        break;

      /* delay 10ms */
      m_step.SetStep(CN_DO_STEP_DATA_CLEAR_END);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_DATA_CLEAR_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
      m_step.SetStep(CN_DO_STEP_TODO);
      m_pre_time = millis();
    }
    break;

    /*######################################################
    CN_DO_STEP_WRITE_MOTOR_DATA
    ######################################################*/
    case CN_DO_STEP_WRITE_MOTOR_DATA:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_DO_STEP_WRITE_MOTOR_DATA_START);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_WRITE_MOTOR_DATA_START:
    {
      axis_dat::dat_t data;
      data.cmd_pos = utilDwToInt(&m_stepBuffer[4]);
      data.cmd_vel = utilDwToUint(&m_stepBuffer[8]);
      axis_dat::addr_e addr = static_cast<axis_dat::addr_e>(utilDwToUint(&m_stepBuffer[0]));
      m_pApAxisDat->WriteData(addr, data);

      m_step.SetStep(CN_DO_STEP_WRITE_MOTOR_DATA_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_WRITE_MOTOR_DATA_WAIT:
    {
      if (millis() - m_pre_time < 20)
        break;

      m_step.SetStep(CN_DO_STEP_WRITE_MOTOR_DATA_END);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_WRITE_MOTOR_DATA_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;

      m_step.SetStep(CN_DO_STEP_TODO);
      m_pre_time = millis();
    }
    break;

    /*######################################################
    CN_DO_STEP_WRITE_AP_CFG
    ######################################################*/
    case CN_DO_STEP_WRITE_AP_CFG:
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
      CN_DO_BUFF_ASSIGN_MAX_CNT =  CN_DO_BUFF_ASSIGN_DATA_LENGTH / sizeof(ap_dat::dat_t) ;
      CN_DO_BUFF_ASSIGN_OFFSET = 1;
      CN_DO_BUFF_ASSIGN_INCREASE_NUM = 0;
      m_step.SetStep(CN_DO_STEP_WRITE_AP_CFG_START);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_WRITE_AP_CFG_START:
    {
      uint8_t idx = CN_DO_BUFF_ASSIGN_OFFSET + (sizeof(ap_dat::dat_t)* CN_DO_BUFF_ASSIGN_INCREASE_NUM) ;
      ap_dat::dat_t data;
      data.parm1 = m_stepBuffer[idx + 0] | (m_stepBuffer[idx + 1] << 8);
      data.parm2 = m_stepBuffer[idx + 2] | (m_stepBuffer[idx + 3] << 8);
      ap_dat::addr_e addr = static_cast<ap_dat::addr_e>(CN_DO_BUFF_ASSIGN_INCREASE_NUM++);
      m_pApCfgDat->WriteData(addr, data);

      m_step.SetStep(CN_DO_STEP_WRITE_AP_CFG_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_WRITE_AP_CFG_WAIT:
    {
      if (millis() - m_pre_time < 10)
        break;

      if (CN_DO_BUFF_ASSIGN_INCREASE_NUM < CN_DO_BUFF_ASSIGN_MAX_CNT)
      {
        m_step.SetStep(CN_DO_STEP_WRITE_AP_CFG_START);
      }
      else
      {
        m_step.SetStep(CN_DO_STEP_WRITE_AP_CFG_END);
      }
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_WRITE_AP_CFG_END:
    {
      m_step.SetStep(CN_DO_STEP_TODO);
      m_pre_time = millis();
    }
    break;

    /*######################################################
    CN_DO_STEP_WRITE_CYL_DATA
    ######################################################*/
    case CN_DO_STEP_WRITE_CYL_DATA:
    {
      m_retryCnt = 0;

      CN_DO_BUFF_ASSIGN_MAX_CNT = CN_DO_BUFF_ASSIGN_DATA_LENGTH / sizeof(cyl_dat::dat_t);
      CN_DO_BUFF_ASSIGN_OFFSET = 1;
      CN_DO_BUFF_ASSIGN_INCREASE_NUM = 0;
      m_step.SetStep(CN_DO_STEP_WRITE_CYL_DATA_START);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_WRITE_CYL_DATA_START:
    {
      uint8_t idx = CN_DO_BUFF_ASSIGN_OFFSET + (sizeof(cyl_dat::dat_t)* CN_DO_BUFF_ASSIGN_INCREASE_NUM) ;
      cyl_dat::dat_t data;
      data.timeout = m_stepBuffer[idx + 0] | (m_stepBuffer[idx + 1] << 8);
      data.on_settling = m_stepBuffer[idx + 2] | (m_stepBuffer[idx + 3] << 8);
      cyl_dat::addr_e addr = static_cast<cyl_dat::addr_e>(CN_DO_BUFF_ASSIGN_INCREASE_NUM++);
      m_pApCylDat->WriteData(addr, data);

      m_step.SetStep(CN_DO_STEP_WRITE_CYL_DATA_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_WRITE_CYL_DATA_WAIT:
    {
      if (millis() - m_pre_time < 10)
        break;

      if (CN_DO_BUFF_ASSIGN_INCREASE_NUM < CN_DO_BUFF_ASSIGN_MAX_CNT)
      {
        m_step.SetStep(CN_DO_STEP_WRITE_CYL_DATA_START);
      }
      else
      {
        m_step.SetStep(CN_DO_STEP_WRITE_CYL_DATA_END);
      }
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_WRITE_CYL_DATA_END:
    {
      m_step.SetStep(CN_DO_STEP_TODO);
      m_pre_time = millis();
    }
    break;

    /*######################################################
    CN_DO_STEP_WRITE_VAC_DATA
    ######################################################*/
    case CN_DO_STEP_WRITE_VAC_DATA:
    {
      m_retryCnt = 0;
      CN_DO_BUFF_ASSIGN_MAX_CNT =  CN_DO_BUFF_ASSIGN_DATA_LENGTH / sizeof(vac_dat::dat_t) ;
      CN_DO_BUFF_ASSIGN_OFFSET = 1;
      CN_DO_BUFF_ASSIGN_INCREASE_NUM = 0;
      m_step.SetStep(CN_DO_STEP_WRITE_VAC_DATA_START);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_WRITE_VAC_DATA_START:
    {
      uint8_t idx = CN_DO_BUFF_ASSIGN_OFFSET + (sizeof(vac_dat::dat_t)* CN_DO_BUFF_ASSIGN_INCREASE_NUM) ;
      vac_dat::dat_t data;
      data.timeout = m_stepBuffer[idx + 0] | (m_stepBuffer[idx + 1] << 8);
      data.on_settling = m_stepBuffer[idx + 2] | (m_stepBuffer[idx + 3] << 8);
      vac_dat::addr_e addr = static_cast<vac_dat::addr_e>(CN_DO_BUFF_ASSIGN_INCREASE_NUM++);
      m_pApVacDat->WriteData(addr, data);

      m_step.SetStep(CN_DO_STEP_WRITE_VAC_DATA_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_WRITE_VAC_DATA_WAIT:
    {
      if (millis() - m_pre_time < 10)
        break;

      if (CN_DO_BUFF_ASSIGN_INCREASE_NUM < CN_DO_BUFF_ASSIGN_MAX_CNT)
      {
        m_step.SetStep(CN_DO_STEP_WRITE_VAC_DATA_START);
      }
      else
      {
        m_step.SetStep(CN_DO_STEP_WRITE_VAC_DATA_END);
      }
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_WRITE_VAC_DATA_END:
    {
      m_step.SetStep(CN_DO_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    /*######################################################
    CN_DO_STEP_WRITE_SEQ_DATA
    ######################################################*/
    case CN_DO_STEP_WRITE_SEQ_DATA:
    {
      m_retryCnt = 0;
      CN_DO_BUFF_ASSIGN_MAX_CNT = CN_DO_BUFF_ASSIGN_DATA_LENGTH / sizeof(seq_dat::dat_t);
      CN_DO_BUFF_ASSIGN_OFFSET = 1;
      CN_DO_BUFF_ASSIGN_INCREASE_NUM = 0;
      m_step.SetStep(CN_DO_STEP_WRITE_SEQ_DATA_START);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_WRITE_SEQ_DATA_START:
    {
      uint8_t idx = CN_DO_BUFF_ASSIGN_OFFSET + (sizeof(seq_dat::dat_t)* CN_DO_BUFF_ASSIGN_INCREASE_NUM) ;
      seq_dat::dat_t data;
      data.parm1 = m_stepBuffer[idx + 0] | (m_stepBuffer[idx + 1] << 8);
      data.parm2 = m_stepBuffer[idx + 2] | (m_stepBuffer[idx + 3] << 8);
      seq_dat::addr_e addr = static_cast<seq_dat::addr_e>(CN_DO_BUFF_ASSIGN_INCREASE_NUM++);
      m_pApSeqDat->WriteData(addr, data);

      m_step.SetStep(CN_DO_STEP_WRITE_SEQ_DATA_WAIT);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_WRITE_SEQ_DATA_WAIT:
    {
      if (millis() - m_pre_time < 10)
        break;

      if (CN_DO_BUFF_ASSIGN_INCREASE_NUM < CN_DO_BUFF_ASSIGN_MAX_CNT)
      {
        m_step.SetStep(CN_DO_STEP_WRITE_SEQ_DATA_START);
      }
      else
      {
        m_step.SetStep(CN_DO_STEP_WRITE_SEQ_DATA_END);
      }
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_WRITE_SEQ_DATA_END:
    {
      m_step.SetStep(CN_DO_STEP_TODO);
      m_pre_time = millis();
    }
    break;

    /*######################################################
     axis move relative position
    ######################################################*/
    case CN_DO_STEP_REL_MOVE:
    {
      m_retryCnt = 0;
      m_step.SetStep(CN_DO_STEP_REL_MOVE_START);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_REL_MOVE_START:
    {
      int cmd_pos = 0;
      cmd_pos = utilDwToInt(&m_stepBuffer[0]);
      uint32_t cmd_vel = 0;
      cmd_vel = utilDwToUint(&m_stepBuffer[4]);
      m_pFm->RelMove(cmd_pos, cmd_vel);

      m_step.SetStep(CN_DO_STEP_POS_REATTACH_RUN_WAIT);
      uint32_t pre_ms = millis();
      while(!m_pFm->IsBusy()) // not
      {
        if (m_pFm->IsAxisDone())
          break;

        if (millis()-pre_ms >= CN_DO_WAIT_TIMEOUT)
        {
          m_step.SetStep(CN_DO_STEP_TIMEOUT);
          // setting alarm bit
          m_pAuto->AlarmAuto(cnAuto::state_e::axis_move_timeout);
          break;
        }
        delay(1);
      }
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_REL_MOVE_WAIT:
    {
      if (millis() - m_pre_time >= CN_DO_MOTOR_MOVE_TIMEOUT)
      {
        m_retryCnt = 0;
        m_pAuto->AlarmAuto(cnAuto::state_e::axis_move_timeout);
        m_step.SetStep(CN_DO_STEP_TIMEOUT);
        m_pre_time = millis();
      }

      if (m_pFm->IsAxisDone())
      {
        m_step.SetStep(CN_DO_STEP_REL_MOVE_END);
        m_pre_time = millis();
      }
    }
    break;
    case CN_DO_STEP_REL_MOVE_END:
    {
      //if (millis() - m_pre_time < 5)
      //  break;
      m_step.SetStep(CN_DO_STEP_TODO);
      m_pre_time = millis();
    }
    break;

    /*######################################################
       timeout
    ######################################################*/
    case CN_DO_STEP_TIMEOUT:
    {
      if (millis() - m_pre_time < 5)
        break;
      m_retryCnt = 0;
      m_step.SetStep(CN_DO_STEP_TODO);
      m_pre_time = millis();
    }
    break;
    case CN_DO_STEP_END:
    {

    }
    break;
    default:
      break;
  }//switch
}

void cnDo::Initialize()
{
  // vacuum
  m_step.SetStep(CN_DO_STEP_SEQ_INITIAL);
  m_pre_time = millis();
}

int cnDo::VacOn(uint8_t obj_id, bool skip_sensor )
{
  int ret = 0;
  ret = m_pVac[obj_id].On(skip_sensor);
  if (ret != 0)
  {
    uint8_t err = static_cast<uint8_t>(cnAuto::state_e::vac_0_on_timeout)+obj_id;
    m_pAuto->AlarmAuto(static_cast<cnAuto::state_e>(err));
  }
  return ret;
}
int cnDo::VacOff(uint8_t obj_id, bool skip_sensor )
{
  int ret = 0;
  ret = m_pVac[obj_id].Off(skip_sensor);
  if (ret != 0)
  {
    uint8_t err = static_cast<uint8_t>(cnAuto::state_e::vac_0_off_timeout)+obj_id;
    m_pAuto->AlarmAuto(static_cast<cnAuto::state_e>(err));
  }
  return ret;
}


int cnDo::CylOpen(uint8_t obj_id, bool skip_sensor )
{
  int ret = 0;
  ret = m_pCyl[obj_id].Open(skip_sensor);
  if (ret != 0)
  {
    uint8_t err = static_cast<uint8_t>(cnAuto::state_e::cyl_0_open_timeout)+obj_id;
    m_pAuto->AlarmAuto(static_cast<cnAuto::state_e>(err));
  }
  return ret;
}
int cnDo::CylClose(uint8_t obj_id, bool skip_sensor )
{
  int ret = 0;
  ret = m_pCyl[obj_id].Close(skip_sensor);
  if (ret != 0)
  {
    uint8_t err = static_cast<uint8_t>(cnAuto::state_e::cyl_0_close_timeout)+obj_id;
    m_pAuto->AlarmAuto(static_cast<cnAuto::state_e>(err));
  }
  return ret;
}

bool cnDo::IsGripPhone(uint8_t obj_id)
{
  bool ret = false;
  ret |= m_pCyl[obj_id].IsOpen();
  ret |= m_pCyl[obj_id].IsClose();
  return !(ret);
}

int cnDo::GripPhone(uint8_t obj_id)
{
  int ret = 0;
  ret = m_pCyl[obj_id].Close(true);
  uint32_t pre_ms = millis();
  while (!IsGripPhone(obj_id))
  {
    if ( millis()-pre_ms >CN_DO_WAIT_TIMEOUT)
    {
      uint8_t err = static_cast<uint8_t>(cnAuto::state_e::cyl_0_close_timeout)+obj_id;
      m_pAuto->AlarmAuto(static_cast<cnAuto::state_e>(err));
      ret = (int)err;
      break;
    }
  }
  return ret;
}

bool cnDo::IsMotorOn()
{
  return m_pFm->IsMotorOn();
}

bool cnDo::IsMotorRun()
{
  return m_pFm->IsBusy();
}

bool cnDo::IsMotorMoveCplt()
{
  return m_pFm->IsAxisDone();
}

void cnDo::MotorOnOff(bool on_off)
{
  uint32_t pre_ms = millis();
  while (m_step.GetStep() != CN_DO_STEP_TODO)
  {
    if ( millis() - pre_ms > CN_DO_WAIT_TIMEOUT)
      break;
  }

  if (on_off)
    m_step.SetStep(CN_DO_STEP_MOTOR_ON);
  else
    m_step.SetStep(CN_DO_STEP_MOTOR_OFF);
  m_pre_time = millis();
}

void cnDo::MotorStop()
{
  uint32_t pre_ms = millis();
  while (m_step.GetStep() != CN_DO_STEP_TODO)
  {
    if ( millis() - pre_ms > CN_DO_WAIT_TIMEOUT)
      break;
  }
  m_step.SetStep(CN_DO_STEP_MOTOR_STOP);
  m_pre_time = millis();
}


void cnDo::MotorOrigin()
{
  if (!m_pFm->IsMotorOn())
    return ;

  uint32_t pre_ms = millis();
  while (m_step.GetStep() != CN_DO_STEP_TODO)
  {
    if ( millis() - pre_ms > CN_DO_WAIT_TIMEOUT)
      break;
  }

  m_step.SetStep(CN_DO_STEP_POS_ORIGIN);
  m_pre_time = millis();
}


void cnDo::MotorRun(int cmd_pos, uint32_t cmd_vel, uint32_t acc, uint32_t decel)
{
  uint32_t pre_ms = millis();
  while (m_step.GetStep() != CN_DO_STEP_TODO)
  {
    if ( millis() - pre_ms > CN_DO_WAIT_TIMEOUT)
      break;
  }
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
  m_step.SetStep(CN_DO_STEP_MOTOR_MOVE);
}


void cnDo::RelMove(int cmd_dist, uint32_t cmd_vel)
{
  uint32_t pre_ms = millis();
  while (m_step.GetStep() != CN_DO_STEP_TODO)
  {
    if ( millis() - pre_ms > CN_DO_WAIT_TIMEOUT)
      break;
  }

  m_stepBuffer[0] = (uint8_t)(cmd_dist >> 0);
  m_stepBuffer[1] = (uint8_t)(cmd_dist >> 8);
  m_stepBuffer[2] = (uint8_t)(cmd_dist >> 16);
  m_stepBuffer[3] = (uint8_t)(cmd_dist >> 24);

  m_stepBuffer[4] = (uint8_t)(cmd_vel >> 0);
  m_stepBuffer[5] = (uint8_t)(cmd_vel >> 8);
  m_stepBuffer[6] = (uint8_t)(cmd_vel >> 16);
  m_stepBuffer[7] = (uint8_t)(cmd_vel >> 24);

  m_pre_time = millis();
  m_step.SetStep(CN_DO_STEP_REL_MOVE);

}


void cnDo::WriteMotorData(uint8_t *p_data, uint8_t length)
{
  uint32_t pre_ms = millis();
  while (m_step.GetStep() != CN_DO_STEP_TODO)
  {
    if ( millis() - pre_ms > CN_DO_WAIT_TIMEOUT)
      break;
  }

  memcpy(&m_stepBuffer[0], p_data, length);
  if(m_step.GetStep() == CN_DO_STEP_TODO)
  {
    m_step.SetStep(CN_DO_STEP_WRITE_MOTOR_DATA);
    m_pre_time = millis();
  }
}
void cnDo::WriteApCfgData(uint8_t *p_data, uint8_t length)
{
  uint32_t pre_ms = millis();
  while (m_step.GetStep() != CN_DO_STEP_TODO)
  {
    if ( millis() - pre_ms > CN_DO_WAIT_TIMEOUT)
      break;
  }
  memcpy(&m_stepBuffer[0], p_data, length);
  if(m_step.GetStep() == CN_DO_STEP_TODO)
  {
    m_step.SetStep(CN_DO_STEP_WRITE_AP_CFG);
    m_pre_time = millis();
  }
}
void cnDo::WriteCylData(uint8_t *p_data, uint8_t length)
{
  uint32_t pre_ms = millis();
  while (m_step.GetStep() != CN_DO_STEP_TODO)
  {
    if ( millis() - pre_ms > CN_DO_WAIT_TIMEOUT)
      break;
  }
  memcpy(&m_stepBuffer[0], p_data, length);
  if(m_step.GetStep() == CN_DO_STEP_TODO)
  {
    m_step.SetStep(CN_DO_STEP_WRITE_CYL_DATA);
    m_pre_time = millis();
  }
}
void cnDo::WriteVacData(uint8_t *p_data, uint8_t length)
{
  uint32_t pre_ms = millis();
  while (m_step.GetStep() != CN_DO_STEP_TODO)
  {
    if ( millis() - pre_ms > CN_DO_WAIT_TIMEOUT)
      break;
  }
  memcpy(&m_stepBuffer[0], p_data, length);
  if(m_step.GetStep() == CN_DO_STEP_TODO)
  {
    m_step.SetStep(CN_DO_STEP_WRITE_VAC_DATA);
    m_pre_time = millis();
  }
}

void cnDo::WriteSeqData(uint8_t *p_data, uint8_t length)
{
  uint32_t pre_ms = millis();
  while (m_step.GetStep() != CN_DO_STEP_TODO)
  {
    if ( millis() - pre_ms > CN_DO_WAIT_TIMEOUT)
      break;
  }
  memcpy(&m_stepBuffer[0], p_data, length);
  if(m_step.GetStep() == CN_DO_STEP_TODO)
  {
    m_step.SetStep(CN_DO_STEP_WRITE_SEQ_DATA);
    m_pre_time = millis();
  }
}









