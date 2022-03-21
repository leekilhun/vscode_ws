/*
 * enFastechMotor.cpp
 *
 *  Created on: 2022. 2. 22.
 *      Author: gns2l
 */
#include "ap_def.h"
#include <engine/enFastechMotor.h>

#define DEF_FM_COMM_ERROR_CNT_MAX   10

//extern uint32_t check_pass_ms;


enFastechMotor::enFastechMotor (uint8_t id = 0):m_AxisId(id)
{
  m_pApReg = NULL;
  m_pApIo = NULL;
  m_pApDat = NULL;
  m_Packet.MotorId = m_AxisId;
  m_IsConnected = false;
}

enFastechMotor::~enFastechMotor ()
{
  // TODO Auto-generated destructor stub
}

int enFastechMotor::Init(enFastechMotor::cfg_t* cfg)
{
  cmdFastech_Init(&m_Packet);
  cmdFastech_Open(&m_Packet, cfg->ch, cfg->baud);

  m_pApReg = cfg->p_apReg;
  m_pApIo = cfg->p_apIo;
  m_pApDat = cfg->p_apDat;

  return 0;
}

void enFastechMotor::CommRecovery()
{
  cmdFastech_Recovery(&m_Packet);
  m_Packet.check_sync.is_checked = true;
}

bool enFastechMotor::IsBusy()
{
  return m_AxisState.axis_status[DEF_FM_BANK_STATE_02][FM_REG_MOTIONING];
}

int enFastechMotor::MotorOnOff(bool on_off)
{
  bool ret;
  uint8_t data[2] ={0,};
  data[0] = DEF_FASTECH_COMM_TYPE_SERVO_ENABLE;
  data[1] = (uint8_t)on_off;
  ret = cmdFastech_SendCmd(&this->m_Packet,&data[0],2);

  return (ret ? 0 : -1);
}

int enFastechMotor::Move (int cmd_pos, uint32_t cmd_vel, uint32_t acc, uint32_t decel)
{
  bool ret;
  if (!IsMotorOn())
    return -1;

  uint8_t data[9] ={0};
  data[0] = DEF_FASTECH_COMM_TYPE_MOVE_ABS_SINGLE_AXIS;
  data[1] = (uint8_t)(cmd_pos>>0);
  data[2] = (uint8_t)(cmd_pos>>8);
  data[3] = (uint8_t)(cmd_pos>>16);
  data[4] = (uint8_t)(cmd_pos>>24);
  data[5] = (uint8_t)(cmd_vel>>0);
  data[6] = (uint8_t)(cmd_vel>>8);
  data[7] = (uint8_t)(cmd_vel>>16);
  data[8] = (uint8_t)(cmd_vel>>24);
  ret = cmdFastech_SendCmd(&this->m_Packet,&data[0],9);
  uint32_t pass_ms = millis();
  uint8_t retry_cnt = 0;

  m_Packet.rx_packet.response = 0xfe;
  while(m_Packet.rx_packet.response)
  {
    if ((millis() - pass_ms) > 100)
    {
      if (retry_cnt++ == 3)
        break;
      else
        ret = cmdFastech_SendCmd(&this->m_Packet,&data[0],9);
      pass_ms = millis();
    }
  }
  return (ret ? 0 : -1);
}



int enFastechMotor::JogMove(uint32_t cmd_vel,bool is_cw)
{
  bool ret;
  if (!IsMotorOn())
    return -1;

  uint8_t data[6] ={0};
  data[0] = DEF_FASTECH_COMM_TYPE_MOVE_VELOCITY;
  data[1] = (uint8_t)(cmd_vel>>0);
  data[2] = (uint8_t)(cmd_vel>>8);
  data[3] = (uint8_t)(cmd_vel>>16);
  data[4] = (uint8_t)(cmd_vel>>24);
  data[5] = (uint8_t)(is_cw);
  ret = cmdFastech_SendCmd(&this->m_Packet,&data[0],6);

  return (ret ? 0 : -1);
}

int enFastechMotor::JogMove(uint32_t jog_pos, uint32_t cmd_vel,bool is_cw)
{
  bool ret;
  if (!IsMotorOn())
    return -1;
  uint8_t data[9] ={0};

  if (!is_cw)
  {
    jog_pos = ~(jog_pos) +1;   // minus
  }

  data[0] = DEF_FASTECH_COMM_TYPE_MOVE_REL_SINGLE_AXIS;
  data[1] = (uint8_t)(jog_pos>>0);
  data[2] = (uint8_t)(jog_pos>>8);
  data[3] = (uint8_t)(jog_pos>>16);
  data[4] = (uint8_t)(jog_pos>>24);
  data[5] = (uint8_t)(cmd_vel>>0);
  data[6] = (uint8_t)(cmd_vel>>8);
  data[7] = (uint8_t)(cmd_vel>>16);
  data[8] = (uint8_t)(cmd_vel>>24);
  ret = cmdFastech_SendCmd(&this->m_Packet,&data[0],9);

  return (ret ? 0 : -1);
}
int enFastechMotor::Stop ()
{
  bool ret;
  uint8_t data[1] ={0};
  data[0] = DEF_FASTECH_COMM_TYPE_MOVE_STOP;
  ret = cmdFastech_SendCmd(&this->m_Packet,&data[0],1);
  uint32_t pass_ms = millis();
  uint8_t retry_cnt = 0;

  m_Packet.rx_packet.response = 0xfe;
  while(m_Packet.rx_packet.response)
  {
    if ((millis() - pass_ms) > 100)
    {
      if (retry_cnt++ == 3)
        break;
      else
        ret = cmdFastech_SendCmd(&this->m_Packet,&data[0],1);

      pass_ms = millis();
    }
  }
  return (ret ? 0 : -1);
}


int enFastechMotor::VStop ()
{
  bool ret;
  if (IsMotorOn() != false)
    return -1;
  uint8_t data[1] ={0};
  data[0] = DEF_FASTECH_COMM_TYPE_MOVE_STOP;
  ret = cmdFastech_SendCmd(&this->m_Packet,&data[0],1);
  uint32_t pass_ms = millis();
  uint8_t retry_cnt = 0;

  m_Packet.rx_packet.response = 0xfe;
  while(m_Packet.rx_packet.response)
  {
    if ((millis() - pass_ms) > 100)
    {
      if (retry_cnt++ == 3)
        break;  
      else
        ret = cmdFastech_SendCmd(&this->m_Packet,&data[0],1);
      pass_ms = millis();
    }
  }

  return (ret ? 0 : -1);
}

int enFastechMotor::EStop ()
{
  bool ret;
  if (IsMotorOn() != false)
    return -1;
  uint8_t data[1] ={0};
  data[0] = DEF_FASTECH_COMM_TYPE_MOVE_EMG_STOP;

  ret = cmdFastech_SendCmd(&this->m_Packet,&data[0],1);
  uint32_t pass_ms = millis();
  uint8_t retry_cnt = 0;

  m_Packet.rx_packet.response = 0xfe;
  while(m_Packet.rx_packet.response)
  {
    if ((millis() - pass_ms) > 100)
    {
      if (retry_cnt++ == 3)
        break;
      else
        ret = cmdFastech_SendCmd(&this->m_Packet,&data[0],1);
      pass_ms = millis();
    }
  }

  return (ret ? 0 : -1);
}

int enFastechMotor::ClearPos()
{
  bool ret;
  if (IsMotorOn() != false)
    return -1;
  uint8_t data[1] ={0};
  data[0] = DEF_FASTECH_COMM_TYPE_CLEAR_POSITION;
  ret = cmdFastech_SendCmd(&this->m_Packet,&data[0],1);

  return (ret ? 0 : -1);
}


bool enFastechMotor::IsOriginOK()
{
  return m_AxisState.axis_status[DEF_FM_BANK_STATE_02][FM_REG_ORG_RETURN_OK];
}

bool enFastechMotor::IsMotorOn()
{
  return m_AxisState.axis_status[DEF_FM_BANK_STATE_01][FM_REG_SERVO_ON]/*m_AxisState.is_motor_on*/;
}

int enFastechMotor::WaitDone(uint8_t mode)
{
  return 0;
}

void enFastechMotor::IsAxisDone()
{
  ;
}

int  enFastechMotor::ClearState()
{
  return 0;
}


void enFastechMotor::set_UserOutput(uint32_t mask_on, uint32_t mask_off)
{
  uint8_t data[9] ={0,};
  data[0] = DEF_FASTECH_COMM_TYPE_SET_IO_OUT_REG;
  data[1] = mask_on>>0;
  data[2] = mask_on>>8;
  data[3] = mask_on>>16;
  data[4] = mask_on>>24;
  data[5] = mask_off>>0;
  data[6] = mask_off>>8;
  data[7] = mask_off>>16;
  data[8] = mask_off>>24;

  cmdFastech_SendCmd(&this->m_Packet,&data[0],9);
}


void enFastechMotor::ProcessCmd()
{
  uint8_t* p_data = m_Packet.rx_packet.data;

  switch (m_Packet.rx_packet.type)
  {
    case DEF_FASTECH_COMM_TYPE_GET_ALARM_INFO:
      break;
    case DEF_FASTECH_COMM_TYPE_GET_RAM_PARAM:
      break;
    case DEF_FASTECH_COMM_TYPE_GET_AXIS_STATUS:
      break;
    case DEF_FASTECH_COMM_TYPE_GET_MOTION_STATUS:
    {
      enum class step_ {cmd_pos, act_pos, pos_err, act_vel, axis_reg };
      //int step = 0;
      step_ step = step_::cmd_pos;
      uint8_t loc = 0;
      while (loc < m_Packet.rx_packet.length)
      {
        switch (step)
        {
          case step_::cmd_pos:
          {
            uint8_t tmp[4] = { 0, };
            for (uint8_t i = 0; i < 4; i++)
            {
              tmp[i] = p_data[loc++];
            }
            m_AxisState.cmd_pos = conv::DwToInt(&tmp[0]);
            step = step_::act_pos;
          }
          break;
          case step_::act_pos:
          {
            uint8_t tmp[4] = { 0, };
            for (uint8_t i = 0; i < 4; i++)
            {
              tmp[i] = p_data[loc++];
            }
            m_AxisState.act_pos = conv::DwToInt(&tmp[0]);
            step = step_::pos_err;
          }
          break;
          case step_::pos_err:
          {
            uint8_t tmp[4] = { 0, };
            for (uint8_t i = 0; i < 4; i++)
            {
              tmp[i] = p_data[loc++];
            }
            m_AxisState.pos_err = conv::DwToInt(&tmp[0]);
            step = step_::act_vel;
          }
          break;
          case step_::act_vel:
          {
            uint8_t tmp[4] = { 0, };
            for (uint8_t i = 0; i < 4; i++)
            {
              tmp[i] = p_data[loc++];
            }
            m_AxisState.act_vel = conv::DwToInt(&tmp[0]);
            step = step_::axis_reg;
          }
          break;
          case step_::axis_reg:
          {
            for (uint8_t i = 0; i < 4; i++)
            {
              m_AxisState.axis_status[i] = p_data[loc++];
            }
          }
          break;
        }
      }
      update_AxisState();
    }
    break;
    case DEF_FASTECH_COMM_TYPE_GET_ALL_STATUS:
    {
      enum class step_{
        io_in,io_out, axis_reg,cmd_pos,act_pos,pos_err,act_vel,pt_item
      };
      //int step = 0;
      step_ step = step_::io_in;
      uint8_t loc = 0;
      while (loc < m_Packet.rx_packet.length)
      {
        switch (step)
        {
          case step_::io_in:
          {
            for (uint8_t i = 0; i < m_AxisState.io_in.GetSize(); i++)
            {
              m_AxisState.io_in[(uint8_t)i] = p_data[loc++];
            }
            step = step_::io_out;
          }
          break;
          case step_::io_out:
          {
            for (uint8_t i = 0; i < m_AxisState.io_out.GetSize(); i++)
            {
              m_AxisState.io_out[(uint8_t)i] = p_data[loc++];
            }
            step = step_::axis_reg;
          }
          break;
          case step_::axis_reg:
          {
            for (uint8_t i = 0; i < 4; i++)
            {
              m_AxisState.axis_status[i] = p_data[loc++];
            }
            step = step_::cmd_pos;
          }
          break;
          case step_::cmd_pos:
          {
            uint8_t tmp[4] = { 0, };
            for (uint8_t i = 0; i < 4; i++)
            {
              tmp[i] = p_data[loc++];
            }
            step = step_::act_pos;
            m_AxisState.cmd_pos = conv::DwToInt(&tmp[0]);
          }
          break;
          case step_::act_pos:
          {
            uint8_t tmp[4] = { 0, };
            for (uint8_t i = 0; i < 4; i++)
            {
              tmp[i] = p_data[loc++];
            }
            step = step_::pos_err;
            m_AxisState.act_pos = conv::DwToInt(&tmp[0]);
          }
          break;
          case step_::pos_err:
          {
            uint8_t tmp[4] = { 0, };
            for (uint8_t i = 0; i < 4; i++)
            {
              tmp[i] = p_data[loc++];
            }
            step = step_::act_vel;
            m_AxisState.pos_err = conv::DwToInt(&tmp[0]);
          }
          break;
          case step_::act_vel:
          {
            uint8_t tmp[4] = { 0, };
            for (uint8_t i = 0; i < 4; i++)
            {
              tmp[i] = p_data[loc++];
            }
            step = step_::pt_item;
            m_AxisState.act_vel = conv::DwToInt(&tmp[0]);
          }
          break;
          case step_::pt_item:
          {
            uint8_t tmp[4] = { 0, };
            for (uint8_t i = 0; i < 4; i++)
            {
              tmp[i] = p_data[loc++];
            }
            m_AxisState.pt_item = conv::DwToInt(&tmp[0]);
          }
          break;
        }
      }
      update_AxisState();
    }
    break;
    default:
      break;
  }
  m_Packet.check_sync.is_checked = false;

}

int enFastechMotor::update_AxisState()
{
  int ret = 0;
  m_AxisState.is_motor_on = m_AxisState.axis_status[DEF_FM_BANK_STATE_01][FM_REG_SERVO_ON];
  m_pApReg->status[AP_REG_BANK_RUN_STATE][AP_REG_MOTOR_ON] = m_AxisState.is_motor_on;

  return ret;
}

void enFastechMotor::SendGetAllStatus()
{
  cmdFastech_SendGetAllStatus(&m_Packet);
}


