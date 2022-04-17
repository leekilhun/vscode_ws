#pragma once

/*
 * ap_io.h
 *
 *  Created on: 2022. 4. 7.
 *      Author: gns2l
 * 
 *  ap?? ???? io ???
 */

//#include "interface/iio.h"



//user io register
#define AP_IO_DEF_BANK_NO_0                       (uint8_t)0
#define AP_IO_DEF_BANK_NO_1                       (uint8_t)1
#define AP_IO_DEF_BANK_NO_2                       (uint8_t)2
#define AP_IO_DEF_BANK_NO_3                       (uint8_t)3
#define AP_IO_DEF_BANK_OUT_0                      AP_IO_DEF_BANK_NO_0
#define AP_IO_DEF_BANK_OUT_1                      AP_IO_DEF_BANK_NO_1
#define AP_IO_DEF_BANK_OUT_2                      AP_IO_DEF_BANK_NO_2
#define AP_IO_DEF_BANK_OUT_3                      AP_IO_DEF_BANK_NO_3
#define AP_IO_DEF_BANK_IN_0                       AP_IO_DEF_BANK_NO_0
#define AP_IO_DEF_BANK_IN_1                       AP_IO_DEF_BANK_NO_1
#define AP_IO_DEF_BANK_IN_2                       AP_IO_DEF_BANK_NO_2
#define AP_IO_DEF_BANK_IN_3                       AP_IO_DEF_BANK_NO_3
#define AP_IO_DEF_BANK_NO_MAX                     4

#define AP_IO_DEF_IO_ADDR_MCU_OUT_CYL_ON          0
#define AP_IO_DEF_IO_ADDR_MCU_OUT_1               1
#define AP_IO_DEF_IO_ADDR_MCU_OUT_2               2
#define AP_IO_DEF_IO_ADDR_MCU_OUT_3               3
#define AP_IO_DEF_IO_ADDR_MCU_OUT_4               4
#define AP_IO_DEF_IO_ADDR_MCU_OUT_5               5
#define AP_IO_DEF_IO_ADDR_MCU_OUT_6               6
#define AP_IO_DEF_IO_ADDR_MCU_OUT_7               7

#define AP_IO_DEF_IO_ADDR_MCU_OUT_LAMP_START      AP_IO_DEF_IO_ADDR_MCU_OUT_7 + 1
#define AP_IO_DEF_IO_ADDR_MCU_OUT_LAMP_STOP       AP_IO_DEF_IO_ADDR_MCU_OUT_7 + 2
#define AP_IO_DEF_IO_ADDR_MCU_OUT_LAMP_RESET      AP_IO_DEF_IO_ADDR_MCU_OUT_7 + 3
#define AP_IO_DEF_IO_ADDR_MCU_OUT_B               AP_IO_DEF_IO_ADDR_MCU_OUT_7 + 4
#define AP_IO_DEF_IO_ADDR_MCU_OUT_C               AP_IO_DEF_IO_ADDR_MCU_OUT_7 + 5
#define AP_IO_DEF_IO_ADDR_MCU_OUT_D               AP_IO_DEF_IO_ADDR_MCU_OUT_7 + 6
#define AP_IO_DEF_IO_ADDR_MCU_OUT_E               AP_IO_DEF_IO_ADDR_MCU_OUT_7 + 7
#define AP_IO_DEF_IO_ADDR_MCU_OUT_F               AP_IO_DEF_IO_ADDR_MCU_OUT_7 + 8

#define AP_IO_DEF_IO_ADDR_FM_DIFF_VALUE      -1
#define AP_IO_DEF_IO_ADDR_FM_USER_OUT_0      15
#define AP_IO_DEF_IO_ADDR_FM_USER_OUT_1      AP_IO_DEF_IO_ADDR_FM_USER_OUT_0 +1
#define AP_IO_DEF_IO_ADDR_FM_USER_OUT_2      AP_IO_DEF_IO_ADDR_FM_USER_OUT_0 +2
#define AP_IO_DEF_IO_ADDR_FM_USER_OUT_3      AP_IO_DEF_IO_ADDR_FM_USER_OUT_0 +3
#define AP_IO_DEF_IO_ADDR_FM_USER_OUT_4      AP_IO_DEF_IO_ADDR_FM_USER_OUT_0 +4
#define AP_IO_DEF_IO_ADDR_FM_USER_OUT_5      AP_IO_DEF_IO_ADDR_FM_USER_OUT_0 +5
#define AP_IO_DEF_IO_ADDR_FM_USER_OUT_6      AP_IO_DEF_IO_ADDR_FM_USER_OUT_0 +6
#define AP_IO_DEF_IO_ADDR_FM_USER_OUT_7      AP_IO_DEF_IO_ADDR_FM_USER_OUT_0 +7
#define AP_IO_DEF_IO_ADDR_FM_USER_OUT_8      AP_IO_DEF_IO_ADDR_FM_USER_OUT_0 +8

#define AP_IO_DEF_IO_ADDR_FM_USER_IN_0      26
#define AP_IO_DEF_IO_ADDR_FM_USER_IN_1      AP_IO_DEF_IO_ADDR_FM_USER_IN_0 +1
#define AP_IO_DEF_IO_ADDR_FM_USER_IN_2      AP_IO_DEF_IO_ADDR_FM_USER_IN_0 +2
#define AP_IO_DEF_IO_ADDR_FM_USER_IN_3      AP_IO_DEF_IO_ADDR_FM_USER_IN_0 +3
#define AP_IO_DEF_IO_ADDR_FM_USER_IN_4      AP_IO_DEF_IO_ADDR_FM_USER_IN_0 +4
#define AP_IO_DEF_IO_ADDR_FM_USER_IN_5      AP_IO_DEF_IO_ADDR_FM_USER_IN_0 +5
#define AP_IO_DEF_IO_ADDR_FM_USER_IN_6      AP_IO_DEF_IO_ADDR_FM_USER_IN_0 +6
#define AP_IO_DEF_IO_ADDR_FM_USER_IN_7      AP_IO_DEF_IO_ADDR_FM_USER_IN_0 +7
#define AP_IO_DEF_IO_ADDR_FM_USER_IN_8      AP_IO_DEF_IO_ADDR_FM_USER_IN_0 +8


class ap_io:public iio
{
public:
  enum class in_e
  {
    mcu_in_cyl_1_on,
    mcu_in_cyl_1_off,
    mcu_in_cyl_2_on,
    mcu_in_cyl_2_off,
    mcu_in_drum_vac_on,
    mcu_in_drum_tail_vac_on,
    mcu_in_phonejig_vac_on,
    mcu_area_sensor, // b0 mcu   in00 ~ 07
    in10,
    in11,
    in12,
    in13,
    in14,
    in15,
    in16,
    in17, // b1 mcu   in10 ~ 17
    fm_in_cyl_3_on,
    fm_in_cyl_3_off,
    fm_in_cyl_4_on,
    fm_in_cyl_4_off,
    in24,
    in25,
    in26,
    in27, // b2 motor in00 ~ 07
    in30,
    in31,
    in32,
    in33,
    in34,
    in35,
    in36,
    in37, // b3 motor in10 ~ 17
    _max
  };

  enum class out_e
  {
    mcu_out_cyl_1_on,
    mcu_out_cyl_1_off,
    mcu_out_cyl_2_on,
    mcu_out_cyl_2_off,
    mcu_out_vac_1_on,
    mcu_out_vac_1_off,
    mcu_out_vac_2_on,
    mcu_out_vac_2_off, // b0 mcu   out00 ~ 07
    mcu_lamp_start,
    mcu_lamp_stop,
    mcu_lamp_reset,
    out13,
    out14,
    out15,
    out16,
    out17, // b1 mcu   out10 ~ 17
    fm_out_cyl_3_on,
    fm_out_cyl_3_off,
    fm_out_cyl_4_on,
    fm_out_cyl_4_off,
    fm_out_vac_3_on,
    fm_out_vac_3_off,
    fm_out_vac_4_on,
    fm_out_vac_4_off, // b2 motor out00 ~ 07
    fm_out10,
    fm_out11,
    fm_out12,
    fm_out13,
    fm_out14,
    fm_out15,
    fm_out16,
    fm_out17, // b3 motor out10 ~ 17
    _max
  };

public:
  ap_io();
  virtual ~ap_io();

  fdata in{AP_IO_DEF_BANK_NO_MAX};
  fdata out{AP_IO_DEF_BANK_NO_MAX};
  iio* pIo;

  void assignObj(iio* p_io);
  int SetGpioOut(out_e out_idx, bool onoff);
  void Update_io(void);

  /* IO  ÇÔ¼ö*/
  void SetBank_Out(uint8_t bank, uint8_t val);

  uint8_t GetBank_Out(uint8_t bank);

  uint8_t GetBank_In(uint8_t bank);

  /* IO  control function */
  bool IsOn(uint32_t addr);

  bool IsOff(uint32_t addr);

  int OutputOn(uint32_t addr);

  int OutputOff(uint32_t addr);

  int OutputToggle(uint32_t addr);
};