/*
 * ivacuum.h
 *
 *  Created on: 2022. 4. 7.
 *      Author: gns2l
 */
#pragma once

class ivacuum
{
public:
  enum time_e{ turn, settle };
  enum type_e{
    unkwn,
    suction,
    suction_blow,
  };;

  /*----------- Vacuum 상태 확인  -----------------------*/
  virtual bool IsOn() = 0;

  virtual bool IsOff() = 0;

  /*----------- Vacuum 동작  -----------------------*/
  virtual int On(bool skip_sensor = false) = 0;

  virtual int Off(bool skip_sensor = false) = 0;

  /*----------- Vacuum 동작 시작  -----------------------*/
  virtual int BlowOn() = 0;

  virtual int BlowOff() = 0;

  /*----------- Vacuum 동작완료시까지 Sleep  -------------------*/
  virtual int Wait4OnCplt() = 0;

  virtual int Wait4OffCplt() = 0;

  /*--------------- Common Interface -----------------------*/
  virtual int SetData(vac_dat::dat_t* p_data) = 0;

  virtual int GetData(vac_dat::dat_t* p_data) = 0;




};

