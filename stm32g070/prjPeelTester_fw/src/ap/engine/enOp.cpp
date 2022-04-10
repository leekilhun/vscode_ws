#include "ap.h"
#include "enOp.h"

enOp::enOp() : m_status(), m_mode(), m_pin()
{
  m_status = enOp::status::INIT;
  m_mode = enOp::mode::STOP;
}
enOp::~enOp()
{
}

int enOp::Init(enOp::cfg_t* cfg)
{
  m_pin[static_cast<uint8_t>(enOp::panel::SW_START)] = cfg->sw_pin_start;
  m_pin[static_cast<uint8_t>(enOp::panel::SW_STOP)] = cfg->sw_pin_stop;
  m_pin[static_cast<uint8_t>(enOp::panel::SW_RESET)] = cfg->sw_pin_reset;
  m_pin[static_cast<uint8_t>(enOp::panel::SW_ESTOP)] = cfg->sw_pin_estop;

  return 0;
}

enOp::status enOp::GetStatus() const
{
  return m_status;
}

void enOp::SetStatus(enOp::status state)
{
  m_status = state;
}

enOp::mode enOp::GetMode() const
{
  return m_mode;
}

void enOp::SetMode(enOp::mode md)
{
  m_mode = md;
}

bool enOp::GetPressed(enOp::panel op_sw)
{
  uint8_t ch = 0;
  switch (op_sw)
  {
  case enOp::panel::SW_START:
    ch = m_pin[static_cast<uint8_t>(enOp::panel::SW_START)];
    break;

  case enOp::panel::SW_STOP:
    ch = m_pin[static_cast<uint8_t>(enOp::panel::SW_STOP)];
    break;

  case enOp::panel::SW_RESET:
    ch = m_pin[static_cast<uint8_t>(enOp::panel::SW_RESET)];
    break;

  case enOp::panel::SW_ESTOP:
    ch = m_pin[static_cast<uint8_t>(enOp::panel::SW_ESTOP)];
    break;
    
  default:
    break;
  }
  return gpioPinRead(ch);
}