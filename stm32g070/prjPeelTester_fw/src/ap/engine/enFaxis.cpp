#include "ap.h"
#include "enFaxis.h"

#define EN_FAXIS_WAIT_TIMEOUT                         100
#define EN_FAXIS_RETRAY_CNT_MAX                       3
enFaxis::enFaxis (uint8_t id):m_AxisId(id)
{
  m_pApReg = NULL;
  m_pApIo = NULL;
  m_pApDat = NULL;
  m_pApCfgDat = NULL;
  m_Packet.motor_id = m_AxisId;
  m_IsConnected = false;
}

enFaxis::  ~enFaxis ()
{
 // TODO Auto-generated destructor stub
}

int enFaxis::Init(enFaxis::cfg_t* cfg)
{
  cmdFastech_Init(&m_Packet);
  cmdFastech_Open(&m_Packet, cfg->ch, cfg->baud);

  m_pApReg = cfg->p_apReg;
  m_pApCfgDat =cfg->p_apCfgDat;
  m_pApIo = cfg->p_apIo;
  m_pApDat = cfg->p_apDat;

  return 0;
}


void enFaxis::CommRecovery()
{
  cmdFastech_Recovery(&m_Packet);
  m_Packet.check_sync.is_checked = true;
}

bool enFaxis::IsBusy()
{
  return m_AxisState.axis_status[DEF_FM_BANK_STATE_02][FM_REG_MOTIONING];
}



int enFaxis::SetMotorParameter(uint8_t parm_no, int value)
{
  int ret = 0;
  uint8_t data[6] = {0,};
  data[0] = DEF_FASTECH_COMM_TYPE_SET_RAM_PARAM;
  data[1] = parm_no;
  data[2] = (uint8_t)(value>>0);
  data[3] = (uint8_t)(value>>8);
  data[4] = (uint8_t)(value>>16);
  data[5] = (uint8_t)(value>>24);

  cmdFastech_SendCmd(&this->m_Packet,&data[0],6);
  uint32_t pass_ms = millis();
  uint8_t retry_cnt = 0;

  m_Packet.rx_packet.response = 0xfe;
  while(m_Packet.rx_packet.response == 0xfe)
  {
    if ((millis() - pass_ms) > EN_FAXIS_WAIT_TIMEOUT)
    {
      if (retry_cnt++ == EN_FAXIS_RETRAY_CNT_MAX)
      {
        ret = -1;
        break;
      }
      else
        cmdFastech_SendCmd(&this->m_Packet,&data[0],6);
      pass_ms = millis();
    }
  }


  return ret;
}

int enFaxis::GetMotorParameter(uint8_t parm_no)
{
  int ret = 0;
  uint8_t data[2] = {0,};
  data[0] = DEF_FASTECH_COMM_TYPE_GET_RAM_PARAM;
  data[1] = parm_no;

  cmdFastech_SendCmd(&this->m_Packet,&data[0],2);
  uint32_t pass_ms = millis();
  uint8_t retry_cnt = 0;

  m_Packet.rx_packet.response = 0xfe;
  while(m_Packet.rx_packet.response == 0xfe)
  {
    if ((millis() - pass_ms) > EN_FAXIS_WAIT_TIMEOUT)
    {
      if (retry_cnt++ == EN_FAXIS_RETRAY_CNT_MAX)
        break;
      else
        ret = cmdFastech_SendCmd(&this->m_Packet,&data[0],2);
      pass_ms = millis();
    }
  }

  ret = utilDwToInt(m_Packet.rx_packet.data);

  return ret;
}

int enFaxis::MoveOrigin()
{
  bool ret;
  if (!IsMotorOn())
    return -1;
  uint8_t data[1] ={0};
  data[0] = DEF_FASTECH_COMM_TYPE_MOVE_ORG_SINGLE_AXIS;

  ret = cmdFastech_SendCmd(&this->m_Packet,&data[0],1);
  uint32_t pass_ms = millis();
  uint8_t retry_cnt = 0;

  m_Packet.rx_packet.response = 0xfe;
  while(m_Packet.rx_packet.response)
  {
    if ((millis() - pass_ms) > EN_FAXIS_WAIT_TIMEOUT)
    {
      if (retry_cnt++ == EN_FAXIS_RETRAY_CNT_MAX)
        break;
      else
        ret = cmdFastech_SendCmd(&this->m_Packet,&data[0],1);
      pass_ms = millis();
    }
  }

  return (ret ? 0 : -1);
}

int enFaxis::MotorOnOff(bool on_off)
{
  bool ret;
  uint8_t data[2] ={0,};
  data[0] = DEF_FASTECH_COMM_TYPE_SERVO_ENABLE;
  data[1] = (uint8_t)on_off;
  ret = cmdFastech_SendCmd(&this->m_Packet,&data[0],2);

  return (ret ? 0 : -1);
}

int enFaxis::Move (int cmd_pos, uint32_t cmd_vel, uint32_t acc, uint32_t decel)
{
  bool ret;
  if (!IsMotorOn())
    return -1;

  int conv_pos = 0;
  uint32_t conv_vel = 0;
  ap_dat::dat_t cfg = m_pApCfgDat->apcfg_dat[static_cast<uint8_t>(ap_dat::addr_e::ap_mot_scale)];
  uint16_t turn_per_pulse = cfg.parm1;
  uint16_t turn_per_move_mm = cfg.parm2;

  //conv_pos = (((cmd_pos)/(int)turn_per_move_mm)*(int)turn_per_pulse)/1000;
  conv_pos = (((float)(cmd_pos)/(float)turn_per_move_mm)*(int)turn_per_pulse)/1000;
  conv_vel = (uint32_t)(((float)cmd_vel/(float)60)*(float)turn_per_pulse);

  uint8_t data[9] ={0};
  data[0] = DEF_FASTECH_COMM_TYPE_MOVE_ABS_SINGLE_AXIS;
  data[1] = (uint8_t)(conv_pos>>0);
  data[2] = (uint8_t)(conv_pos>>8);
  data[3] = (uint8_t)(conv_pos>>16);
  data[4] = (uint8_t)(conv_pos>>24);
  data[5] = (uint8_t)(conv_vel>>0);
  data[6] = (uint8_t)(conv_vel>>8);
  data[7] = (uint8_t)(conv_vel>>16);
  data[8] = (uint8_t)(conv_vel>>24);
  ret = cmdFastech_SendCmd(&this->m_Packet,&data[0],9);
  uint32_t pass_ms = millis();
  uint8_t retry_cnt = 0;

  m_Packet.rx_packet.response = 0xfe;
  while(m_Packet.rx_packet.response)
  {
    if ((millis() - pass_ms) > EN_FAXIS_WAIT_TIMEOUT)
    {
      if (retry_cnt++ == EN_FAXIS_RETRAY_CNT_MAX)
        break;
      else
        ret = cmdFastech_SendCmd(&this->m_Packet,&data[0],9);
      pass_ms = millis();
    }
  }
  return (ret ? 0 : -1);
}

int enFaxis::MoveToLimit(uint32_t cmd_vel, bool is_cw)
{
  bool ret;
  if (!IsMotorOn())
    return -1;

  uint8_t data[6] ={0};
  data[0] = DEF_FASTECH_COMM_TYPE_MOVE_TO_LIMIT;
  data[1] = (uint8_t)(cmd_vel>>0);
  data[2] = (uint8_t)(cmd_vel>>8);
  data[3] = (uint8_t)(cmd_vel>>16);
  data[4] = (uint8_t)(cmd_vel>>24);
  data[5] = (uint8_t)(is_cw);
  ret = cmdFastech_SendCmd(&this->m_Packet,&data[0],6);

  return (ret ? 0 : -1);

}

int enFaxis::JogMove(uint32_t cmd_vel,bool is_cw)
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

int enFaxis::JogMove(uint32_t jog_pos, uint32_t cmd_vel,bool is_cw)
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
int enFaxis::Stop ()
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
    if ((millis() - pass_ms) > EN_FAXIS_WAIT_TIMEOUT)
    {
      if (retry_cnt++ == EN_FAXIS_RETRAY_CNT_MAX)
        break;
      else
        ret = cmdFastech_SendCmd(&this->m_Packet,&data[0],1);

      pass_ms = millis();
    }
  }
  return (ret ? 0 : -1);
}


int enFaxis::VStop ()
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
    if ((millis() - pass_ms) > EN_FAXIS_WAIT_TIMEOUT)
    {
      if (retry_cnt++ == EN_FAXIS_RETRAY_CNT_MAX)
        break;  
      else
        ret = cmdFastech_SendCmd(&this->m_Packet,&data[0],1);
      pass_ms = millis();
    }
  }

  return (ret ? 0 : -1);
}

int enFaxis::EStop ()
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
    if ((millis() - pass_ms) > EN_FAXIS_WAIT_TIMEOUT)
    {
      if (retry_cnt++ == EN_FAXIS_RETRAY_CNT_MAX)
        break;
      else
        ret = cmdFastech_SendCmd(&this->m_Packet,&data[0],1);
      pass_ms = millis();
    }
  }

  return (ret ? 0 : -1);
}

int enFaxis::ClearPos()
{
  bool ret;
  if (IsMotorOn() != false)
    return -1;
  uint8_t data[1] ={0};
  data[0] = DEF_FASTECH_COMM_TYPE_CLEAR_POSITION;
  ret = cmdFastech_SendCmd(&this->m_Packet,&data[0],1);

  return (ret ? 0 : -1);
}


bool enFaxis::IsOriginOK()
{
  return m_AxisState.axis_status[DEF_FM_BANK_STATE_02][FM_REG_ORG_RETURN_OK];
}

bool enFaxis::IsMotorOn()
{
  return m_AxisState.axis_status[DEF_FM_BANK_STATE_01][FM_REG_SERVO_ON]/*m_AxisState.is_motor_on*/;
}

int enFaxis::WaitDone(uint8_t mode)
{
  return 0;
}

bool enFaxis::IsAxisDone()
{
  bool ret = false;
  ret |= m_AxisState.axis_status[DEF_FM_BANK_STATE_01][FM_REG_INPOSIOTION];
  ret |= (m_AxisState.act_pos == m_AxisState.cmd_pos);
  return ret;
}

int enFaxis::ClearState()
{
  return 0;
}


bool enFaxis::set_UserOutput(uint32_t mask_on, uint32_t mask_off)
{
  bool ret = false;
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

  ret = cmdFastech_SendCmd(&this->m_Packet,&data[0],9);
  uint32_t pass_ms = millis();
  uint8_t retry_cnt = 0;

  m_Packet.rx_packet.response = 0xfe;
  while(m_Packet.rx_packet.response)
  {
    if ((millis() - pass_ms) > EN_FAXIS_WAIT_TIMEOUT)
    {
      if (retry_cnt++ == EN_FAXIS_RETRAY_CNT_MAX)
        break;
      else
        ret = cmdFastech_SendCmd(&this->m_Packet,&data[0],9);
      pass_ms = millis();
    }
  }
  return ret;
}


void enFaxis::ProcessCmd()
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

int enFaxis::update_AxisState()
{
  int ret = 0;
  m_AxisState.is_motor_on = m_AxisState.axis_status[DEF_FM_BANK_STATE_01][FM_REG_SERVO_ON];
  m_pApReg->status[AP_REG_BANK_RUN_STATE][AP_REG_MOTOR_ON] = m_AxisState.is_motor_on;

  return ret;
}

void enFaxis::SendGetAllStatus()
{
  cmdFastech_SendGetAllStatus(&m_Packet);
}

void enFaxis::SetBank_Out(uint8_t bank, uint8_t val)
{
  m_AxisState.io_out[bank] = val;
}

uint8_t enFaxis::GetBank_Out(uint8_t bank)
{
  return m_AxisState.io_out[bank].get();
  ;
}

uint8_t enFaxis::GetBank_In(uint8_t bank)
{
  return m_AxisState.io_in[bank].get();
}

/* IO  control function */
bool enFaxis::IsOn(uint32_t addr)
{
  bool ret = false;
  if (scope::between(addr, 0, static_cast<uint32_t>(in_def::_max)))
  {

    div_t div_result = div(addr, 8);
    ret = m_AxisState.io_in[(uint8_t)div_result.quot][REG_BIT(div_result.rem)];
  }
  return ret;
}

bool enFaxis::IsOff(uint32_t addr)
{
  bool ret = false;
  if (scope::between(addr, 0, static_cast<uint32_t>(in_def::_max)))
  {
    ret = IsOn(addr);
  }
  return ret;
}

int enFaxis::OutputOn(uint32_t addr)
{
  int ret = -1;
  if (scope::between(addr, static_cast<uint32_t>(out_def::user_out_0), static_cast<uint32_t>(out_def::_max)))
  {

    set_UserOutput(DEF_EN_FM_IO_USER_OUT(addr), 0);

    div_t div_result = div(addr, 8);
    m_AxisState.io_out[(uint8_t)div_result.quot][REG_BIT(div_result.rem)] = true;
    ret = 0;
  }
  return ret;
}

int enFaxis::OutputOff(uint32_t addr)
{
  int ret = -1;
  if (scope::between(addr, static_cast<uint32_t>(out_def::user_out_0), static_cast<uint32_t>(out_def::_max)))
  {

    set_UserOutput(0, DEF_EN_FM_IO_USER_OUT(addr));

    div_t div_result = div(addr, 8);
    m_AxisState.io_out[(uint8_t)div_result.quot][REG_BIT(div_result.rem)] = false;
    ret = 0;
  }
  return ret;
}

int enFaxis::OutputToggle(uint32_t addr)
{
  int ret = -1;
  if (scope::between(addr, static_cast<uint32_t>(out_def::user_out_0), static_cast<uint32_t>(out_def::_max)))
  {
    div_t div_result = div(addr, 8);
    if (m_AxisState.io_out[(uint8_t)div_result.quot][REG_BIT(div_result.rem)])
    {
      set_UserOutput(0, DEF_EN_FM_IO_USER_OUT(addr));
      m_AxisState.io_out[(uint8_t)div_result.quot][REG_BIT(div_result.rem)] = false;
    }
    else
    {
      set_UserOutput(DEF_EN_FM_IO_USER_OUT(addr), 0);
      m_AxisState.io_out[(uint8_t)div_result.quot][REG_BIT(div_result.rem)] = true;
    }
    ret = 0;
  }
  return ret;
}