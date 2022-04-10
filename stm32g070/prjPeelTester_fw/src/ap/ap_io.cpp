#include "ap_def.h"
#include "ap_io.h"

ap_io::ap_io() 
{

}
ap_io::~ap_io() 
{

}

void ap_io::assignObj(iio* p_io)
{
  pIo = p_io;
}

int ap_io::SetGpioOut(out_e out_idx, bool onoff)
{
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

void ap_io::Update_io(void)
{

  // out
  uint8_t gpio = gpioPinRead(_GPIO_IO_OUT_01) << 0 | gpioPinRead(_GPIO_IO_OUT_02) << 1 | gpioPinRead(_GPIO_IO_OUT_03) << 2 | gpioPinRead(_GPIO_IO_OUT_04) << 3 | gpioPinRead(_GPIO_IO_OUT_05) << 4 | gpioPinRead(_GPIO_IO_OUT_06) << 5 | gpioPinRead(_GPIO_IO_OUT_07) << 6 | gpioPinRead(_GPIO_IO_OUT_08) << 7;
  out[AP_IO_DEF_BANK_NO_0] = gpio;

  gpio = 0x0;
  gpio = gpioPinRead(_GPIO_OP_LAMP_START) << 0 | gpioPinRead(_GPIO_OP_LAMP_STOP) << 1 | gpioPinRead(_GPIO_OP_LAMP_RESET) << 2
      //| gpioPinRead() << 3
      //| gpioPinRead() << 4
      //| gpioPinRead() << 5
      // is_motorOn | gpioPinRead() << 6
      // is_autoReady  | gpioPinRead() << 7
      ;

  out[AP_IO_DEF_BANK_NO_1] = gpio;

  gpio = 0x0;
  uint8_t fm_out_1 = pIo->GetBank_Out(AP_IO_DEF_BANK_NO_1);
  uint8_t fm_out_2 = pIo->GetBank_Out(AP_IO_DEF_BANK_NO_2);
  gpio = ((fm_out_1 & bit7) >> 7)   //<<0       //user_out_0
         | ((fm_out_2 & bit0) << 1) //>>0)<<1   //user_out_1
         | ((fm_out_2 & bit1) << 1) //>>1)<<2   //user_out_2
         | ((fm_out_2 & bit2) << 1) //>>2)<<3   //user_out_3
         | ((fm_out_2 & bit3) << 1) //>>3)<<4   //user_out_4
         | ((fm_out_2 & bit4) << 1) //>>4)<<5   //user_out_5
         | ((fm_out_2 & bit5) << 1) //>>5)<<6   //user_out_6
         | ((fm_out_2 & bit6) << 1) //>>6)<<7   //user_out_7
      ;
  out[AP_IO_DEF_BANK_NO_2] = gpio;

  gpio = 0x0;
  gpio = (fm_out_2 & bit7) >> 7; // user_out_0
  out[AP_IO_DEF_BANK_NO_3] = gpio;

  // in
  gpio = 0x0;
  gpio = gpioPinRead(_GPIO_IO_IN_01) << 0 | gpioPinRead(_GPIO_IO_IN_02) << 1 | gpioPinRead(_GPIO_IO_IN_03) << 2 | gpioPinRead(_GPIO_IO_IN_04) << 3 | gpioPinRead(_GPIO_IO_IN_05) << 4 | gpioPinRead(_GPIO_IO_IN_06) << 5 | gpioPinRead(_GPIO_IO_IN_07) << 6 | gpioPinRead(_GPIO_IO_IN_08) << 7;
  in[AP_IO_DEF_BANK_NO_0] = gpio;

  gpio = 0x0;
  gpio = gpioPinRead(_GPIO_OP_SW_START) << 0 | gpioPinRead(_GPIO_OP_SW_STOP) << 1 | gpioPinRead(_GPIO_OP_SW_RESET) << 2 | gpioPinRead(_GPIO_OP_SW_ESTOP) << 3
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

/* IO  */
void ap_io::SetBank_Out(uint8_t bank, uint8_t val)
{
  uint8_t base = 0;
  base = out[bank].get();
  for (uint8_t i = 0; i < 8; i++)
  {
    if ((((val >> i) ^ (base >> i)) & bit(0)))
    {
      uint8_t check = val & bit(i);
      bool is_on = check > 0 ? true : false;
      SetGpioOut(static_cast<out_e>(8 * bank + i), is_on);
    }
  }
  out[bank] = val;
}

uint8_t ap_io::GetBank_Out(uint8_t bank)
{
  return out[bank].get();
}

uint8_t ap_io::GetBank_In(uint8_t bank)
{
  return in[bank].get();
}

/* IO  control function */
bool ap_io::IsOn(uint32_t addr)
{
  if (!(addr < static_cast<uint32_t>(ap_io::in_e::_max))) // not
    return false;
  div_t div_result = div(addr, 8);
  return in[(uint8_t)div_result.quot][REG_BIT(div_result.rem)];
}

bool ap_io::IsOff(uint32_t addr)
{
  if (!(addr < static_cast<uint32_t>(ap_io::in_e::_max))) // not
    return false;
  div_t div_result = div(addr, 8);
  return !(in[(uint8_t)div_result.quot][REG_BIT(div_result.rem)]);
}

int ap_io::OutputOn(uint32_t addr)
{
  if (!(addr < static_cast<uint32_t>(ap_io::out_e::_max))) // not
    return -1;
  return SetGpioOut(static_cast<ap_io::out_e>(addr), true);
}

int ap_io::OutputOff(uint32_t addr)
{
  if (!(addr < static_cast<uint32_t>(ap_io::out_e::_max))) // not
    return -1;
  return SetGpioOut(static_cast<ap_io::out_e>(addr), false);
}

int ap_io::OutputToggle(uint32_t addr)
{
  if (!(addr < static_cast<uint32_t>(ap_io::out_e::_max))) // not
    return -1;
  div_t div_result = div(addr, 8); // div_result.quot ; // ¸ò //div_result.rem; // ³ª¸ÓÁö
  if (out[(uint8_t)div_result.quot][REG_BIT(div_result.rem)])
  {
    SetGpioOut(static_cast<ap_io::out_e>(addr), false);
  }
  else
  {
    SetGpioOut(static_cast<ap_io::out_e>(addr), true);
  }
  return 0;
}
