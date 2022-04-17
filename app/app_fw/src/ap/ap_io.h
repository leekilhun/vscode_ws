/*
 * ap_io.h
 *
 *  Created on: 2022. 3. 1.
 *      Author: gns2l
 */

#ifndef SRC_AP_AP_IO_H_
#define SRC_AP_AP_IO_H_


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




class ap_io:public IIO
{
public:
  enum class in_e {
    mcu_in_cyl_1_on, mcu_in_cyl_1_off, mcu_in_cyl_2_on, mcu_in_cyl_2_off,
    mcu_in_drum_vac_on, mcu_in_drum_tail_vac_on, mcu_in_phonejig_vac_on, mcu_area_sensor,  // b0 mcu   in00 ~ 07
    in10, in11, in12, in13, in14, in15, in16, in17,  // b1 mcu   in10 ~ 17
    fm_in_cyl_3_on, fm_in_cyl_3_off, fm_in_cyl_4_on, fm_in_cyl_4_off,
    in24, in25, in26, in27,  // b2 motor in00 ~ 07
    in30, in31, in32, in33, in34, in35, in36, in37,  // b3 motor in10 ~ 17
    _max
  };

  enum class out_e {
    mcu_out_cyl_1_on, mcu_out_cyl_1_off, mcu_out_cyl_2_on, mcu_out_cyl_2_off,
    mcu_out_vac_1_on, mcu_out_vac_1_off, mcu_out_vac_2_on, mcu_out_vac_2_off,  // b0 mcu   out00 ~ 07
    mcu_lamp_start, mcu_lamp_stop, mcu_lamp_reset,
    out13, out14, out15, out16, out17,  // b1 mcu   out10 ~ 17
    fm_out_cyl_3_on, fm_out_cyl_3_off, fm_out_cyl_4_on, fm_out_cyl_4_off,
    fm_out_vac_3_on, fm_out_vac_3_off, fm_out_vac_4_on, fm_out_vac_4_off,  // b2 motor out00 ~ 07
    fm_out10, fm_out11, fm_out12, fm_out13,
    fm_out14, fm_out15, fm_out16, fm_out17,  // b3 motor out10 ~ 17
    _max
  };

public:
  ap_io(){}
  virtual ~ap_io(){}

  _Regist   in{AP_IO_DEF_BANK_NO_MAX};
  _Regist   out{AP_IO_DEF_BANK_NO_MAX};
  IIO* pIo;

  inline void assignObj(IIO* p_io){
    pIo = p_io;
  }

  inline int SetGpioOut(out_e out_idx, bool onoff){
    switch (out_idx)
    {
      case out_e::mcu_out_cyl_1_on:
        gpioPinWrite(_GPIO_IO_OUT_01, onoff);
        break;
      case out_e::mcu_out_cyl_1_off:
        gpioPinWrite(_GPIO_IO_OUT_02, onoff);
        break;
      case out_e::mcu_out_cyl_2_on:
        gpioPinWrite(_GPIO_IO_OUT_03, onoff);
        break;
      case out_e::mcu_out_cyl_2_off:
        gpioPinWrite(_GPIO_IO_OUT_04, onoff);
        break;
      case out_e::mcu_out_vac_1_on:
        gpioPinWrite(_GPIO_IO_OUT_05, onoff);
        break;
      case out_e::mcu_out_vac_1_off:
        gpioPinWrite(_GPIO_IO_OUT_06, onoff);
        break;
      case out_e::mcu_out_vac_2_on:
        gpioPinWrite(_GPIO_IO_OUT_07, onoff);
        break;
      case out_e::mcu_out_vac_2_off:
        gpioPinWrite(_GPIO_IO_OUT_08, onoff);
        break;
      case out_e::mcu_lamp_start:
        gpioPinWrite(_GPIO_OP_LAMP_START, onoff);
        break;
      case out_e::mcu_lamp_stop:
        gpioPinWrite(_GPIO_OP_LAMP_STOP, onoff);
        break;
      case out_e::mcu_lamp_reset:
        gpioPinWrite(_GPIO_OP_LAMP_RESET, onoff);
        break;
      case out_e::out13:
        break;
      case out_e::out14:
        break;
      case out_e::out15:
        break;
      case out_e::out16:
        break;
      case out_e::out17:
        break;
      case out_e::fm_out_cyl_3_on:
      case out_e::fm_out_cyl_3_off:
      case out_e::fm_out_cyl_4_on:
      case out_e::fm_out_cyl_4_off:
      case out_e::fm_out_vac_3_on:
      case out_e::fm_out_vac_3_off:
      case out_e::fm_out_vac_4_on:
      case out_e::fm_out_vac_4_off:
      case out_e::fm_out10:
      {
        if (onoff)
          pIo->OutputOn(static_cast<uint32_t>(out_idx) + AP_IO_DEF_IO_ADDR_FM_DIFF_VALUE);
        else
          pIo->OutputOff(static_cast<uint32_t>(out_idx) + AP_IO_DEF_IO_ADDR_FM_DIFF_VALUE);
      }
      break;
      case out_e::fm_out11:
        break;
      case out_e::fm_out12:
        break;
      case out_e::fm_out13:
        break;
      case out_e::fm_out14:
        break;
      case out_e::fm_out15:
        break;
      case out_e::fm_out16:
        break;
      case out_e::fm_out17:
        break;
      default:
        break;
    }
    return 0;
  }

  inline void Update_io(void){

    // out
    uint8_t gpio = gpioPinRead(_GPIO_IO_OUT_01) << 0
        | gpioPinRead(_GPIO_IO_OUT_02) << 1
        | gpioPinRead(_GPIO_IO_OUT_03) << 2
        | gpioPinRead(_GPIO_IO_OUT_04) << 3
        | gpioPinRead(_GPIO_IO_OUT_05) << 4
        | gpioPinRead(_GPIO_IO_OUT_06) << 5
        | gpioPinRead(_GPIO_IO_OUT_07) << 6
        | gpioPinRead(_GPIO_IO_OUT_08) << 7
        ;
    out[AP_IO_DEF_BANK_NO_0] = gpio;

    gpio = 0x0 ;
    gpio = gpioPinRead(_GPIO_OP_LAMP_START) << 0
        | gpioPinRead(_GPIO_OP_LAMP_STOP)  << 1
        | gpioPinRead(_GPIO_OP_LAMP_RESET) << 2
        //| gpioPinRead() << 3
        //| gpioPinRead() << 4
        //| gpioPinRead() << 5
        //is_motorOn | gpioPinRead() << 6
        //is_autoReady  | gpioPinRead() << 7
        ;

    out[AP_IO_DEF_BANK_NO_1] = gpio;

    gpio = 0x0 ;
    uint8_t fm_out_1 = pIo->GetBank_Out(AP_IO_DEF_BANK_NO_1);
    uint8_t fm_out_2 = pIo->GetBank_Out(AP_IO_DEF_BANK_NO_2);
    gpio = ((fm_out_1 & bit7)>>7)//<<0       //user_out_0
              |((fm_out_2 & bit0)<<1)//>>0)<<1   //user_out_1
              |((fm_out_2 & bit1)<<1)//>>1)<<2   //user_out_2
              |((fm_out_2 & bit2)<<1)//>>2)<<3   //user_out_3
              |((fm_out_2 & bit3)<<1)//>>3)<<4   //user_out_4
              |((fm_out_2 & bit4)<<1)//>>4)<<5   //user_out_5
              |((fm_out_2 & bit5)<<1)//>>5)<<6   //user_out_6
              |((fm_out_2 & bit6)<<1)//>>6)<<7   //user_out_7
              ;
    out[AP_IO_DEF_BANK_NO_2] = gpio;

    gpio = 0x0 ;
    gpio = (fm_out_2 & bit7)>>7 ; //user_out_0
    out[AP_IO_DEF_BANK_NO_3] = gpio;


    // in
    gpio = 0x0 ;
    gpio = gpioPinRead(_GPIO_IO_IN_01) << 0
        | gpioPinRead(_GPIO_IO_IN_02) << 1
        | gpioPinRead(_GPIO_IO_IN_03) << 2
        | gpioPinRead(_GPIO_IO_IN_04) << 3
        | gpioPinRead(_GPIO_IO_IN_05) << 4
        | gpioPinRead(_GPIO_IO_IN_06) << 5
        | gpioPinRead(_GPIO_IO_IN_07) << 6
        | gpioPinRead(_GPIO_IO_IN_08) << 7
        ;
    in[AP_IO_DEF_BANK_NO_0] = gpio;

    gpio = 0x0 ;
    gpio= gpioPinRead(_GPIO_OP_SW_START) << 0
        | gpioPinRead(_GPIO_OP_SW_STOP)  << 1
        | gpioPinRead(_GPIO_OP_SW_RESET) << 2
        | gpioPinRead(_GPIO_OP_SW_ESTOP) << 3
        //| gpioPinRead() << 4
        //| gpioPinRead() << 5
        //| gpioPinRead() << 6
        //| gpioPinRead() << 7
        ;
    in[AP_IO_DEF_BANK_NO_1] = gpio;


    in[AP_IO_DEF_BANK_NO_2][REG_BIT(0)] = pIo->IsOn(AP_IO_DEF_IO_ADDR_FM_USER_IN_0);
    in[AP_IO_DEF_BANK_NO_2][REG_BIT(1)] = pIo->IsOn(AP_IO_DEF_IO_ADDR_FM_USER_IN_1);
    in[AP_IO_DEF_BANK_NO_2][REG_BIT(2)] = pIo->IsOn(AP_IO_DEF_IO_ADDR_FM_USER_IN_2);
    in[AP_IO_DEF_BANK_NO_2][REG_BIT(3)] = pIo->IsOn(AP_IO_DEF_IO_ADDR_FM_USER_IN_3);
    in[AP_IO_DEF_BANK_NO_2][REG_BIT(4)] = pIo->IsOn(AP_IO_DEF_IO_ADDR_FM_USER_IN_4);
    in[AP_IO_DEF_BANK_NO_2][REG_BIT(5)] = pIo->IsOn(AP_IO_DEF_IO_ADDR_FM_USER_IN_5);
    in[AP_IO_DEF_BANK_NO_2][REG_BIT(6)] = pIo->IsOn(AP_IO_DEF_IO_ADDR_FM_USER_IN_6);
    in[AP_IO_DEF_BANK_NO_2][REG_BIT(7)] = pIo->IsOn(AP_IO_DEF_IO_ADDR_FM_USER_IN_7);

    in[AP_IO_DEF_BANK_NO_3][REG_BIT(0)] = pIo->IsOn(AP_IO_DEF_IO_ADDR_FM_USER_IN_8);

  }

  /* IO  함수*/
  inline void SetBank_Out(uint8_t bank, uint8_t val){
    uint8_t base = 0;
    base = out[bank].get();
    for(uint8_t i =0; i < 8; i++)
    {
      if ((((val>>i)^(base>>i))&bit(0)))
      {
        uint8_t check = val&bit(i);
        bool is_on = check > 0?true:false;
        SetGpioOut(static_cast<out_e>(8*bank + i), is_on);
      }
    }
    out[bank] = val;
  }

  inline uint8_t GetBank_Out(uint8_t bank){
    return out[bank].get();
  }


  inline uint8_t GetBank_In(uint8_t bank){
    return in[bank].get();
  }


  /* IO  control function */
  inline bool IsOn(uint32_t addr) {
    if (!(addr < static_cast<uint32_t>(ap_io::in_e::_max))) //not
      return false;
    div_t div_result = div(addr,8);
    return in[(uint8_t)div_result.quot][REG_BIT(div_result.rem)];
  }


  inline bool IsOff(uint32_t addr) {
    if (!(addr < static_cast<uint32_t>(ap_io::in_e::_max))) //not
      return false;
    div_t div_result = div(addr,8);
    return !(in[(uint8_t)div_result.quot][REG_BIT(div_result.rem)]);
  }

  inline int OutputOn(uint32_t addr) {
    if (!(addr < static_cast<uint32_t>(ap_io::out_e::_max))) //not
      return -1;
    return SetGpioOut(static_cast<ap_io::out_e>(addr), true);
  }

  inline int OutputOff(uint32_t addr) {
    if (!(addr < static_cast<uint32_t>(ap_io::out_e::_max))) //not
      return -1;
    return SetGpioOut(static_cast<ap_io::out_e>(addr), false);
  }

  inline int OutputToggle(uint32_t addr){
    if (!(addr < static_cast<uint32_t>(ap_io::out_e::_max))) //not
      return -1;
    div_t div_result = div(addr,8);//div_result.quot ; // 몫 //div_result.rem; // 나머지
    if (out[(uint8_t)div_result.quot][REG_BIT(div_result.rem)]){
      SetGpioOut(static_cast<ap_io::out_e>(addr), false);
    }
    else{
      SetGpioOut(static_cast<ap_io::out_e>(addr), true);
    }
    return 0;
  }


};


#endif /* SRC_AP_AP_IO_H_ */
