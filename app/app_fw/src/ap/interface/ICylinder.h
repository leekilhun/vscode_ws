/*
 * ICylinder.h
 *
 *  Created on: 2022. 3. 1.
 *      Author: gns2l
 */

#ifndef SRC_AP_INTERFACE_ICYLINDER_H_
#define SRC_AP_INTERFACE_ICYLINDER_H_


class ICylinder
{
public:
  enum time_e{ move, settle };
  enum type_e{
    unknwn,
    up_down,
    left_right,
    forward_backward,
    turn_return,
    lock_unlock,
    open_close,
    up_mid_down,
    left_mid_right,
    front_mid_back,
  };
  enum sol_e{
    unkwn, one, reserse_one, two
  };

  /*----------- 실린더 상태 확인  -----------------------*/

  virtual bool IsUp() = 0;

  virtual bool IsDown() = 0;

  virtual bool IsLeft() = 0;

  virtual bool IsRight() = 0;

  virtual bool IsForward() = 0;

  virtual bool IsBackward() = 0;

  virtual bool IsOpen() = 0;

  virtual bool IsClose() = 0;

  virtual bool IsLock() = 0;

  virtual bool IsUnlock() = 0;

  virtual bool IsTurn() = 0;

  virtual bool IsReturn() = 0;

  /*----------- 실린더 이동   -----------------------*/

  virtual int Up(bool skip_sensor = false) = 0;

  virtual int Down(bool skip_sensor = false) = 0;

  virtual int Left(bool skip_sensor = false) = 0;

  virtual int Right(bool skip_sensor = false) = 0;

  virtual int Forward(bool skip_sensor = false) = 0;

  virtual int Backward(bool skip_sensor = false)  = 0;

  virtual int Open(bool skip_sensor = false)  = 0;

  virtual int Close(bool skip_sensor = false)  = 0;

  virtual int Lock(bool skip_sensor = false)  = 0;

  virtual int Unlock(bool skip_sensor = false)  = 0;

  virtual int Return(bool skip_sensor = false)  = 0;

  virtual int Turn(bool skip_sensor = false)  = 0;

  /*----------- 실린더 이동 시작  -----------------------*/
  virtual int Start() = 0;

  /*---------- 실린더 동작완료때까지 Sleep   --------------*/
  virtual int Wait4UpCplt() = 0;
  virtual int Wait4DownCplt() = 0;

  /*------------------------ Common Interface -----------------------*/

  virtual int SetData(cyl_dat::dat_t* p_data) = 0;

  virtual int GetData(cyl_dat::dat_t* p_data) = 0;

};



#endif /* SRC_AP_INTERFACE_ICYLINDER_H_ */
