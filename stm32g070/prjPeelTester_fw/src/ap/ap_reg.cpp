#include "ap_def.h"
#include "ap_reg.h"

#ifdef _USE_HW_RTOS
#define AP_REG_LOCK_BEGIN osMutexWait(ap_reg_mutex_id, osWaitForever)
#define AP_REG_LOCK_END osMutexRelease(ap_reg_mutex_id)
#else
#define AP_REG_LOCK_BEGIN
#define AP_REG_LOCK_END
#endif

/*constructor*/
Ap_reg::Ap_reg() : bank_size(AP_REG_BANK_NO_MAX)
{
#ifdef _USE_HW_RTOS
  osMutexDef(ap_reg_mutex_id);
  ap_reg_mutex_id = osMutexCreate(osMutex(ap_reg_mutex_id));
#endif
}

bool Ap_reg::IsAutoReadyMode()
{
  return status[AP_REG_BANK_RUN_STATE][AP_REG_AUTO_READY];
}

bool Ap_reg::IsAutoRunning()
{
  return status[AP_REG_BANK_RUN_STATE][AP_REG_AUTO_RUNNING];
}

void Ap_reg::SetConfReg(uint8_t data)
{
  status[AP_REG_BANK_SETTING] = data;
}

uint8_t Ap_reg::GetAlarmState()
{
  return status[AP_REG_BANK_ERR_L].get();
}

void Ap_reg::ClearAlarmState()
{
  status[AP_REG_BANK_ERR_L] = 0;
}

void Ap_reg::SetRunState(bit_e bit, bool on_off)
{
  AP_REG_LOCK_BEGIN;
  uint8_t value = status[AP_REG_BANK_RUN_STATE].get();
  switch (bit)
  {
  case AP_REG_AUTO_RUNNING:
    if (on_off)
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
    if (on_off)
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

  case AP_REG_DETECT_AREA_SEN:
    if (on_off)
    {
      value = value | B00010000;
    }
    else
      value = value & B11101111;
    break;

  case AP_REG_MOTOR_ON:
    if (on_off)
      value = value | B00100000;
    else
      value = value & B11011111;
    break;

  case AP_REG_ORG_COMPLETED:
    if (on_off)
      value = value | B01000000;
    else
      value = value & B10111111;
    break;

  case AP_REG_ALARM_STATUS:
    if (on_off)
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
