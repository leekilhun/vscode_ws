/*
 * enOp.h
 *
 *  Created on: 2022. 4. 7.
 *      Author: gns2l
 * 
 *  operating panel switch
 *  [sw start][sw stop][sw reset][sw estop]
 */
#pragma once

class enOp
{
public:
  enum class mode
  {
    READY,
    AUTORUN,
    STOP,
    DRY_RUN,
  };

  enum class status
  {
    INIT,
    ERR_STOP,
    STEP_STOP,
    RUN_READY,
    RUN,
  };

  enum class panel
  {
    SW_START,
    SW_STOP,
    SW_RESET,
    SW_ESTOP,
    SW_MAX
  };

  struct cfg_t
  {
    uint8_t sw_pin_start;
    uint8_t sw_pin_stop;
    uint8_t sw_pin_reset;
    uint8_t sw_pin_estop;
  };

private:
  enOp::status m_status;
  enOp::mode m_mode;
  uint8_t m_pin[static_cast<uint8_t>(enOp::panel::SW_MAX)];

public:
  enOp(){
    m_status = enOp::status::INIT;
    m_mode = enOp::mode::STOP;
  }
  virtual ~enOp(){}
  inline int Init(enOp::cfg_t* cfg){
    m_pin[static_cast<uint8_t>(enOp::panel::SW_START)] = cfg->sw_pin_start;
    m_pin[static_cast<uint8_t>(enOp::panel::SW_STOP)] = cfg->sw_pin_stop;
    m_pin[static_cast<uint8_t>(enOp::panel::SW_RESET)] = cfg->sw_pin_reset;
    m_pin[static_cast<uint8_t>(enOp::panel::SW_ESTOP)] = cfg->sw_pin_estop;

    return 0;
  }

  inline enOp::status GetStatus() const{
    return m_status;
  }

  inline void SetStatus(enOp::status state){
    m_status = state;
  }

  inline enOp::mode GetMode() const{
    return m_mode;
  }

  inline void SetMode(enOp::mode md){
    m_mode = md;
  }

  inline bool GetPressed(enOp::panel op_sw){
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

};
