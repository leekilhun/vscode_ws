/*
 * IAxis.h
 *
 *  Created on: 2022. 2. 24.
 *      Author: gns2l
 */

#ifndef SRC_AP_INTERFACE_IAXIS_H_
#define SRC_AP_INTERFACE_IAXIS_H_


class IAxis
{
public:
  virtual int Move (int cmd_pos, uint32_t cmd_vel = 100, uint32_t acc = 100, uint32_t decel = 100) = 0;
  //축을 정지한다. (한개의 축에 대한 정지)
  virtual int Stop () = 0;
  //축을 등속이동에 대해 정지한다.
  virtual int VStop () = 0;
  // 축을 비상정지한다.
  virtual int EStop ()= 0;

  virtual int JogMove(uint32_t cmd_vel = 1000, bool is_cw = true) = 0;
  virtual int JogMove(uint32_t jog_pos, uint32_t cmd_vel = 1000, bool is_cw = true) = 0;

  virtual int MotorOnOff(bool on_off) = 0;
  virtual bool IsMotorOn() = 0;
  virtual int WaitDone(uint8_t mode) = 0;
  virtual bool IsAxisDone() = 0;
  virtual int  ClearState() = 0;



};



#endif /* SRC_AP_INTERFACE_IAXIS_H_ */
