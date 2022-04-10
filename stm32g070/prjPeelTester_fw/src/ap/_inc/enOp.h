/*
 * enOp.h
 *
 *  Created on: 2022. 4. 7.
 *      Author: gns2l
 * 
 *  operating panel switch 입력 처리 클래스
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
  enOp();
  virtual ~enOp();
  int Init(enOp::cfg_t* cfg);
  enOp::status GetStatus() const;

  void SetStatus(enOp::status state);

  enOp::mode GetMode() const;

  void SetMode(enOp::mode md);

  bool GetPressed(enOp::panel op_sw);

};
