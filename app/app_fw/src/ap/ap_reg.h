/*
 * ap_reg.h
 *
 *  Created on: 2022. 2. 22.
 *      Author: gns2l
 */

#ifndef SRC_AP_AP_REG_H_
#define SRC_AP_AP_REG_H_


/*
 *
 [ bank 0] error state [H]
  - bit 0          bit 1        bit 2       bit 3       bit 4       bit 5       bit 6       bit 7
    no error     no-resp LCD  no-resp Mot

 [ bank 1] error state [L]
  - bit 8          bit 9        bit 10      bit 11      bit 12      bit 13      bit 14      bit 15
    hw +limit    hw -limit    sw +limit    sw -limit

 [ bank 2]  setting
  - bit 16         bit 17       bit 18      bit 19      bit 20      bit 21      bit 22      bit 23
    use beep

 [ bank 3]  run state
  - bit 24         bit 25       bit 26      bit 27      bit 28      bit 29      bit 30      bit 31
    auto_run/stop  emg stop   auto_ready   error stop              motor-on   orgin cplt     alarm
 *
 */

#define AP_REG_ERR_CLEAR             REG_BIT(0)
#define AP_REG_ERR_NO_RESP_LCD       REG_BIT(1)
#define AP_REG_ERR_NO_RESP_MOT       REG_BIT(2)
#define AP_REG_ERR_B03               REG_BIT(3)
#define AP_REG_ERR_B04               REG_BIT(4)
#define AP_REG_ERR_B05               REG_BIT(5)
#define AP_REG_ERR_B06               REG_BIT(6)
#define AP_REG_ERR_B07               REG_BIT(7)

#define AP_REG_ERR_HW_PLUS_LIMIT     REG_BIT(0)
#define AP_REG_ERR_HW_MINU_LIMIT     REG_BIT(1)
#define AP_REG_ERR_SW_PLUS_LIMIT     REG_BIT(2)
#define AP_REG_ERR_SW_MINU_LIMIT     REG_BIT(3)
#define AP_REG_ERR_B14               REG_BIT(4)
#define AP_REG_ERR_B15               REG_BIT(5)
#define AP_REG_ERR_B16               REG_BIT(6)
#define AP_REG_ERR_B17               REG_BIT(7)

#define AP_REG_USE_BEEP              REG_BIT(0)
#define AP_REG_B21                   REG_BIT(1)
#define AP_REG_B22                   REG_BIT(2)
#define AP_REG_B23                   REG_BIT(3)
#define AP_REG_B24                   REG_BIT(4)
#define AP_REG_B25                   REG_BIT(5)
#define AP_REG_B26                   REG_BIT(6)
#define AP_REG_B27                   REG_BIT(7)

#define AP_REG_AUTO_RUNNING          REG_BIT(0)
#define AP_REG_EMG_STOP              REG_BIT(1)
#define AP_REG_AUTO_READY            REG_BIT(2)
#define AP_REG_ERROR_STOP            REG_BIT(3)
#define AP_REG_B34                   REG_BIT(4)
#define AP_REG_MOTOR_ON              REG_BIT(5)
#define AP_REG_ORG_COMPLETED         REG_BIT(6)
#define AP_REG_ALARM_STATUS          REG_BIT(7)

#define AP_REG_BANK_NO_MAX           (uint8_t)4
#define AP_REG_BANK_ERR_H            (uint8_t)0
#define AP_REG_BANK_ERR_L            (uint8_t)1
#define AP_REG_BANK_SETTING          (uint8_t)2
#define AP_REG_BANK_RUN_STATE        (uint8_t)3


#ifdef _USE_HW_RTOS
static osMutexId ap_reg_mutex_id;
#endif

#ifdef _USE_HW_RTOS
#define AP_REG_LOCK_BEGIN osMutexWait(ap_reg_mutex_id, osWaitForever)
#define AP_REG_LOCK_END   osMutexRelease(ap_reg_mutex_id)
#else
#define AP_REG_LOCK_BEGIN
#define AP_REG_LOCK_END
#endif


struct Ap_reg
{
  enum class err_t{
    no_error,
    no_response_lcd,
    no_response_mot,
    reserved_03,
    reserved_04,
    reserved_05,
    reserved_06,
    reserved_07,

    hw_plus_limit,
    hw_minus_limit,
    sw_plus_limit,
    sw_minus_limit,
    reserved_14,
    reserved_15,
    reserved_16,
    reserved_17,
    _max
  };
#define AP_ERR_CODE(x)          static_cast<uint8_t>(Ap_reg::err_t::x)


  _Regist   status{AP_REG_BANK_NO_MAX};
  uint8_t   bank_size;
  Ap_reg():bank_size(AP_REG_BANK_NO_MAX){
#ifdef _USE_HW_RTOS
    osMutexDef(ap_reg_mutex_id);
    ap_reg_mutex_id = osMutexCreate (osMutex(ap_reg_mutex_id));
#endif

  }
  /*
  inline bool Setter(uint8_t* p_data, uint8_t len){
    bool ret = true;
    if (len > AP_REG_BANK_NO_MAX)
      return ret;
    for (uint8_t i = 0; i < len; i++) {
      status[i] = p_data[i];
    }
    return ret;
  }*/

  inline bool IsAutoReadyMode(){
    return status[AP_REG_BANK_RUN_STATE][AP_REG_AUTO_READY];
  }

  inline bool IsAutoRunning(){
    return status[AP_REG_BANK_RUN_STATE][AP_REG_AUTO_RUNNING];
  }

  inline void SetConfReg(uint8_t data){
    status[AP_REG_BANK_SETTING]= data;
  }

  inline void SetRunState(reg_ bit, bool on_off = true){
    AP_REG_LOCK_BEGIN;
    uint8_t value = status[AP_REG_BANK_RUN_STATE].get();
    switch (bit) {
      case AP_REG_AUTO_RUNNING:
        if(on_off)
        {
          value = value | B00000001;
          value = value & B11110001;
        }
        else
        {
          value = value & B11110000;
        }

        break;

      case AP_REG_EMG_STOP:
        if(on_off)
        {
          value = value | B00000010;
          value = value & B11110010;
        }
        else
        {
          value = value & B11111101;
        }

        break;

      case AP_REG_AUTO_READY:
        value = value | B00000100;
        value = value & B11110100;
        break;

      case AP_REG_ERROR_STOP:
        value = value | B00001000;
        value = value & B11111000;
        break;

      case AP_REG_B34:
        if(on_off)
          value = value & B11111111;
        else
          value = value & B11101111;
        break;

      case AP_REG_MOTOR_ON:
        if(on_off)
          value = value | B00100000;
        else
          value = value & B11011111;
        break;

      case AP_REG_ORG_COMPLETED:
        if(on_off)
          value = value | B01000000;
        else
          value = value & B10111111;
        break;

      case AP_REG_ALARM_STATUS:
        if(on_off)
          value = value | B10000000;
        else
          value = value & B01111111;
        break;

      default:
        break;
    }
    status[AP_REG_BANK_RUN_STATE] = value;
    AP_REG_LOCK_END;
  }


};
#if 0

//user io register
#define IOMANAGER_DEF_BANK_NO_0                  (uint8_t)0
#define IOMANAGER_DEF_BANK_NO_1                  (uint8_t)1
#define IOMANAGER_DEF_BANK_NO_2                  (uint8_t)2
#define IOMANAGER_DEF_BANK_NO_3                  (uint8_t)3
#define IOMANAGER_DEF_BANK_NO_MAX                (uint8_t)4

#define IOMANAGER_DEF_IO_ADDR_FM_USER_OUT_0      15
#define IOMANAGER_DEF_IO_ADDR_FM_USER_OUT_1      IOMANAGER_DEF_IO_ADDR_FM_USER_OUT_0 +1
#define IOMANAGER_DEF_IO_ADDR_FM_USER_OUT_2      IOMANAGER_DEF_IO_ADDR_FM_USER_OUT_0 +2
#define IOMANAGER_DEF_IO_ADDR_FM_USER_OUT_3      IOMANAGER_DEF_IO_ADDR_FM_USER_OUT_0 +3
#define IOMANAGER_DEF_IO_ADDR_FM_USER_OUT_4      IOMANAGER_DEF_IO_ADDR_FM_USER_OUT_0 +4
#define IOMANAGER_DEF_IO_ADDR_FM_USER_OUT_5      IOMANAGER_DEF_IO_ADDR_FM_USER_OUT_0 +5
#define IOMANAGER_DEF_IO_ADDR_FM_USER_OUT_6      IOMANAGER_DEF_IO_ADDR_FM_USER_OUT_0 +6
#define IOMANAGER_DEF_IO_ADDR_FM_USER_OUT_7      IOMANAGER_DEF_IO_ADDR_FM_USER_OUT_0 +7
#define IOMANAGER_DEF_IO_ADDR_FM_USER_OUT_8      IOMANAGER_DEF_IO_ADDR_FM_USER_OUT_0 +8

#define IOMANAGER_DEF_IO_ADDR_FM_USER_IN_0      26
#define IOMANAGER_DEF_IO_ADDR_FM_USER_IN_1      IOMANAGER_DEF_IO_ADDR_FM_USER_IN_0 +1
#define IOMANAGER_DEF_IO_ADDR_FM_USER_IN_2      IOMANAGER_DEF_IO_ADDR_FM_USER_IN_0 +2
#define IOMANAGER_DEF_IO_ADDR_FM_USER_IN_3      IOMANAGER_DEF_IO_ADDR_FM_USER_IN_0 +3
#define IOMANAGER_DEF_IO_ADDR_FM_USER_IN_4      IOMANAGER_DEF_IO_ADDR_FM_USER_IN_0 +4
#define IOMANAGER_DEF_IO_ADDR_FM_USER_IN_5      IOMANAGER_DEF_IO_ADDR_FM_USER_IN_0 +5
#define IOMANAGER_DEF_IO_ADDR_FM_USER_IN_6      IOMANAGER_DEF_IO_ADDR_FM_USER_IN_0 +6
#define IOMANAGER_DEF_IO_ADDR_FM_USER_IN_7      IOMANAGER_DEF_IO_ADDR_FM_USER_IN_0 +7
#define IOMANAGER_DEF_IO_ADDR_FM_USER_IN_8      IOMANAGER_DEF_IO_ADDR_FM_USER_IN_0 +8

struct ioManager:public IIO
{


  ioManager(){
  }
  enum class in_e {
    in00, in01, in02, in03, in04, in05, in06, in07,  // b0 mcu   in00 ~ 07
    in10, in11, in12, in13, in14, in15, in16, in17,  // b1 mcu   in10 ~ 17
    in20, in21, in22, in23, in24, in25, in26, in27,  // b2 motor in00 ~ 07
    in30, in31, in32, in33, in34, in35, in36, in37,  // b3 motor in10 ~ 17
    _max
  };

  enum class out_e {
    mcu_out00, mcu_out01, mcu_out02, mcu_out03,
    mcu_out04, mcu_out05, mcu_out06, mcu_out07,  // b0 mcu   out00 ~ 07
    mcu_lamp_start, mcu_lamp_stop, mcu_lamp_reset,
    out13, out14, out15, out16, out17,  // b1 mcu   out10 ~ 17
    fm_out00, fm_out01, fm_out02, fm_out03,
    fm_out04, fm_out05, fm_out06, fm_out07,  // b2 motor out00 ~ 07
    fm_out10, fm_out11, fm_out12, fm_out13,
    fm_out14, fm_out15, fm_out16, fm_out17,  // b3 motor out10 ~ 17
    _max
  };

  _Regist   in{IOMANAGER_DEF_BANK_NO_MAX};
  _Regist   out{IOMANAGER_DEF_BANK_NO_MAX};
  IIO* pIo;

  inline void assignObj(IIO* p_io){
    pIo = p_io;
  }

  inline int SetGpioOut(out_e out_idx, bool onoff){
    switch (out_idx)
    {
      case out_e::mcu_out00:
        gpioPinWrite(_GPIO_IO_OUT_01, onoff);
        break;
      case out_e::mcu_out01:
        gpioPinWrite(_GPIO_IO_OUT_02, onoff);
        break;
      case out_e::mcu_out02:
        gpioPinWrite(_GPIO_IO_OUT_03, onoff);
        break;
      case out_e::mcu_out03:
        gpioPinWrite(_GPIO_IO_OUT_04, onoff);
        break;
      case out_e::mcu_out04:
        gpioPinWrite(_GPIO_IO_OUT_05, onoff);
        break;
      case out_e::mcu_out05:
        gpioPinWrite(_GPIO_IO_OUT_06, onoff);
        break;
      case out_e::mcu_out06:
        gpioPinWrite(_GPIO_IO_OUT_07, onoff);
        break;
      case out_e::mcu_out07:
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
      case out_e::fm_out00:
        if (onoff)
          pIo->OutputOff(static_cast<uint32_t>(enFastechMotor::out_def::user_out_0));
        else
          pIo->OutputOn(static_cast<uint32_t>(enFastechMotor::out_def::user_out_0));
        break;
      case out_e::fm_out01:
        if (onoff)
          pIo->OutputOff(static_cast<uint32_t>(enFastechMotor::out_def::user_out_1));
        else
          pIo->OutputOn(static_cast<uint32_t>(enFastechMotor::out_def::user_out_1));
        break;
      case out_e::fm_out12:
        if (onoff)
          pIo->OutputOff(static_cast<uint32_t>(enFastechMotor::out_def::user_out_2));
        else
          pIo->OutputOn(static_cast<uint32_t>(enFastechMotor::out_def::user_out_2));
        break;
      case out_e::fm_out13:
        if (onoff)
          pIo->OutputOff(static_cast<uint32_t>(enFastechMotor::out_def::user_out_3));
        else
          pIo->OutputOn(static_cast<uint32_t>(enFastechMotor::out_def::user_out_3));
        break;
      case out_e::fm_out14:
        if (onoff)
          pIo->OutputOff(static_cast<uint32_t>(enFastechMotor::out_def::user_out_4));
        else
          pIo->OutputOn(static_cast<uint32_t>(enFastechMotor::out_def::user_out_4));
        break;
      case out_e::fm_out15:
        if (onoff)
          pIo->OutputOff(static_cast<uint32_t>(enFastechMotor::out_def::user_out_5));
        else
          pIo->OutputOn(static_cast<uint32_t>(enFastechMotor::out_def::user_out_5));
        break;
      case out_e::fm_out16:
        if (onoff)
          pIo->OutputOff(static_cast<uint32_t>(enFastechMotor::out_def::user_out_6));
        else
          pIo->OutputOn(static_cast<uint32_t>(enFastechMotor::out_def::user_out_6));
        break;
      case out_e::fm_out17:
        if (onoff)
          pIo->OutputOff(static_cast<uint32_t>(enFastechMotor::out_def::user_out_7));
        else
          pIo->OutputOn(static_cast<uint32_t>(enFastechMotor::out_def::user_out_7));
        break;
      case out_e::fm_out10:
        if (onoff)
          pIo->OutputOff(static_cast<uint32_t>(enFastechMotor::out_def::user_out_8));
        else
          pIo->OutputOn(static_cast<uint32_t>(enFastechMotor::out_def::user_out_8));
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
    uint8_t gpio = gpioPinRead(_GPIO_IO_OUT_01) << 7
    | gpioPinRead(_GPIO_IO_OUT_02) << 6
    | gpioPinRead(_GPIO_IO_OUT_03) << 5
    | gpioPinRead(_GPIO_IO_OUT_04) << 4
    | gpioPinRead(_GPIO_IO_OUT_05) << 3
    | gpioPinRead(_GPIO_IO_OUT_06) << 2
    | gpioPinRead(_GPIO_IO_OUT_07) << 1
    | gpioPinRead(_GPIO_IO_OUT_08) << 0
    ;
    out[IOMANAGER_DEF_BANK_NO_0] = gpio;

    gpio = 0x0 ;
    gpio = gpioPinRead(_GPIO_OP_LAMP_START) << 7
        | gpioPinRead(_GPIO_OP_LAMP_STOP)  << 6
        | gpioPinRead(_GPIO_OP_LAMP_RESET) << 5
        //| gpioPinRead() << 4
        //| gpioPinRead() << 3
        //| gpioPinRead() << 2
        //is_motorOn | gpioPinRead() << 1
        //is_autoReady  | gpioPinRead() << 0
        ;

    out[IOMANAGER_DEF_BANK_NO_1] = gpio;

    gpio = 0x0 ;
    uint8_t fm_out_1 = pIo->GetBank_Out(IOMANAGER_DEF_BANK_NO_1);
    uint8_t fm_out_2 = pIo->GetBank_Out(IOMANAGER_DEF_BANK_NO_2);
    gpio = (fm_out_1 & bit7) /*>>7)<<7*/   //user_out_0
                |((fm_out_2 & bit0)>>0)<<6             //user_out_1
                |((fm_out_2 & bit1)>>1)<<5             //user_out_2
                |((fm_out_2 & bit2)>>2)<<4             //user_out_3
                |((fm_out_2 & bit3)>>3)<<3             //user_out_4
                |((fm_out_2 & bit4)>>4)<<2             //user_out_5
                |((fm_out_2 & bit5)>>5)<<1             //user_out_6
                |((fm_out_2 & bit6)>>6)<<0             //user_out_7
                ;
    out[IOMANAGER_DEF_BANK_NO_2] = gpio;

    gpio = 0x0 ;
    gpio = (fm_out_2 & bit7) << 7;
    out[IOMANAGER_DEF_BANK_NO_3] = gpio;


    // in
    gpio = 0x0 ;
    gpio = gpioPinRead(_GPIO_IO_IN_01) << 7
        | gpioPinRead(_GPIO_IO_IN_02) << 6
        | gpioPinRead(_GPIO_IO_IN_03) << 5
        | gpioPinRead(_GPIO_IO_IN_04) << 4
        | gpioPinRead(_GPIO_IO_IN_05) << 3
        | gpioPinRead(_GPIO_IO_IN_06) << 2
        | gpioPinRead(_GPIO_IO_IN_07) << 1
        | gpioPinRead(_GPIO_IO_IN_08) << 0
        ;
    in[IOMANAGER_DEF_BANK_NO_0] = gpio;

    gpio = 0x0 ;
    gpio= gpioPinRead(_GPIO_OP_SW_START) << 7
        | gpioPinRead(_GPIO_OP_SW_STOP)  << 6
        | gpioPinRead(_GPIO_OP_SW_RESET) << 5
        | gpioPinRead(_GPIO_OP_SW_ESTOP) << 4
        //| gpioPinRead() << 3
        //| gpioPinRead() << 2
        //| gpioPinRead() << 1
        //| gpioPinRead()
        ;
    in[IOMANAGER_DEF_BANK_NO_1] = gpio;


    in[IOMANAGER_DEF_BANK_NO_2][REG_BIT(0)] = pIo->IsOn(IOMANAGER_DEF_IO_ADDR_FM_USER_IN_0);
    in[IOMANAGER_DEF_BANK_NO_2][REG_BIT(1)] = pIo->IsOn(IOMANAGER_DEF_IO_ADDR_FM_USER_IN_1);
    in[IOMANAGER_DEF_BANK_NO_2][REG_BIT(2)] = pIo->IsOn(IOMANAGER_DEF_IO_ADDR_FM_USER_IN_2);
    in[IOMANAGER_DEF_BANK_NO_2][REG_BIT(3)] = pIo->IsOn(IOMANAGER_DEF_IO_ADDR_FM_USER_IN_3);
    in[IOMANAGER_DEF_BANK_NO_2][REG_BIT(4)] = pIo->IsOn(IOMANAGER_DEF_IO_ADDR_FM_USER_IN_4);
    in[IOMANAGER_DEF_BANK_NO_2][REG_BIT(5)] = pIo->IsOn(IOMANAGER_DEF_IO_ADDR_FM_USER_IN_5);
    in[IOMANAGER_DEF_BANK_NO_2][REG_BIT(6)] = pIo->IsOn(IOMANAGER_DEF_IO_ADDR_FM_USER_IN_6);
    in[IOMANAGER_DEF_BANK_NO_2][REG_BIT(7)] = pIo->IsOn(IOMANAGER_DEF_IO_ADDR_FM_USER_IN_7);

    in[IOMANAGER_DEF_BANK_NO_3][REG_BIT(0)] = pIo->IsOn(IOMANAGER_DEF_IO_ADDR_FM_USER_IN_8);

  }

  /* IO  함수*/
  inline void SetBank_Out(uint8_t bank, uint8_t val){
    out[bank] = val;
  }

  inline uint8_t GetBank_Out(uint8_t bank) {
    return out[bank].get();
  }

  inline uint8_t GetBank_In(uint8_t bank) {
    return in[bank].get();
  }


  /* IO  control function */
  inline bool IsOn(uint32_t addr) {
    if (!(addr < static_cast<uint32_t>(ioManager::in_e::_max))) //not
      return -1;
    div_t div_result = div(addr,8);
    return in[(uint8_t)div_result.quot][static_cast<reg_>(div_result.rem)];
  }

  inline bool IsOff(uint32_t addr) {
    if (!(addr < static_cast<uint32_t>(ioManager::in_e::_max))) //not
      return -1;
    div_t div_result = div(addr,8);
    return !(in[(uint8_t)div_result.quot][static_cast<reg_>(div_result.rem)]);
  }

  inline int OutputOn(uint32_t addr) {
    if (!(addr < static_cast<uint32_t>(ioManager::out_e::_max))) //not
      return -1;
    return SetGpioOut(static_cast<ioManager::out_e>(addr), true);
  }

  inline int OutputOff(uint32_t addr) {
    if (!(addr < static_cast<uint32_t>(ioManager::out_e::_max))) //not
      return -1;
    return SetGpioOut(static_cast<ioManager::out_e>(addr), false);
  }

  inline int OutputToggle(uint32_t addr) {
    if (!(addr < static_cast<uint32_t>(ioManager::out_e::_max))) //not
      return -1;
    div_t div_result = div(addr,8);//div_result.quot ; // 몫 //div_result.rem; // 나머지
    if (out[(uint8_t)div_result.quot][static_cast<reg_>(div_result.rem)]){
      SetGpioOut(static_cast<ioManager::out_e>(addr), false);
    }
    else{
      SetGpioOut(static_cast<ioManager::out_e>(addr), true);
    }
    return 0;
  }


};
#endif

#endif /* SRC_AP_AP_REG_H_ */
