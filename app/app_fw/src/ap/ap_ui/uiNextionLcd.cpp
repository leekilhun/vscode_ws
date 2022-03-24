/*
 * uiNextionLcd.cpp
 *
 *  Created on: 2022. 3. 4.
 *      Author: gns2l
 */

#include "ap_def.h"
#include <ap_ui/uiNextionLcd.h>

#define UI_NXLCD_COMM_ERROR_CNT_MAX                   10

#define UI_NXLCD_RXCMD_MOT_CTRL_B0_MOTOR_ON           B00000001
#define UI_NXLCD_RXCMD_MOT_CTRL_B0_MOTOR_OFF          B00000010
#define UI_NXLCD_RXCMD_MOT_CTRL_B0_RESERVED2          B00000100
#define UI_NXLCD_RXCMD_MOT_CTRL_B0_STOP               B00001000
#define UI_NXLCD_RXCMD_MOT_CTRL_B0_START              B00010000
#define UI_NXLCD_RXCMD_MOT_CTRL_B0_RESERVED5          B00100000
#define UI_NXLCD_RXCMD_MOT_CTRL_B0_JOG_CW             B01000000
#define UI_NXLCD_RXCMD_MOT_CTRL_B0_JOG_CCW            B10000000

#define UI_NXLCD_RXCMD_MOT_CTRL_B1_ENCODE_RST         B00000001
#define UI_NXLCD_RXCMD_MOT_CTRL_B1_RESERVED1          B00000010
#define UI_NXLCD_RXCMD_MOT_CTRL_B1_RESERVED2          B00000100
#define UI_NXLCD_RXCMD_MOT_CTRL_B1_RESERVED3          B00001000
#define UI_NXLCD_RXCMD_MOT_CTRL_B1_RESERVED4          B00010000
#define UI_NXLCD_RXCMD_MOT_CTRL_B1_RESERVED5          B00100000
#define UI_NXLCD_RXCMD_MOT_CTRL_B1_RESERVED6          B01000000
#define UI_NXLCD_RXCMD_MOT_CTRL_B1_RESERVED7          B10000000

#define UI_NXLCD_RXCMD_LCD_SW_KEY_EVENT_READY         B00000001
#define UI_NXLCD_RXCMD_LCD_SW_KEY_EVENT_START         B00000010
#define UI_NXLCD_RXCMD_LCD_SW_KEY_EVENT_STOP          B00000100
#define UI_NXLCD_RXCMD_LCD_SW_KEY_EVENT_RESET         B00001000
#define UI_NXLCD_RXCMD_LCD_SW_KEY_EVENT_RESERVED4     B00010000
#define UI_NXLCD_RXCMD_LCD_SW_KEY_EVENT_RESERVED5     B00100000
#define UI_NXLCD_RXCMD_LCD_SW_KEY_EVENT_RESERVED6     B01000000
#define UI_NXLCD_RXCMD_LCD_SW_KEY_EVENT_RESERVED7     B10000000

#define UI_NEXTION_STEP_RETRY_CNT_MAX                 3

#define UI_NEXTION_STEP_INIT                          0
#define UI_NEXTION_STEP_TODO                          1
#define UI_NEXTION_STEP_TIMEOUT                       2
#define UI_NEXTION_STEP_DATA_SEND_MOTOR_POS           3
#define UI_NEXTION_STEP_DATA_SEND_MOTOR_POS_START     4
#define UI_NEXTION_STEP_DATA_SEND_MOTOR_POS_WAIT      5
#define UI_NEXTION_STEP_DATA_SEND_MOTOR_POS_END       6
#define UI_NEXTION_STEP_DATA_SEND_AP_DAT              7
#define UI_NEXTION_STEP_DATA_SEND_AP_DAT_START        8
#define UI_NEXTION_STEP_DATA_SEND_AP_DAT_WAIT         9
#define UI_NEXTION_STEP_DATA_SEND_AP_DAT_END          10
#define UI_NEXTION_STEP_DATA_SEND_CYL_DAT             11
#define UI_NEXTION_STEP_DATA_SEND_CYL_DAT_START       12
#define UI_NEXTION_STEP_DATA_SEND_CYL_DAT_WAIT        13
#define UI_NEXTION_STEP_DATA_SEND_CYL_DAT_END         14
#define UI_NEXTION_STEP_DATA_SEND_VAC_DAT             15
#define UI_NEXTION_STEP_DATA_SEND_VAC_DAT_START       16
#define UI_NEXTION_STEP_DATA_SEND_VAC_DAT_WAIT        17
#define UI_NEXTION_STEP_DATA_SEND_VAC_DAT_END         18
#define UI_NEXTION_STEP_DATA_SEND_SEQ_DAT             19
#define UI_NEXTION_STEP_DATA_SEND_SEQ_DAT_START       20
#define UI_NEXTION_STEP_DATA_SEND_SEQ_DAT_WAIT        21
#define UI_NEXTION_STEP_DATA_SEND_SEQ_DAT_END         22
#define UI_NEXTION_STEP_DATA_SEND_ALL                 23
#define UI_NEXTION_STEP_DATA_SEND_ALL_START           24
#define UI_NEXTION_STEP_DATA_SEND_ALL_WAIT            25
#define UI_NEXTION_STEP_DATA_SEND_ALL_END             26
#define UI_NEXTION_STEP_DATA_SEND_IO_REG              27
#define UI_NEXTION_STEP_DATA_SEND_IO_REG_START        28
#define UI_NEXTION_STEP_DATA_SEND_IO_REG_WAIT         29
#define UI_NEXTION_STEP_DATA_SEND_IO_REG_END          30
#define UI_NEXTION_STEP_DATA_SEND_AP_REG              31
#define UI_NEXTION_STEP_DATA_SEND_AP_REG_START        32
#define UI_NEXTION_STEP_DATA_SEND_AP_REG_WAIT         33
#define UI_NEXTION_STEP_DATA_SEND_AP_REG_END          34
#define UI_NEXTION_STEP_DATA_SEND_AXIS_DAT            35
#define UI_NEXTION_STEP_DATA_SEND_AXIS_DAT_START      36
#define UI_NEXTION_STEP_DATA_SEND_AXIS_DAT_WAIT       37
#define UI_NEXTION_STEP_DATA_SEND_AXIS_DAT_END        38
#define UI_NEXTION_STEP_DATA_SEND_LOG_DAT             39
#define UI_NEXTION_STEP_DATA_SEND_LOG_DAT_START       40
#define UI_NEXTION_STEP_DATA_SEND_LOG_DAT_WAIT        41
#define UI_NEXTION_STEP_DATA_SEND_LOG_DAT_END         42
#define UI_NEXTION_STEP_WRITE_IO_REG_OUT              43
#define UI_NEXTION_STEP_WRITE_IO_REG_OUT_START        44
#define UI_NEXTION_STEP_WRITE_IO_REG_OUT_WAIT         45
#define UI_NEXTION_STEP_WRITE_IO_REG_OUT_END          46
#define UI_NEXTION_STEP_WRITE_MOTOR_DATA              47
#define UI_NEXTION_STEP_WRITE_MOTOR_DATA_START        48
#define UI_NEXTION_STEP_WRITE_MOTOR_DATA_WAIT         49
#define UI_NEXTION_STEP_WRITE_MOTOR_DATA_END          50
#define UI_NEXTION_STEP_WRITE_AP_CFG                  51
#define UI_NEXTION_STEP_WRITE_AP_CFG_START            52
#define UI_NEXTION_STEP_WRITE_AP_CFG_WAIT             53
#define UI_NEXTION_STEP_WRITE_AP_CFG_END              54

#define UI_NEXTION_STEP_DATA_RECEIVE_CNT_RESET        99

#define UI_NEXTION_CMD_SEND_DATA_TYPE_MOTOR_POS       0x01
#define UI_NEXTION_CMD_SEND_DATA_TYPE_AP_DAT          0x02
#define UI_NEXTION_CMD_SEND_DATA_TYPE_CYL_DAT         0x03
#define UI_NEXTION_CMD_SEND_DATA_TYPE_VAC_DAT         0x04
#define UI_NEXTION_CMD_SEND_DATA_TYPE_SEQ_DAT         0x05
#define UI_NEXTION_CMD_SEND_DATA_TYPE_IO              0x06
#define UI_NEXTION_CMD_SEND_DATA_TYPE_AP_REG          0x07
#define UI_NEXTION_CMD_SEND_DATA_TYPE_AXIS_STATE      0x08
#define UI_NEXTION_CMD_SEND_DATA_TYPE_LOG_DATA AP_LOG_DAT_HEADER

// extern uint32_t check_pass_ms;

uiNextionLcd::uiNextionLcd() : m_currPage(nextionpage_t::NEX_PAGE_INIT)
{
  m_pApReg = NULL;
  m_pApIo = NULL;
  m_msgCnt = 0;
  m_pFm = NULL;
  m_pApAxisDat = NULL;
  m_pApCylDat = NULL;
  m_pApVacDat = NULL;
  m_pApCfgDat = NULL;
  m_pApSeqDat = NULL;
  m_pAp = NULL;
  m_pAuto = NULL;
  m_pLog = NULL;
  
  m_IsConnected = false;
  m_pre_time = 0;
  m_step = {};
  m_bkCmdLevel = BKCMD_ONLY_FAIL;
  m_IsRespCplt = true;
  m_retryCnt = 0;
  memset(&m_stepBuffer, 0, UI_NEXTION_MAX_BUFFER_LENGTH);
}

uiNextionLcd::~uiNextionLcd()
{
}

int uiNextionLcd::Init(uiNextionLcd::cfg_t *cfg)
{
  cmdNextion_Init(&m_Packet);
  cmdNextion_Open(&m_Packet, cfg->ch, cfg->baud /*115200*/);

  m_pApReg = cfg->p_apReg;
  m_pApIo = cfg->p_apIo;
  m_pFm = cfg->p_Fm;
  m_pApAxisDat = cfg->p_apAxisDat;
  m_pApCylDat = cfg->p_apCylDat;
  m_pApVacDat = cfg->p_apVacDat;
  m_pApCfgDat = cfg->p_apCfgDat;
  m_pApSeqDat = cfg->p_apSeqDat;
  m_pAp = cfg->p_Ap;
  m_pAuto = cfg->p_Auto;
  m_pLog = cfg->p_Log;
  m_step.SetStep(UI_NEXTION_STEP_INIT);
  m_bkCmdLevel = BKCMD_ONLY_FAIL;

  memset(&m_stepBuffer, 0, UI_NEXTION_MAX_BUFFER_LENGTH);

  //
  return 0;
}

bool uiNextionLcd::CheckCommLcdCfg()
{
  bool ret = false;

  // uint8_t level =(uint8_t)BKCMD_ALWAYS_RETURN;
  // if (cmdNextion_SendCmdRxResp(&m_Packet,NX_LCD_TXCMD_TYPE_LCD_BKCMDLEVEL,&level,1,100))
  // {
  //   m_bkCmdLevel = BKCMD_ALWAYS_RETURN;
  // }
  return ret;
}

void uiNextionLcd::SendMotorData()
{
  if (m_step.GetStep() == UI_NEXTION_STEP_TODO)
  {
    m_pre_time = millis();
    m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_MOTOR_POS);
  }
}

int uiNextionLcd::SaveMotorData()
{
  return 0;
}

void uiNextionLcd::SendAxisData()
{
  if (m_step.GetStep() == UI_NEXTION_STEP_TODO)
  {
    m_pre_time = millis();
    m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_AXIS_DAT);
  }
}

void uiNextionLcd::SendApReg()
{
  if (m_step.GetStep() == UI_NEXTION_STEP_TODO)
  {
    m_pre_time = millis();
    m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_AP_REG);
  }
}

void uiNextionLcd::SendApIoReg()
{
  if (m_step.GetStep() == UI_NEXTION_STEP_TODO)
  {
    m_pre_time = millis();
    m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_IO_REG);
  }
}

void uiNextionLcd::SendApCfgData()
{
  if (m_step.GetStep() == UI_NEXTION_STEP_TODO)
  {
    m_pre_time = millis();
    m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_AP_DAT);
  }
}

void uiNextionLcd::SendCylinderData()
{
  if (m_step.GetStep() == UI_NEXTION_STEP_TODO)
  {
    m_pre_time = millis();
    m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_CYL_DAT);
  }
}
void uiNextionLcd::SendVacuumData()
{
  if (m_step.GetStep() == UI_NEXTION_STEP_TODO)
  {
    m_pre_time = millis();
    m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_VAC_DAT);
  }
}

void uiNextionLcd::SendSequenceData()
{
  if (m_step.GetStep() == UI_NEXTION_STEP_TODO)
  {
    m_pre_time = millis();
    m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_SEQ_DAT);
  }
}

void uiNextionLcd::SendLogData()
{
  if (m_step.GetStep() == UI_NEXTION_STEP_TODO)
  {
    m_pre_time = millis();
    m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_LOG_DAT);
  }
}

void uiNextionLcd::SendDataAll()
{
  if (m_step.GetStep() == UI_NEXTION_STEP_TODO)
  {
    m_pre_time = millis();
    m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_ALL);
  }
}

void uiNextionLcd::ClearDataAll()
{
  m_pApAxisDat->ClearRomData();
  m_pApCylDat->ClearRomData();
  m_pApVacDat->ClearRomData();
  m_pApSeqDat->ClearRomData();
  m_pApCfgDat->ClearRomData();
}

int uiNextionLcd::ChangePage(nextionpage_t page)
{
  int ret = 0;
  ret = static_cast<int>(page);
  cmdNextion_SendCmd(&m_Packet,NX_LCD_TXCMD_TYPE_LCD_CHANGE_PAGE, (uint8_t*)&ret, 1);
  return ret;
}

void uiNextionLcd::CommRecovery()
{
  cmdNextion_Recovery(&m_Packet);
  cmdNextion_SendCmd(&m_Packet,NX_LCD_TXCMD_TYPE_LCD_REQUSET_PAGE, 0, 0);
}

void uiNextionLcd::ProcessCmd()
{
  // bool lock_refrash = false;

  switch (m_Packet.rx_packet.type)
  {
  case NX_LCD_RXCMD_TYPE_RESP_BKCMD:
  {
  }
  break;
  case NX_LCD_REPLY_BKCMD_OK:
  {
    if ((uint8_t)m_Packet.rx_packet.page_no <= (uint8_t)(NEX_PAGE_LOG))
    {
      m_currPage = static_cast<nextionpage_t>(m_Packet.rx_packet.page_no);
    }
    else
    {
      cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_REQUSET_PAGE, 0, 0);
    }
  }
  break;
  case NX_LCD_RXCMD_TYPE_REQ_BEEP:
  {
    if (m_pApReg->status[AP_REG_BANK_SETTING][AP_REG_USE_BEEP])
    {
      buzzerBeep(1, 5);
    }
  }
  break;
  case NX_LCD_RXCMD_TYPE_CURR_PAGE_NO:
  {
    m_currPage = static_cast<nextionpage_t>(m_Packet.rx_packet.page_no);
  }
  break;
  case NX_LCD_RXCMD_TYPE_WRITE_OUT_REG:
  {
    m_stepBuffer[0] = m_Packet.rx_packet.data[1];
    
    m_step.SetStep(UI_NEXTION_STEP_WRITE_IO_REG_OUT);
    m_pre_time = millis();
  }
  break;
  case NX_LCD_RXCMD_TYPE_UPATE_MCU_REG:
  {
    /* lcd sw key event update */

    switch (m_Packet.rx_packet.data[0])
    {
     case UI_NXLCD_RXCMD_LCD_SW_KEY_EVENT_READY:
       m_pAuto->UiStarSw();
       break;

     case UI_NXLCD_RXCMD_LCD_SW_KEY_EVENT_START:
       m_pAuto->UiStarSw();
       break;

     case UI_NXLCD_RXCMD_LCD_SW_KEY_EVENT_STOP:
       m_pApReg->SetRunState(AP_REG_AUTO_RUNNING, false);
       m_pAuto->StopSw();
       break;

     case UI_NXLCD_RXCMD_LCD_SW_KEY_EVENT_RESET:
       break;
     default:
       break;
    }
  }
  break;
  case NX_LCD_RXCMD_TYPE_CTRL_MOTOR:
  {
    switch (m_Packet.rx_packet.data[0])
    {
     case UI_NXLCD_RXCMD_MOT_CTRL_B0_MOTOR_ON:
       m_pAp->MotorOnOff(true);
       break;
     case UI_NXLCD_RXCMD_MOT_CTRL_B0_MOTOR_OFF:
       m_pAp->MotorOnOff(false);
       break;
     case UI_NXLCD_RXCMD_MOT_CTRL_B0_STOP:
       m_pAp->MotorStop();
       break;
     case UI_NXLCD_RXCMD_MOT_CTRL_B0_START:
     {
       int pos;
       pos = utilDwToInt(&m_Packet.rx_packet.data[4]);
       uint32_t vel;
       vel = utilDwToUint(&m_Packet.rx_packet.data[8]);
       m_pAp->MotorRun(pos, vel);
     }
     break;
     case UI_NXLCD_RXCMD_MOT_CTRL_B0_JOG_CW:
     {
       int pos;
       pos = utilDwToInt(&m_Packet.rx_packet.data[4]);
       uint32_t vel;
       vel = utilDwToUint(&m_Packet.rx_packet.data[8]);
       m_pFm->JogMove(pos,vel,true);
     }
     break;
     case UI_NXLCD_RXCMD_MOT_CTRL_B0_JOG_CCW:
     {
       int pos;
       pos = utilDwToInt(&m_Packet.rx_packet.data[4]);
       uint32_t vel;
       vel = utilDwToUint(&m_Packet.rx_packet.data[8]);
       m_pFm->JogMove(pos,vel,false);

     }
     break;
     case UI_NXLCD_RXCMD_MOT_CTRL_B0_RESERVED2:
     case UI_NXLCD_RXCMD_MOT_CTRL_B0_RESERVED5:
     default:
       break;
    }
    switch (m_Packet.rx_packet.data[1])
    {
     case UI_NXLCD_RXCMD_MOT_CTRL_B1_ENCODE_RST:
       m_pFm->ClearPos();
       break;
     case UI_NXLCD_RXCMD_MOT_CTRL_B1_RESERVED1:
     case UI_NXLCD_RXCMD_MOT_CTRL_B1_RESERVED2:
     case UI_NXLCD_RXCMD_MOT_CTRL_B1_RESERVED3:
     case UI_NXLCD_RXCMD_MOT_CTRL_B1_RESERVED4:
     case UI_NXLCD_RXCMD_MOT_CTRL_B1_RESERVED5:
     case UI_NXLCD_RXCMD_MOT_CTRL_B1_RESERVED6:
     case UI_NXLCD_RXCMD_MOT_CTRL_B1_RESERVED7:
     default:
       break;
    }
  }
  break;
  case NX_LCD_RXCMD_TYPE_SAVE_MOTOR_DATA:
  {
    m_pAp->WriteMotorData(&m_Packet.rx_packet.data[0], 12);    
  }
  break;
  case NX_LCD_RXCMD_TYPE_REQ_MOTOR_DATA:
  {
    SendMotorData();
  }
  break;
  case NX_LCD_RXCMD_TYPE_SAVE_AP_CFG:
  {    
    m_stepBuffer[0] = m_Packet.rx_packet.length;
    memcpy(&m_stepBuffer[1],&m_Packet.rx_packet.data[0],m_Packet.rx_packet.length);
    m_pAp->WriteApCfgData(&m_stepBuffer[0], m_Packet.rx_packet.length + 1);    
  }
  break;
  case NX_LCD_RXCMD_TYPE_REQ_AP_CFG:
  {
     SendApCfgData();
  }
  break;
  case NX_LCD_RXCMD_TYPE_SAVE_CYL_DATA:
  {
    m_stepBuffer[0] = m_Packet.rx_packet.length;
    memcpy(&m_stepBuffer[1],&m_Packet.rx_packet.data[0],m_Packet.rx_packet.length);
    m_pAp->WriteCylData(&m_stepBuffer[0], m_Packet.rx_packet.length+1);     
  }
  break;
  case NX_LCD_RXCMD_TYPE_REQ_CYL_DATA:
  {
    SendCylinderData();
  }
  break;
  case NX_LCD_RXCMD_TYPE_SAVE_VAC_DATA:
  {
    m_stepBuffer[0] = m_Packet.rx_packet.length;
    memcpy(&m_stepBuffer[1],&m_Packet.rx_packet.data[0],m_Packet.rx_packet.length);
    m_pAp->WriteVacData(&m_stepBuffer[0], m_Packet.rx_packet.length + 1);   
  }
  break;
  case NX_LCD_RXCMD_TYPE_REQ_VAC_DATA:
  {
    SendVacuumData();
  }
  break;
  case NX_LCD_RXCMD_TYPE_SAVE_SEQ_DATA:
  {
    m_stepBuffer[0] = m_Packet.rx_packet.length;
    memcpy(&m_stepBuffer[1],&m_Packet.rx_packet.data[0],m_Packet.rx_packet.length);
    m_pAp->WriteSeqData(&m_stepBuffer[0], m_Packet.rx_packet.length); 
  }
  break;
  case NX_LCD_RXCMD_TYPE_REQ_SEQ_DATA:
  {
    SendSequenceData();
  }
  break;
  default:
    break;
  }

  m_IsRespCplt = true;
}

bool uiNextionLcd::ThreadJob()
{
  doRunStep();
  return m_IsRespCplt;
}

void uiNextionLcd::doRunStep()
{

  if (millis() - m_pre_time >= 1000 * 1)
  {
    m_step.SetStep(UI_NEXTION_STEP_INIT);
  }

  switch (m_step.GetStep())
  {
  case UI_NEXTION_STEP_INIT:
  {
    memset(&m_stepBuffer, 0, UI_NEXTION_MAX_BUFFER_LENGTH);
    m_step.SetStep(UI_NEXTION_STEP_TODO);
    m_pre_time = millis();
  }
  break;
  /*######################################################
     to do
    ######################################################*/
  case UI_NEXTION_STEP_TODO:
  {
    m_IsRespCplt = true;
    m_retryCnt = 0;
  }
  break;
  /*######################################################
     timeout/ page check
    ######################################################*/
  case UI_NEXTION_STEP_TIMEOUT:
  {
    if (millis() - m_pre_time < 5)
    {
      break;
    }
    m_retryCnt = 0;
    m_step.SetStep(UI_NEXTION_STEP_TODO);
  }
  break;
  /*######################################################
     UI_NEXTION_STEP_DATA_SEND_MOTOR_POS
    ######################################################*/
  case UI_NEXTION_STEP_DATA_SEND_MOTOR_POS:
  {    
    m_retryCnt = 0;
    /* recmod change */
    uint8_t send_value = 1;
    cmdNextion_SendCmd(&m_Packet,NX_LCD_TXCMD_TYPE_LCD_START_REPARSEMODE, &send_value, 1);
    m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_MOTOR_POS_START);
    m_pre_time = millis();
  }
  break;
  case UI_NEXTION_STEP_DATA_SEND_MOTOR_POS_START:
  {
    if (millis() - m_pre_time < 5)
      break;

    memset(&m_stepBuffer,0,UI_NEXTION_MAX_BUFFER_LENGTH);
    m_stepBuffer[0] = UI_NEXTION_CMD_SEND_DATA_TYPE_MOTOR_POS;
    axis_dat::dat_t data;
    data = m_pApAxisDat->ReadData(axis_dat::addr_e::ready_pos);
    memcpy(&m_stepBuffer[1], (uint8_t*)&data, 8);
    data = m_pApAxisDat->ReadData(axis_dat::addr_e::pos_0);
    memcpy(&m_stepBuffer[9], (uint8_t*)&data, 8);
    data = m_pApAxisDat->ReadData(axis_dat::addr_e::pos_1);
    memcpy(&m_stepBuffer[17], (uint8_t*)&data, 8);
    data = m_pApAxisDat->ReadData(axis_dat::addr_e::pos_2);
    memcpy(&m_stepBuffer[25], (uint8_t*)&data, 8);

    cmdNextion_SendData(&m_Packet, 0, 0, &m_stepBuffer[0], UI_NEXTION_MAX_BUFFER_LENGTH);

    m_IsRespCplt=false;
    if (m_step.pre_step == UI_NEXTION_STEP_DATA_SEND_ALL_START)
      m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_ALL_WAIT);
    else if (m_step.pre_step == UI_NEXTION_STEP_DATA_SEND_AXIS_DAT_START)
      m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_AXIS_DAT_WAIT);
    else
      m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_MOTOR_POS_WAIT);
    m_pre_time = millis();
  }
  break;
  case UI_NEXTION_STEP_DATA_SEND_MOTOR_POS_WAIT:
  {
    if (millis()-m_pre_time >= 50)
    {
      if (m_retryCnt++ < UI_NEXTION_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_MOTOR_POS_START);
        m_pre_time = millis();
      }
      else
      {
        m_retryCnt = 0;    
        cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_END_REPARSEMODE, 0, 0);    
        m_step.SetStep(UI_NEXTION_STEP_TIMEOUT);        
        m_pre_time = millis();
      }
    }

    if(m_IsRespCplt)
    {
      cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_END_REPARSEMODE, 0, 0);
      m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_MOTOR_POS_END);
      m_pre_time = millis();      
    }
  }
  break;
  case UI_NEXTION_STEP_DATA_SEND_MOTOR_POS_END:
  {
    if (millis() - m_pre_time < 5)
      break;

    m_step.SetStep(UI_NEXTION_STEP_TODO);
    m_pre_time = millis();
  }
  break;

  /*######################################################
     UI_NEXTION_STEP_DATA_SEND_AP_DAT
    ######################################################*/
  case UI_NEXTION_STEP_DATA_SEND_AP_DAT:
  {    
    m_retryCnt = 0;
    /* recmod change */
    uint8_t send_value = 1;
    cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_START_REPARSEMODE, &send_value, 1);
    m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_AP_DAT_START);
    m_pre_time = millis();
  }
  break;
  case UI_NEXTION_STEP_DATA_SEND_AP_DAT_START:
  {
    if (millis() - m_pre_time < 5)
      break;

    memset(&m_stepBuffer, 0, UI_NEXTION_MAX_BUFFER_LENGTH);
    m_stepBuffer[0] = UI_NEXTION_CMD_SEND_DATA_TYPE_AP_DAT;
    ap_dat::dat_t data;
    for (uint8_t i = 0; i < static_cast<uint8_t>(ap_dat::addr_e::_max); i++)
    {
      data = m_pApCfgDat->ReadData(static_cast<ap_dat::addr_e>(i));
      memcpy(&m_stepBuffer[(1 + i * 4) % UI_NEXTION_MAX_BUFFER_LENGTH], (uint8_t *)&data, 4);
    }

    cmdNextion_SendData(&m_Packet, 0, 0, &m_stepBuffer[0], UI_NEXTION_MAX_BUFFER_LENGTH);

    m_IsRespCplt = false;
    if (m_step.pre_step == UI_NEXTION_STEP_DATA_SEND_ALL_START)
      m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_ALL_WAIT);
    else
      m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_AP_DAT_WAIT);
    m_pre_time = millis();
  }
  break;
  case UI_NEXTION_STEP_DATA_SEND_AP_DAT_WAIT:
  {
    if (millis() - m_pre_time >= 50)
    {
      if (m_retryCnt++ < UI_NEXTION_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_AP_DAT_START);
        m_pre_time = millis();
      }
      else
      {
        m_retryCnt = 0;
        cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_END_REPARSEMODE, 0, 0);
        m_step.SetStep(UI_NEXTION_STEP_TIMEOUT);
        m_pre_time = millis();
      }
    }
    if (m_IsRespCplt)
    {
      cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_END_REPARSEMODE, 0, 0);
      m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_AP_DAT_END);
      m_pre_time = millis();
    }
  }
  break;
  case UI_NEXTION_STEP_DATA_SEND_AP_DAT_END:
  {
    if (millis() - m_pre_time < 5)
      break;

    m_step.SetStep(UI_NEXTION_STEP_TODO);
    m_pre_time = millis();
  }
  break;

  /*######################################################
     UI_NEXTION_STEP_DATA_SEND_CYL_DAT
    ######################################################*/
  case UI_NEXTION_STEP_DATA_SEND_CYL_DAT:
  {
    m_retryCnt = 0;
    /* recmod change */
    uint8_t send_value = 1;
    cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_START_REPARSEMODE, &send_value, 1);
    m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_CYL_DAT_START);
    m_pre_time = millis();
  }
  break;
  case UI_NEXTION_STEP_DATA_SEND_CYL_DAT_START:
  {
    if (millis() - m_pre_time < 5)
      break;

    memset(&m_stepBuffer,0,UI_NEXTION_MAX_BUFFER_LENGTH);
    m_stepBuffer[0] = UI_NEXTION_CMD_SEND_DATA_TYPE_CYL_DAT;
    cyl_dat::dat_t data;
    for (uint8_t i = 0; i < static_cast<uint8_t>(cyl_dat::addr_e::_max); i++)
    {
      data = m_pApCylDat->ReadData(static_cast<cyl_dat::addr_e>(i));
      memcpy(&m_stepBuffer[(1 + i * 4) % UI_NEXTION_MAX_BUFFER_LENGTH], (uint8_t *)&data, 4);
    }

    cmdNextion_SendData(&m_Packet, 0, 0, &m_stepBuffer[0], UI_NEXTION_MAX_BUFFER_LENGTH);

    m_IsRespCplt = false;
    if (m_step.pre_step == UI_NEXTION_STEP_DATA_SEND_ALL_START)
      m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_ALL_WAIT);
    else
      m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_CYL_DAT_WAIT);
    m_pre_time = millis();
  }
  break;
  case UI_NEXTION_STEP_DATA_SEND_CYL_DAT_WAIT:
  {
    if (millis() - m_pre_time >= 50)
    {
      if (m_retryCnt++ < UI_NEXTION_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_CYL_DAT_START);
        m_pre_time = millis();
      }
      else
      {
        m_retryCnt = 0;
        cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_END_REPARSEMODE, 0, 0);
        m_step.SetStep(UI_NEXTION_STEP_TIMEOUT);
        m_pre_time = millis();
      }
    }
    if (m_IsRespCplt)
    {
      cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_END_REPARSEMODE, 0, 0);
      m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_CYL_DAT_END);
      m_pre_time = millis();
    }
  }
  break;
  case UI_NEXTION_STEP_DATA_SEND_CYL_DAT_END:
  {
    if (millis() - m_pre_time < 5)
      break;

    m_step.SetStep(UI_NEXTION_STEP_TODO);
    m_pre_time = millis();
  }
  break;

  /*######################################################
     UI_NEXTION_STEP_DATA_SEND_VAC_DAT
    ######################################################*/
  case UI_NEXTION_STEP_DATA_SEND_VAC_DAT:
  {
    m_retryCnt = 0;
    /* recmod change */
    uint8_t send_value = 1;
    cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_START_REPARSEMODE, &send_value, 1);
    m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_VAC_DAT_START);
    m_pre_time = millis();
  }
  break;
  case UI_NEXTION_STEP_DATA_SEND_VAC_DAT_START:
  {
    if (millis() - m_pre_time < 5)
      break;

    memset(&m_stepBuffer,0,UI_NEXTION_MAX_BUFFER_LENGTH);
    m_stepBuffer[0] = UI_NEXTION_CMD_SEND_DATA_TYPE_VAC_DAT;
    vac_dat::dat_t data;
    for (uint8_t i = 0; i < static_cast<uint8_t>(vac_dat::addr_e::_max); i++)
    {
      data = m_pApVacDat->ReadData(static_cast<vac_dat::addr_e>(i));
      memcpy(&m_stepBuffer[(1 + i * 4) % UI_NEXTION_MAX_BUFFER_LENGTH], (uint8_t *)&data, 4);
    }

    cmdNextion_SendData(&m_Packet, 0, 0, &m_stepBuffer[0], UI_NEXTION_MAX_BUFFER_LENGTH);

    m_IsRespCplt = false;
    if (m_step.pre_step == UI_NEXTION_STEP_DATA_SEND_ALL_START)
      m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_ALL_WAIT);
    else
      m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_VAC_DAT_WAIT);
    m_pre_time = millis();
  }
  break;
  case UI_NEXTION_STEP_DATA_SEND_VAC_DAT_WAIT:
  {
    if (millis() - m_pre_time >= 50)
    {
      if (m_retryCnt++ < UI_NEXTION_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_VAC_DAT_START);
        m_pre_time = millis();
      }
      else
      {
        m_retryCnt = 0;
        cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_END_REPARSEMODE, 0, 0);
        m_step.SetStep(UI_NEXTION_STEP_TIMEOUT);
        m_pre_time = millis();
      }
    }
    if (m_IsRespCplt)
    {
      cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_END_REPARSEMODE, 0, 0);
      m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_VAC_DAT_END);
      m_pre_time = millis();
    }
  }
  break;
  case UI_NEXTION_STEP_DATA_SEND_VAC_DAT_END:
  {
    if (millis() - m_pre_time < 5)
      break;

    m_step.SetStep(UI_NEXTION_STEP_TODO);
    m_pre_time = millis();
  }
  break;

  /*######################################################
     UI_NEXTION_STEP_DATA_SEND_SEQ_DAT
    ######################################################*/
  case UI_NEXTION_STEP_DATA_SEND_SEQ_DAT:
  {
    m_retryCnt = 0;
    /* recmod change */
    uint8_t send_value = 1;
    cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_START_REPARSEMODE, &send_value, 1);
    m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_SEQ_DAT_START);
    m_pre_time = millis();
  }
  break;
  case UI_NEXTION_STEP_DATA_SEND_SEQ_DAT_START:
  {
    if (millis() - m_pre_time < 5)
    {
      break;
    }

    memset(&m_stepBuffer, 0, UI_NEXTION_MAX_BUFFER_LENGTH);
    m_stepBuffer[0] = UI_NEXTION_CMD_SEND_DATA_TYPE_SEQ_DAT;
    seq_dat::dat_t data;
    for (uint8_t i = 0; i < static_cast<uint8_t>(seq_dat::addr_e::_max); i++)
    {
      data = m_pApSeqDat->ReadData(static_cast<seq_dat::addr_e>(i));
      memcpy(&m_stepBuffer[(1 + i * 4) % UI_NEXTION_MAX_BUFFER_LENGTH], (uint8_t *)&data, 4);
    }

    cmdNextion_SendData(&m_Packet, 0, 0, &m_stepBuffer[0], UI_NEXTION_MAX_BUFFER_LENGTH);

    m_IsRespCplt = false;
    if (m_step.pre_step == UI_NEXTION_STEP_DATA_SEND_ALL_START)
      m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_ALL_WAIT);
    else
      m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_SEQ_DAT_WAIT);
    m_pre_time = millis();
  }
  break;
  case UI_NEXTION_STEP_DATA_SEND_SEQ_DAT_WAIT:
  {
    if (millis() - m_pre_time >= 50)
    {
      if (m_retryCnt++ < UI_NEXTION_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_SEQ_DAT_START);
        m_pre_time = millis();
      }
      else
      {
        cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_END_REPARSEMODE, 0, 0);
        m_step.SetStep(UI_NEXTION_STEP_TIMEOUT);
        m_pre_time = millis();
      }
    }
    if (m_IsRespCplt)
    {
      cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_END_REPARSEMODE, 0, 0);
      m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_SEQ_DAT_END);
      m_pre_time = millis();
    }
  }
  break;
  case UI_NEXTION_STEP_DATA_SEND_SEQ_DAT_END:
  {
    if (millis() - m_pre_time < 5)
      break;

    m_step.SetStep(UI_NEXTION_STEP_TODO);
    m_pre_time = millis();
  }
  break;

  /*######################################################
     UI_NEXTION_STEP_DATA_SEND_ALL
    ######################################################*/
  case UI_NEXTION_STEP_DATA_SEND_ALL:
  {
    memset(&m_stepBuffer,0,UI_NEXTION_MAX_BUFFER_LENGTH);
    m_retryCnt = 0;
    /* recmod change */
    uint8_t send_value = 1;
    if (m_step.pre_step == UI_NEXTION_STEP_DATA_RECEIVE_CNT_RESET)
    {
      cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_START_REPARSEMODE, &send_value, 1);
      m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_ALL_START);
    }
    else
    {
      m_step.SetStep(UI_NEXTION_STEP_DATA_RECEIVE_CNT_RESET);
    }
    m_pre_time = millis();
  }
  break;

  case UI_NEXTION_STEP_DATA_SEND_ALL_START:
  {
    if (millis() - m_pre_time < 5) //delay 5ms
      break;

    if (m_retryCnt > 0)
    {
      /* retry case */
      switch (m_stepBuffer[0])
      {
      case UI_NEXTION_CMD_SEND_DATA_TYPE_MOTOR_POS:
        m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_MOTOR_POS_START);
        break;

      case UI_NEXTION_CMD_SEND_DATA_TYPE_AP_DAT:
        m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_AP_DAT_START);
        break;

      case UI_NEXTION_CMD_SEND_DATA_TYPE_CYL_DAT:
        m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_CYL_DAT_START);
        break;

      case UI_NEXTION_CMD_SEND_DATA_TYPE_VAC_DAT:
        m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_VAC_DAT_START);
        break;

      case UI_NEXTION_CMD_SEND_DATA_TYPE_SEQ_DAT:
        m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_SEQ_DAT_START);
        break;

      case 0x00:
      default:
        m_step.SetStep(UI_NEXTION_STEP_TODO);
        break;
      }
    }
    else
    {
      /* 
      tx order
      1. motor (start)
      2. ap data
      3. cylinder data
      4. vacuum data
      5. sequence data (end)
      */

      switch (m_stepBuffer[0])
      {
      case UI_NEXTION_CMD_SEND_DATA_TYPE_MOTOR_POS:
        m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_AP_DAT_START);
        break;

      case UI_NEXTION_CMD_SEND_DATA_TYPE_AP_DAT:
        m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_CYL_DAT_START);
        break;

      case UI_NEXTION_CMD_SEND_DATA_TYPE_CYL_DAT:
        m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_VAC_DAT_START);
        break;

      case UI_NEXTION_CMD_SEND_DATA_TYPE_VAC_DAT:
        m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_SEQ_DAT_START);
        break;

      case UI_NEXTION_CMD_SEND_DATA_TYPE_SEQ_DAT:
        m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_ALL_END);
        break;

      case 0x00:
      default:
        m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_MOTOR_POS_START);
        break;
      }
    }
    m_pre_time = millis();
  }
  break;
  case UI_NEXTION_STEP_DATA_SEND_ALL_WAIT:
  {
    if (millis() - m_pre_time >= 50)
    {
      if (m_retryCnt++ < UI_NEXTION_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_ALL_START);
        m_pre_time = millis();
      }
      else
      {
        cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_END_REPARSEMODE, 0, 0);
        m_step.SetStep(UI_NEXTION_STEP_TIMEOUT);
        m_pre_time = millis();
      }
    }
    if (m_IsRespCplt)
    {
      m_retryCnt = 0;
      switch (m_stepBuffer[0])
      {
      case UI_NEXTION_CMD_SEND_DATA_TYPE_MOTOR_POS:
      case UI_NEXTION_CMD_SEND_DATA_TYPE_AP_DAT:
      case UI_NEXTION_CMD_SEND_DATA_TYPE_CYL_DAT:
      case UI_NEXTION_CMD_SEND_DATA_TYPE_VAC_DAT:
        m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_ALL_START);
        break;

      case UI_NEXTION_CMD_SEND_DATA_TYPE_SEQ_DAT:
      default:
        cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_END_REPARSEMODE, 0, 0);
        m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_ALL_END);
        break;
      }
      m_pre_time = millis();
    }
  }
  break;
  case UI_NEXTION_STEP_DATA_SEND_ALL_END:
  {
    if (millis() - m_pre_time < 5)
      break;

    m_step.SetStep(UI_NEXTION_STEP_TODO);
    m_pre_time = millis();
  }
  break;
  /*######################################################
     UI_NEXTION_STEP_DATA_SEND_IO_REG
    ######################################################*/
  case UI_NEXTION_STEP_DATA_SEND_IO_REG:
  {
    m_retryCnt = 0;
    /* recmod change */
    uint8_t send_value = 1;
    cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_START_REPARSEMODE, &send_value, 1);
    m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_IO_REG_START);
    m_pre_time = millis();
  }
  break;
  case UI_NEXTION_STEP_DATA_SEND_IO_REG_START:
  {
    if (millis() - m_pre_time < 5)
      break;

    memset(&m_stepBuffer,0,UI_NEXTION_MAX_BUFFER_LENGTH);
    m_stepBuffer[0] = UI_NEXTION_CMD_SEND_DATA_TYPE_IO;

    m_stepBuffer[4] = m_pApIo->GetBank_In(AP_IO_DEF_BANK_NO_0);
    m_stepBuffer[3] = m_pApIo->GetBank_In(AP_IO_DEF_BANK_NO_1);
    m_stepBuffer[2] = m_pApIo->GetBank_Out(AP_IO_DEF_BANK_NO_0);
    m_stepBuffer[1] = m_pApIo->GetBank_Out(AP_IO_DEF_BANK_NO_1);

    cmdNextion_SendData(&m_Packet, 0, 0, &m_stepBuffer[0], UI_NEXTION_MAX_BUFFER_LENGTH);

    m_IsRespCplt = false;
    m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_IO_REG_WAIT);
    m_pre_time = millis();
  }
  break;
  case UI_NEXTION_STEP_DATA_SEND_IO_REG_WAIT:
  {
    if (millis() - m_pre_time >= 50)
    {
      if (m_retryCnt++ < UI_NEXTION_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_IO_REG_START);
        m_pre_time = millis();
      }
      else
      {
        m_retryCnt = 0;
        cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_END_REPARSEMODE, 0, 0);
        m_step.SetStep(UI_NEXTION_STEP_TIMEOUT);
        m_pre_time = millis();
      }
    }

    if (m_IsRespCplt)
    {
      cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_END_REPARSEMODE, 0, 0);
      m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_IO_REG_END);
      m_pre_time = millis();
    }
  }
  break;
  case UI_NEXTION_STEP_DATA_SEND_IO_REG_END:
  {
    if (millis() - m_pre_time < 5)
      break;

    m_step.SetStep(UI_NEXTION_STEP_TODO);
    m_pre_time = millis();
  }
  break;
  /*######################################################
     UI_NEXTION_STEP_DATA_SEND_AP_REG
    ######################################################*/
  case UI_NEXTION_STEP_DATA_SEND_AP_REG:
  {
    m_retryCnt = 0;
    /* recmod change */
    uint8_t send_value = 1;
    cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_START_REPARSEMODE, &send_value, 1);
    m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_AP_REG_START);
    m_pre_time = millis();
  }
  break;
  case UI_NEXTION_STEP_DATA_SEND_AP_REG_START:
  {
    if (millis() - m_pre_time < 5)
      break;

    memset(&m_stepBuffer,0,UI_NEXTION_MAX_BUFFER_LENGTH);
    m_stepBuffer[0] = UI_NEXTION_CMD_SEND_DATA_TYPE_AP_REG;

    m_stepBuffer[1] = m_pApReg->status[AP_REG_BANK_ERR_H].get();
    m_stepBuffer[2] = m_pApReg->status[AP_REG_BANK_ERR_L].get();
    m_stepBuffer[3] = m_pApReg->status[AP_REG_BANK_SETTING].get();
    m_stepBuffer[4] = m_pApReg->status[AP_REG_BANK_RUN_STATE].get();

    cmdNextion_SendData(&m_Packet, 0, 0, &m_stepBuffer[0], UI_NEXTION_MAX_BUFFER_LENGTH);

    m_IsRespCplt = false;
    m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_AP_REG_WAIT);
    m_pre_time = millis();
  }
  break;
  case UI_NEXTION_STEP_DATA_SEND_AP_REG_WAIT:
  {
    if (millis() - m_pre_time >= 50)
    {
      if (m_retryCnt++ < UI_NEXTION_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_AP_REG_START);
        m_pre_time = millis();
      }
      else
      {
        m_retryCnt = 0;
        cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_END_REPARSEMODE, 0, 0);
        m_step.SetStep(UI_NEXTION_STEP_TIMEOUT);
        m_pre_time = millis();
      }
    }

    if (m_IsRespCplt)
    {
      cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_END_REPARSEMODE, 0, 0);
      m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_AP_REG_END);
      m_pre_time = millis();
    }
  }
  break;
  case UI_NEXTION_STEP_DATA_SEND_AP_REG_END:
  {
    if (millis() - m_pre_time < 5)
      break;

    m_step.SetStep(UI_NEXTION_STEP_TODO);
    m_pre_time = millis();
  }
  break;

  /*######################################################
     UI_NEXTION_STEP_DATA_SEND_AXIS_DAT
    ######################################################*/
  case UI_NEXTION_STEP_DATA_SEND_AXIS_DAT:
  {
    memset(&m_stepBuffer,0,UI_NEXTION_MAX_BUFFER_LENGTH);
    m_retryCnt = 0;
    /* recmod change */
    uint8_t send_value = 1;
    if (m_step.pre_step == UI_NEXTION_STEP_DATA_RECEIVE_CNT_RESET)
    {
      cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_START_REPARSEMODE, &send_value, 1);
      m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_AXIS_DAT_START);
    }
    else
    {
      m_step.SetStep(UI_NEXTION_STEP_DATA_RECEIVE_CNT_RESET);
    }
    m_pre_time = millis();
  }
  break;
  case UI_NEXTION_STEP_DATA_SEND_AXIS_DAT_START:
  {
    if (millis() - m_pre_time < 5)
      break;

    if (m_retryCnt > 0)
    {
      if (m_stepBuffer[0] == UI_NEXTION_CMD_SEND_DATA_TYPE_MOTOR_POS)
      {
        m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_MOTOR_POS_START);
        m_pre_time = millis();
        break;
      }
    }
    else
    {
      if (m_stepBuffer[0] != UI_NEXTION_CMD_SEND_DATA_TYPE_MOTOR_POS)
      {
        m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_MOTOR_POS_START);
        m_pre_time = millis();
        break;
      }
    }

    memset(&m_stepBuffer, 0, UI_NEXTION_MAX_BUFFER_LENGTH);
    m_stepBuffer[0] = UI_NEXTION_CMD_SEND_DATA_TYPE_AXIS_STATE;

    // act pos/ vel data
    int pos = m_pFm->m_AxisState.act_pos;
    memcpy(&m_stepBuffer[1], (uint8_t*)&pos, 4);
    pos = m_pFm->m_AxisState.act_vel;
    memcpy(&m_stepBuffer[5], (uint8_t*)&pos, 4);

    // ap_reg data
    m_stepBuffer[9] = m_pApReg->status[AP_REG_BANK_ERR_H].get();
    m_stepBuffer[10] = m_pApReg->status[AP_REG_BANK_ERR_L].get();
    m_stepBuffer[11] = m_pApReg->status[AP_REG_BANK_SETTING].get();
    m_stepBuffer[12] = m_pApReg->status[AP_REG_BANK_RUN_STATE].get();
    cmdNextion_SendData(&m_Packet, 0, 0, &m_stepBuffer[0], UI_NEXTION_MAX_BUFFER_LENGTH);

    m_IsRespCplt = false;
    m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_AXIS_DAT_WAIT);
    m_pre_time = millis();
  }
  break;
  case UI_NEXTION_STEP_DATA_SEND_AXIS_DAT_WAIT:
  {
    if (millis() - m_pre_time >= 50)
    {
      if (m_retryCnt++ < UI_NEXTION_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_AXIS_DAT_START);
        m_pre_time = millis();
      }
      else
      {
        m_retryCnt = 0;
        cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_END_REPARSEMODE, 0, 0);
        m_step.SetStep(UI_NEXTION_STEP_TIMEOUT);        
        m_pre_time = millis();
      }
    }

    if (m_IsRespCplt)
    {
      m_retryCnt = 0;
      if (m_stepBuffer[0] == UI_NEXTION_CMD_SEND_DATA_TYPE_MOTOR_POS)
      {
        m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_AXIS_DAT_START);
      }
      else
      {
        cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_END_REPARSEMODE, 0, 0);
        m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_AXIS_DAT_END);
      }
      m_pre_time = millis();
    }
  }
  break;
  case UI_NEXTION_STEP_DATA_SEND_AXIS_DAT_END:
  {
    if (millis() - m_pre_time < 5)
      break;

    m_step.SetStep(UI_NEXTION_STEP_TODO);
    m_pre_time = millis();
  }
  break;

  /*######################################################
     UI_NEXTION_STEP_DATA_SEND_LOG_DAT
    ######################################################*/
  case UI_NEXTION_STEP_DATA_SEND_LOG_DAT:
  {
    m_retryCnt = 0;
    /* recmod change */
    uint8_t send_value = 1;    
    cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_START_REPARSEMODE, &send_value, 1);
    m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_LOG_DAT_START);
    m_pre_time = millis();
  }
  break;
  case UI_NEXTION_STEP_DATA_SEND_LOG_DAT_START:
  {
    if (millis() - m_pre_time < 5)
      break;

    memset(&m_stepBuffer, 0, UI_NEXTION_MAX_BUFFER_LENGTH);
    // cmdNextion_SendData(&m_Packet, 0, 0,/*(uint8_t*)&log_dat*/m_pLog->Get() , APDAT_LOG_DATA_LENGTH);

    m_IsRespCplt = false;
    m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_LOG_DAT_WAIT);
    m_pre_time = millis();
  }
  break;
  case UI_NEXTION_STEP_DATA_SEND_LOG_DAT_WAIT:
  {
    if (millis() - m_pre_time >= 50)
    {
      if (m_retryCnt++ < UI_NEXTION_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_LOG_DAT_START);
        m_pre_time = millis();
      }
      else
      {
        m_retryCnt = 0;
        cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_END_REPARSEMODE, 0, 0);
        m_step.SetStep(UI_NEXTION_STEP_TIMEOUT);
        m_pre_time = millis();
      }
    }

    if (m_IsRespCplt)
    {
      cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_END_REPARSEMODE, 0, 0);
      m_step.SetStep(UI_NEXTION_STEP_DATA_SEND_LOG_DAT_END);
      m_pre_time = millis();
    }
  }
  break;
  case UI_NEXTION_STEP_DATA_SEND_LOG_DAT_END:
  {
    if (millis() - m_pre_time < 5)
      break;

    m_step.SetStep(UI_NEXTION_STEP_TODO);
    m_pre_time = millis();
  }
  break;

  /*######################################################
    UI_NEXTION_STEP_WRITE_IO_REG_OUT
    ######################################################*/
  case UI_NEXTION_STEP_WRITE_IO_REG_OUT:
  {
    m_retryCnt = 0;
    m_step.SetStep(UI_NEXTION_STEP_WRITE_IO_REG_OUT_START);
    m_pre_time = millis();
  }
  break;
  case UI_NEXTION_STEP_WRITE_IO_REG_OUT_START:
  {

    // cmdNextion_SendData(&m_Packet, 0, 0,/*(uint8_t*)&log_dat*/m_pLog->Get() , APDAT_LOG_DATA_LENGTH);
    m_pApIo->SetBank_Out(AP_IO_DEF_BANK_OUT_0, m_stepBuffer[0]);
    m_IsRespCplt = false;
    m_step.SetStep(UI_NEXTION_STEP_WRITE_IO_REG_OUT_WAIT);
    m_pre_time = millis();
  }
  break;
  case UI_NEXTION_STEP_WRITE_IO_REG_OUT_WAIT:
  {
    if (millis() - m_pre_time >= 50)
    {
      if (m_retryCnt++ < UI_NEXTION_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(UI_NEXTION_STEP_WRITE_IO_REG_OUT_START);
        m_pre_time = millis();
      }
      else
      {
        m_retryCnt = 0;
        m_step.SetStep(UI_NEXTION_STEP_TIMEOUT);
        m_pre_time = millis();
      }
    }

    bool is_equal = m_pApIo->GetBank_Out(AP_IO_DEF_BANK_OUT_0) == m_stepBuffer[0];
    if (is_equal)
    {
      m_step.SetStep(UI_NEXTION_STEP_WRITE_IO_REG_OUT_END);
      m_pre_time = millis();
    }
  }
  break;
  case UI_NEXTION_STEP_WRITE_IO_REG_OUT_END:
  {
    //if (millis() - m_pre_time < 5)
    //  break;

    m_step.SetStep(UI_NEXTION_STEP_TODO);
    m_pre_time = millis();
  }
  break;

  /*######################################################
     UI_NEXTION_STEP_DATA_RECEIVE_CNT_RESET
    ######################################################*/
  case UI_NEXTION_STEP_DATA_RECEIVE_CNT_RESET:
  {
    if (millis() - m_pre_time >= 5)
    {
      cmdNextion_SendCmd(&m_Packet, NX_LCD_TXCMD_TYPE_LCD_RESET_RECEIVE_CNT, 0, 0);
      m_pre_time = millis();
      m_step.SetStep(m_step.pre_step);
    }
  }
  break;

  default:
    break;
  } // switch
}

void uiNextionLcd::LcdUpdate()
{
  if (m_pApReg->IsAutoReadyMode() || m_pApReg->IsAutoRunning())
  {
    if (m_currPage != nextionpage_t::NEX_PAGE_MAIN)
    {
      ChangePage(nextionpage_t::NEX_PAGE_MAIN);
    }
  }

  switch (m_currPage)
  {
  case nextionpage_t::NEX_PAGE_INIT:
    SendDataAll();
    break;

  case nextionpage_t::NEX_PAGE_MAIN:
    SendApReg();
    break;

  case nextionpage_t::NEX_PAGE_IO:
    SendApIoReg();
    break;

  case nextionpage_t::NEX_PAGE_MANUAL:
    SendApIoReg();
    break;

  case nextionpage_t::NEX_PAGE_TEACH:
    SendAxisData();
    break;

  case nextionpage_t::NEX_PAGE_DATA:
    SendDataAll();
    break;

  case nextionpage_t::NEX_PAGE_LOG:
  {
    //  if(m_pLog->Available())
    //  {
    //    SendLogData();
    //  }
  }
  break;
  default:
    break;
  }
}