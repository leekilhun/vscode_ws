/*
 * uiRemoteCtrl.cpp
 *
 *  Created on: 2022. 3. 6.
 *      Author: gns2l
 */

#include "ap_def.h"
#include <ap_ui/uiRemoteCtrl.h>


#define UI_REMOTECTRL_CMD_STX                     0x96
#define UI_REMOTECTRL_CMD_ETX                     0x69

#define UI_REMOTECTRL_CMD_READ_ALL_STATE          0x00
#define UI_REMOTECTRL_CMD_READ_BOOT_INFO          0x01
#define UI_REMOTECTRL_CMD_READ_FIRM_INFO          0x02
#define UI_REMOTECTRL_CMD_CONTROL_IO_OUT          0x10
#define UI_REMOTECTRL_CMD_CONTROL_MOT_ORIGIN      0x11
#define UI_REMOTECTRL_CMD_CONTROL_MOT_ONOFF       0x12
#define UI_REMOTECTRL_CMD_CONTROL_MOT_RUN         0x13
#define UI_REMOTECTRL_CMD_CONTROL_MOT_STOP        0x14
#define UI_REMOTECTRL_CMD_AP_CONFIG_WRITE         0x20


#define UI_REMOTECTRL_ERR_WRONG_CMD               0x01
#define UI_REMOTECTRL_ERR_IO_OUT                  0x02
#define UI_REMOTECTRL_ERR_MOT_ORIGIN              0x03
#define UI_REMOTECTRL_ERR_BUF_OVF                 0x06
#define UI_REMOTECTRL_ERR_INVALID_FW              0x07
#define UI_REMOTECTRL_ERR_FW_CRC                  0x08



#define UI_REMOTECTRL_STEP_RETRY_CNT_MAX                   3

#define UI_REMOTECTRL_STEP_INIT                            0
#define UI_REMOTECTRL_STEP_TODO                            1
#define UI_REMOTECTRL_STEP_TIMEOUT                         2
#define UI_REMOTECTRL_STEP_SEND_MCU_ALL_DATA               3
#define UI_REMOTECTRL_STEP_SEND_MCU_ALL_DATA_START         4
#define UI_REMOTECTRL_STEP_SEND_MCU_ALL_DATA_WAIT          5
#define UI_REMOTECTRL_STEP_SEND_MCU_ALL_DATA_END           6
#define UI_REMOTECTRL_STEP_CTRL_IO_OUT                     7
#define UI_REMOTECTRL_STEP_CTRL_IO_OUT_START               8
#define UI_REMOTECTRL_STEP_CTRL_IO_OUT_WAIT                9
#define UI_REMOTECTRL_STEP_CTRL_IO_OUT_END                 10
#define UI_REMOTECTRL_STEP_CTRL_AXIS_ONOFF                 11
#define UI_REMOTECTRL_STEP_CTRL_AXIS_ONOFF_START           12
#define UI_REMOTECTRL_STEP_CTRL_AXIS_ONOFF_WAIT            13
#define UI_REMOTECTRL_STEP_CTRL_AXIS_ONOFF_END             14
#define UI_REMOTECTRL_STEP_CTRL_AXIS_MOVE                  15
#define UI_REMOTECTRL_STEP_CTRL_AXIS_MOVE_START            16
#define UI_REMOTECTRL_STEP_CTRL_AXIS_MOVE_WAIT             17
#define UI_REMOTECTRL_STEP_CTRL_AXIS_MOVE_END              18
#define UI_REMOTECTRL_STEP_CTRL_AXIS_RUN                   19
#define UI_REMOTECTRL_STEP_CTRL_AXIS_RUN_START             20
#define UI_REMOTECTRL_STEP_CTRL_AXIS_RUN_WAIT              21
#define UI_REMOTECTRL_STEP_CTRL_AXIS_RUN_END               22
#define UI_REMOTECTRL_STEP_CTRL_AXIS_STOP                  23
#define UI_REMOTECTRL_STEP_CTRL_AXIS_STOP_START            24
#define UI_REMOTECTRL_STEP_CTRL_AXIS_STOP_WAIT             25
#define UI_REMOTECTRL_STEP_CTRL_AXIS_STOP_END              26
#define UI_REMOTECTRL_STEP_CTRL_AP_CONF_WRITE              27
#define UI_REMOTECTRL_STEP_CTRL_AP_CONF_WRITE_START        28
#define UI_REMOTECTRL_STEP_CTRL_AP_CONF_WRITE_WAIT         29
#define UI_REMOTECTRL_STEP_CTRL_AP_CONF_WRITE_END          30


uiRemoteCtrl::uiRemoteCtrl()
{
  // TODO Auto-generated constructor stub
  m_pApReg = NULL;
  m_pApIo = NULL;
  m_pApAxisDat = NULL;
  m_pApCylDat = NULL;
  m_pApVacDat = NULL;
  m_pApCfgDat = NULL;
  m_pApSeqDat = NULL;
  m_pFm = NULL;
  // m_pAp= NULL;
  // m_pAuto= NULL;
  m_pLog = NULL;
  m_pre_time = 0;
  m_step = {};
  m_IsRespCplt = true;
  m_retryCnt = 0;
}

uiRemoteCtrl::~uiRemoteCtrl()
{
  // TODO Auto-generated destructor stub
}

int uiRemoteCtrl::Init(uiRemoteCtrl::cfg_t *cfg)
{
  m_pApReg = cfg->p_apReg;
  m_pApIo = cfg->p_apIo;
  m_pApAxisDat = cfg->p_apAxisDat;
  m_pApCylDat = cfg->p_apCylDat;
  m_pApVacDat = cfg->p_apVacDat;
  m_pApCfgDat = cfg->p_apCfgDat;
  m_pApSeqDat = cfg->p_apSeqDat;
  m_pFm = cfg->p_Fm;

  m_pAp = cfg->p_Ap;
  m_pAuto = cfg->p_Auto;

  m_pLog = cfg->p_Log;
  cmdRobotro_Init(&m_Packet);
  cmdRobotro_Open(&m_Packet, cfg->ch, cfg->baud /*115200*/);

  return 0;
}
void uiRemoteCtrl::ProcessCmd()
{
  switch (m_Packet.rx_packet.cmd_type)
  {
  case UI_REMOTECTRL_CMD_READ_ALL_STATE:
    if (m_step.GetStep() == UI_REMOTECTRL_STEP_TODO)
    {
      m_pre_time = millis();
      m_step.SetStep(UI_REMOTECTRL_STEP_SEND_MCU_ALL_DATA);
    }
    break;

  case UI_REMOTECTRL_CMD_READ_BOOT_INFO:
    break;

  case UI_REMOTECTRL_CMD_READ_FIRM_INFO:
    break;

  case UI_REMOTECTRL_CMD_CONTROL_IO_OUT:
    if (m_step.GetStep() == UI_REMOTECTRL_STEP_TODO)
    {
      m_pre_time = millis();
      m_step.SetStep(UI_REMOTECTRL_STEP_CTRL_IO_OUT);
    }
    break;

  case UI_REMOTECTRL_CMD_CONTROL_MOT_ORIGIN:
    break;

  case UI_REMOTECTRL_CMD_CONTROL_MOT_ONOFF:
    if (m_step.GetStep() == UI_REMOTECTRL_STEP_TODO)
    {
      m_pre_time = millis();
      m_step.SetStep(UI_REMOTECTRL_STEP_CTRL_AXIS_ONOFF);
    }
    break;

  case UI_REMOTECTRL_CMD_CONTROL_MOT_RUN:
    if (m_step.GetStep() == UI_REMOTECTRL_STEP_TODO)
    {
      m_pre_time = millis();
      m_step.SetStep(UI_REMOTECTRL_STEP_CTRL_AXIS_RUN);
    }
    break;

  case UI_REMOTECTRL_CMD_CONTROL_MOT_STOP:
    if (m_step.GetStep() == UI_REMOTECTRL_STEP_TODO)
    {
      m_pre_time = millis();
      m_step.SetStep(UI_REMOTECTRL_STEP_CTRL_AXIS_STOP);
    }
    break;

  case UI_REMOTECTRL_CMD_AP_CONFIG_WRITE:
    if (m_step.GetStep() == UI_REMOTECTRL_STEP_TODO)
    {
      m_pre_time = millis();
      m_step.SetStep(UI_REMOTECTRL_STEP_CTRL_AP_CONF_WRITE);
    }
    break;

  default:
    // cmdRobotro_SendCmd(m_pCmd, m_pCmd->rx_packet.cmd_type, UI_REMOTECTRL_ERR_WRONG_CMD, NULL);
    break;
  }
}

void uiRemoteCtrl::SendAllState()
{
  memset(&m_stepBuffer[0], 0, UI_REMOTECTRL_MAX_BUFFER_LENGTH);

  m_stepBuffer[0] = m_pApReg->status[(const int)AP_REG_BANK_ERR_H];
  m_stepBuffer[1] = m_pApReg->status[(const int)AP_REG_BANK_ERR_L];
  m_stepBuffer[2] = m_pApReg->status[(const int)AP_REG_BANK_SETTING];
  m_stepBuffer[3] = m_pApReg->status[(const int)AP_REG_BANK_RUN_STATE];

  m_stepBuffer[4] = m_pApIo->GetBank_In(AP_IO_DEF_BANK_NO_0);
  m_stepBuffer[5] = m_pApIo->GetBank_In(AP_IO_DEF_BANK_NO_1);
  m_stepBuffer[6] = m_pApIo->GetBank_In(AP_IO_DEF_BANK_NO_2);
  m_stepBuffer[7] = m_pApIo->GetBank_In(AP_IO_DEF_BANK_NO_3);

  m_stepBuffer[8] = m_pApIo->GetBank_Out(AP_IO_DEF_BANK_NO_0);
  m_stepBuffer[9] = m_pApIo->GetBank_Out(AP_IO_DEF_BANK_NO_1);
  m_stepBuffer[10] = m_pApIo->GetBank_Out(AP_IO_DEF_BANK_NO_2);
  m_stepBuffer[11] = m_pApIo->GetBank_Out(AP_IO_DEF_BANK_NO_3);

  m_stepBuffer[12] = m_pFm->m_AxisState.axis_status[(const int)DEF_FM_BANK_ERR_01];
  m_stepBuffer[13] = m_pFm->m_AxisState.axis_status[(const int)DEF_FM_BANK_ERR_02];
  m_stepBuffer[14] = m_pFm->m_AxisState.axis_status[(const int)DEF_FM_BANK_STATE_01];
  m_stepBuffer[15] = m_pFm->m_AxisState.axis_status[(const int)DEF_FM_BANK_STATE_02];

  m_stepBuffer[16] = (uint8_t)(m_pFm->m_AxisState.cmd_pos >> 0);
  m_stepBuffer[17] = (uint8_t)(m_pFm->m_AxisState.cmd_pos >> 8);
  m_stepBuffer[18] = (uint8_t)(m_pFm->m_AxisState.cmd_pos >> 16);
  m_stepBuffer[19] = (uint8_t)(m_pFm->m_AxisState.cmd_pos >> 24);

  m_stepBuffer[20] = (uint8_t)(m_pFm->m_AxisState.act_pos >> 0);
  m_stepBuffer[21] = (uint8_t)(m_pFm->m_AxisState.act_pos >> 8);
  m_stepBuffer[22] = (uint8_t)(m_pFm->m_AxisState.act_pos >> 16);
  m_stepBuffer[23] = (uint8_t)(m_pFm->m_AxisState.act_pos >> 24);

  m_stepBuffer[24] = (uint8_t)(m_pFm->m_AxisState.act_vel >> 0);
  m_stepBuffer[25] = (uint8_t)(m_pFm->m_AxisState.act_vel >> 8);
  m_stepBuffer[26] = (uint8_t)(m_pFm->m_AxisState.act_vel >> 16);
  m_stepBuffer[27] = (uint8_t)(m_pFm->m_AxisState.act_vel >> 24);

  m_stepBuffer[28] = (uint8_t)(m_pFm->m_AxisState.pos_err >> 0);
  m_stepBuffer[29] = (uint8_t)(m_pFm->m_AxisState.pos_err >> 8);
  m_stepBuffer[30] = (uint8_t)(m_pFm->m_AxisState.pos_err >> 16);
  m_stepBuffer[31] = (uint8_t)(m_pFm->m_AxisState.pos_err >> 24);

  cmdRobotro_SendCmd(&m_Packet, UI_REMOTECTRL_CMD_READ_ALL_STATE, &m_stepBuffer[0], UI_REMOTECTRL_MAX_BUFFER_LENGTH);
}

void uiRemoteCtrl::CommRecovery()
{
  uartClose(m_Packet.ch);
  uartOpen(m_Packet.ch, m_Packet.baud);
}

bool uiRemoteCtrl::ThreadJob()
{
  if (cmdRobotro_ReceivePacket(&m_Packet) == true)
  {
    m_IsRespCplt = true;
    ProcessCmd();
  }

  doRunStep();
  return m_IsRespCplt;
}

void uiRemoteCtrl::doRunStep()
{

  if (millis() - m_pre_time > 1000 * 3)
  {
    m_step.SetStep(UI_REMOTECTRL_STEP_INIT);
    CommRecovery();
    m_pre_time = millis();
  }

  switch (m_step.GetStep())
  {
  case UI_REMOTECTRL_STEP_INIT:
  {
    m_step.SetStep(UI_REMOTECTRL_STEP_TODO);
    m_pre_time = millis();
  }
  break;
  /*######################################################
     to do
    ######################################################*/
  case UI_REMOTECTRL_STEP_TODO:
  {
    m_IsRespCplt = true;
    m_retryCnt = 0;
  }
  break;
  /*######################################################
     timeout/ page check
    ######################################################*/
  case UI_REMOTECTRL_STEP_TIMEOUT:
  {
    if (millis() - m_pre_time < 5)
    {
      break;
    }
    m_retryCnt = 0;
    m_step.SetStep(UI_REMOTECTRL_STEP_TODO);
  }
  break;
  /*######################################################
     UI_REMOTECTRL_STEP_SEND_MCU_ALL_DATA
    ######################################################*/
  case UI_REMOTECTRL_STEP_SEND_MCU_ALL_DATA:
  {
    m_retryCnt = 0;
    m_step.SetStep(UI_REMOTECTRL_STEP_SEND_MCU_ALL_DATA_START);
    m_pre_time = millis();
  }
  break;
  case UI_REMOTECTRL_STEP_SEND_MCU_ALL_DATA_START:
  {
    /* send all state*/
    SendAllState();

    m_IsRespCplt = false;
    m_step.SetStep(UI_REMOTECTRL_STEP_SEND_MCU_ALL_DATA_WAIT);
    m_pre_time = millis();
  }
  break;
  case UI_REMOTECTRL_STEP_SEND_MCU_ALL_DATA_WAIT:
  {
    if (millis() - m_pre_time >= 50)
    {
      if (m_retryCnt++ < UI_REMOTECTRL_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(UI_REMOTECTRL_STEP_SEND_MCU_ALL_DATA_START);
        m_pre_time = millis();
      }
      else
      {
        m_retryCnt = 0;
        m_step.SetStep(UI_REMOTECTRL_STEP_TIMEOUT);
        m_pre_time = millis();
      }
    }

    if (m_IsRespCplt)
    {
      m_step.SetStep(UI_REMOTECTRL_STEP_SEND_MCU_ALL_DATA_END);
      m_pre_time = millis();
    }
  }
  break;
  case UI_REMOTECTRL_STEP_SEND_MCU_ALL_DATA_END:
  {
    m_step.SetStep(UI_REMOTECTRL_STEP_TODO);
    m_pre_time = millis();
  }
  break;
  /*######################################################
     UI_REMOTECTRL_STEP_CTRL_IO_OUT
    ######################################################*/
  case UI_REMOTECTRL_STEP_CTRL_IO_OUT:
  {
    m_retryCnt = 0;
    m_step.SetStep(UI_REMOTECTRL_STEP_CTRL_IO_OUT_START);
    m_pre_time = millis();
  }
  break;
  case UI_REMOTECTRL_STEP_CTRL_IO_OUT_START:
  {
    /* io control*/
    if (m_Packet.rx_packet.length == 4)
    {
      m_pApIo->SetBank_Out(AP_IO_DEF_BANK_NO_0, m_Packet.rx_packet.data[0]);
      m_pApIo->SetBank_Out(AP_IO_DEF_BANK_NO_1, m_Packet.rx_packet.data[1]);
      m_pApIo->SetBank_Out(AP_IO_DEF_BANK_NO_2, m_Packet.rx_packet.data[2]);
      m_pApIo->SetBank_Out(AP_IO_DEF_BANK_NO_3, m_Packet.rx_packet.data[3]);
    }

    m_IsRespCplt = false;
    m_step.SetStep(UI_REMOTECTRL_STEP_CTRL_IO_OUT_WAIT);
    m_pre_time = millis();
  }
  break;
  case UI_REMOTECTRL_STEP_CTRL_IO_OUT_WAIT:
  {
    if (millis() - m_pre_time >= 50)
    {
      if (m_retryCnt++ < UI_REMOTECTRL_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(UI_REMOTECTRL_STEP_CTRL_IO_OUT_START);
        m_pre_time = millis();
      }
      else
      {
        m_retryCnt = 0;
        m_step.SetStep(UI_REMOTECTRL_STEP_TIMEOUT);
        m_pre_time = millis();
      }
    }
    if (m_IsRespCplt)
    {
      m_step.SetStep(UI_REMOTECTRL_STEP_CTRL_IO_OUT_END);
      m_pre_time = millis();
    }
  }
  break;
  case UI_REMOTECTRL_STEP_CTRL_IO_OUT_END:
  {
    m_step.SetStep(UI_REMOTECTRL_STEP_TODO);
    m_pre_time = millis();
  }
  break;
  /*######################################################
     UI_REMOTECTRL_STEP_CTRL_AXIS_ONOFF
    ######################################################*/
  case UI_REMOTECTRL_STEP_CTRL_AXIS_ONOFF:
  {
    m_retryCnt = 0;
    m_step.SetStep(UI_REMOTECTRL_STEP_CTRL_AXIS_ONOFF_START);
    m_pre_time = millis();
  }
  break;
  case UI_REMOTECTRL_STEP_CTRL_AXIS_ONOFF_START:
  {
    /* io control*/
    if (m_Packet.rx_packet.length == 1)
    {
      m_pAp->MotorOnOff((bool)m_Packet.rx_packet.data[0]);
    }

    m_IsRespCplt = false;
    m_step.SetStep(UI_REMOTECTRL_STEP_CTRL_AXIS_ONOFF_WAIT);
    m_pre_time = millis();
  }
  break;
  case UI_REMOTECTRL_STEP_CTRL_AXIS_ONOFF_WAIT:
  {
    if (millis() - m_pre_time >= 50)
    {
      if (m_retryCnt++ < UI_REMOTECTRL_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(UI_REMOTECTRL_STEP_CTRL_AXIS_ONOFF_START);
        m_pre_time = millis();
      }
      else
      {
        m_retryCnt = 0;
        m_step.SetStep(UI_REMOTECTRL_STEP_TIMEOUT);
        m_pre_time = millis();
      }
    }
    if (m_IsRespCplt)
    {
      m_step.SetStep(UI_REMOTECTRL_STEP_CTRL_AXIS_ONOFF_END);
      m_pre_time = millis();
    }
  }
  break;
  case UI_REMOTECTRL_STEP_CTRL_AXIS_ONOFF_END:
  {
    m_step.SetStep(UI_REMOTECTRL_STEP_TODO);
    m_pre_time = millis();
  }
  break;
  /*######################################################
     UI_REMOTECTRL_STEP_CTRL_AXIS_RUN
    ######################################################*/
  case UI_REMOTECTRL_STEP_CTRL_AXIS_RUN:
  {
    m_retryCnt = 0;
    m_step.SetStep(UI_REMOTECTRL_STEP_CTRL_AXIS_RUN_START);
    m_pre_time = millis();
  }
  break;
  case UI_REMOTECTRL_STEP_CTRL_AXIS_RUN_START:
  {
    /* io control*/
    if (m_Packet.rx_packet.length == 16)
    {
      int cmd_pos = 0;
      cmd_pos = utilDwToInt(&m_Packet.rx_packet.data[0]);
      uint32_t cmd_vel = 0;
      cmd_vel = utilDwToUint(&m_Packet.rx_packet.data[4]);
      uint32_t acc = 0;
      acc = utilDwToUint(&m_Packet.rx_packet.data[8]);
      uint32_t decel = 0;
      decel = utilDwToUint(&m_Packet.rx_packet.data[12]);

      m_pAp->MotorRun(cmd_pos, cmd_vel, acc, decel);
    }

    m_IsRespCplt = false;
    m_step.SetStep(UI_REMOTECTRL_STEP_CTRL_AXIS_RUN_WAIT);
    m_pre_time = millis();
  }
  break;
  case UI_REMOTECTRL_STEP_CTRL_AXIS_RUN_WAIT:
  {
    if (millis() - m_pre_time >= 50)
    {
      if (m_retryCnt++ < UI_REMOTECTRL_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(UI_REMOTECTRL_STEP_CTRL_AXIS_RUN_START);
        m_pre_time = millis();
      }
      else
      {
        m_retryCnt = 0;
        m_pre_time = millis();
        m_step.SetStep(UI_REMOTECTRL_STEP_TIMEOUT);
      }
    }
    if (m_IsRespCplt)
    {
      m_step.SetStep(UI_REMOTECTRL_STEP_CTRL_AXIS_RUN_END);
      m_pre_time = millis();
    }
  }
  break;
  case UI_REMOTECTRL_STEP_CTRL_AXIS_RUN_END:
  {
    m_step.SetStep(UI_REMOTECTRL_STEP_TODO);
    m_pre_time = millis();
  }
  break;

  /*######################################################
     UI_REMOTECTRL_STEP_CTRL_AXIS_STOP
    ######################################################*/
  case UI_REMOTECTRL_STEP_CTRL_AXIS_STOP:
  {
    m_pre_time = millis();
    m_retryCnt = 0;
    m_step.SetStep(UI_REMOTECTRL_STEP_CTRL_AXIS_STOP_START);
  }
  break;
  case UI_REMOTECTRL_STEP_CTRL_AXIS_STOP_START:
  {
    /* io control*/
    m_pAp->MotorStop();

    m_IsRespCplt = false;
    m_step.SetStep(UI_REMOTECTRL_STEP_CTRL_AXIS_STOP_WAIT);
    m_pre_time = millis();
  }
  break;
  case UI_REMOTECTRL_STEP_CTRL_AXIS_STOP_WAIT:
  {
    if (millis() - m_pre_time >= 50)
    {
      if (m_retryCnt++ < UI_REMOTECTRL_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(UI_REMOTECTRL_STEP_CTRL_AXIS_STOP_START);
        m_pre_time = millis();
      }
      else
      {
        m_retryCnt = 0;
        m_pre_time = millis();
        m_step.SetStep(UI_REMOTECTRL_STEP_TIMEOUT);
      }
    }
    if (m_IsRespCplt)
    {
      m_step.SetStep(UI_REMOTECTRL_STEP_TIMEOUT);
      m_pre_time = millis();
    }
  }
  break;
  case UI_REMOTECTRL_STEP_CTRL_AXIS_STOP_END:
  {
    m_step.SetStep(UI_REMOTECTRL_STEP_TODO);
    m_pre_time = millis();
  }
  break;

  /*######################################################
     UI_REMOTECTRL_STEP_CTRL_AP_CONF_WRITE
    ######################################################*/
  case UI_REMOTECTRL_STEP_CTRL_AP_CONF_WRITE:
  {
    m_retryCnt = 0;
    m_step.SetStep(UI_REMOTECTRL_STEP_CTRL_AP_CONF_WRITE_START);
    m_pre_time = millis();
  }
  break;
  case UI_REMOTECTRL_STEP_CTRL_AP_CONF_WRITE_START:
  {
    /* ap config write */
    if (m_Packet.rx_packet.length == 1)
    {
      m_pApReg->SetConfReg(m_Packet.rx_packet.data[0]);
    }

    m_IsRespCplt = false;
    m_step.SetStep(UI_REMOTECTRL_STEP_CTRL_AP_CONF_WRITE_WAIT);
    m_pre_time = millis();
  }
  break;
  case UI_REMOTECTRL_STEP_CTRL_AP_CONF_WRITE_WAIT:
  {
    if (millis() - m_pre_time >= 50)
    {
      if (m_retryCnt++ < UI_REMOTECTRL_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(UI_REMOTECTRL_STEP_CTRL_AP_CONF_WRITE_START);
        m_pre_time = millis();
      }
      else
      {
        m_retryCnt = 0;
        m_pre_time = millis();
        m_step.SetStep(UI_REMOTECTRL_STEP_TIMEOUT);
      }
    }
    if (m_IsRespCplt)
    {
      m_step.SetStep(UI_REMOTECTRL_STEP_CTRL_AP_CONF_WRITE_END);
      m_pre_time = millis();
    }
  }
  break;
  case UI_REMOTECTRL_STEP_CTRL_AP_CONF_WRITE_END:
  {
    m_step.SetStep(UI_REMOTECTRL_STEP_TODO);
    m_pre_time = millis();
  }
  break;

  default:
    break;
  } // switch
}
