/*
 * ap.c
 *
 *  Created on: Dec 27, 2021
 *      Author: gns2.lee
 */

#include "ap.h"

static void threadCmdLcd(void const *argument);

static void threadCmdFastechMotor(void const *argument);
static void threadEvent(void const *argument);

#define AP_DEF_ERROR_CNT_MAX 10

/*ap에서 생성할 물리 객체들 */

// 2. 물리 객체들

// 3. 로직 객체

// 4.
uiNextionLcd nextion_lcd;

void apInit(void)
{
  /**/
  osThreadDef(threadCmdLcd, threadCmdLcd, _HW_DEF_RTOS_THREAD_PRI_LCD_CMD, 0, _HW_DEF_RTOS_THREAD_MEM_LCD_CMD);
  if (osThreadCreate(osThread(threadCmdLcd), NULL) != NULL)
  {
    logPrintf("threadCmdLcd \t\t: OK\r\n");
  }
  else
  {
    logPrintf("threadCmdLcd \t\t: Fail\r\n");
  }

  osThreadDef(threadCmdFastechMotor, threadCmdFastechMotor, _HW_DEF_RTOS_THREAD_PRI_FM_CMD, 0, _HW_DEF_RTOS_THREAD_MEM_FM_CMD);
  if (osThreadCreate(osThread(threadCmdFastechMotor), NULL) != NULL)
  {
    logPrintf("threadCmdFastechMotor \t\t: OK\r\n");
  }
  else
  {
    logPrintf("threadCmdFastechMotor \t\t: Fail\r\n");
  }

  /**/
  osThreadDef(threadEvent, threadEvent, _HW_DEF_RTOS_THREAD_PRI_EVENT, 0, _HW_DEF_RTOS_THREAD_MEM_EVENT);
  if (osThreadCreate(osThread(threadEvent), NULL) != NULL)
  {
    logPrintf("threadEvent \t\t: OK\r\n");
  }
  else
  {
    logPrintf("threadEvent \t\t: Fail\r\n");
  }
}

/*######################################################
   Main task
  ######################################################*/
void apMain(void)
{
/*user display object*/
  uiNextionLcd::cfg_t lcd_cfg ={0,};
  // lcd_cfg.p_apReg = &mcu_reg;
  // lcd_cfg.p_apIo = &mcu_io;
  // lcd_cfg.p_Fm = &fastech_motor;
  // lcd_cfg.p_apAxisDat =&axis_data;
  // lcd_cfg.p_apCylDat =&cyl_data;
  // lcd_cfg.p_apVacDat =&vac_data;
  // lcd_cfg.p_apCfgDat = &apCfg_data;
  // lcd_cfg.p_apSeqDat = &seq_data;
  // lcd_cfg.p_Ap=&process;
  // lcd_cfg.p_Auto = &autoManager;
  // lcd_cfg.p_Log = &mcu_log;
  lcd_cfg.ch = _DEF_UART3;
  lcd_cfg.baud = 115200;//250000;//;
  nextion_lcd.Init(&lcd_cfg);






uint32_t pre_main_ms = millis();
  while (1)
  {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
    if (millis() - pre_main_ms >= 1000)
    {
      ledToggle(_DEF_LED1);

      pre_main_ms = millis();
    }
  }
}

/*######################################################
  Event Task  -> cnProcess step
  ######################################################*/
void threadEvent(void const *argument)
{
  UNUSED(argument);
  uint32_t pre_cmd_event_ms = millis();
  while (1)
  {
    if (millis() - pre_cmd_event_ms >= 50)
    {
      pre_cmd_event_ms = millis();
    }
  }
}

/*######################################################
   Nextion Lcd Communication
  ######################################################*/
void threadCmdLcd(void const *argument)
{
  UNUSED(argument);

  uint8_t err_cnt = 0;
  uint32_t pre_cmd_lcd_ms = millis();
  while (1)
  {
    if (cmdNextion_ReceivePacket(&nextion_lcd.m_Packet))
    {
      nextion_lcd.m_IsConnected = true;
      nextion_lcd.ProcessCmd();
    }

    nextion_lcd.ThreadJob();

    if (millis() - pre_cmd_lcd_ms >= 50)
    {
      pre_cmd_lcd_ms = millis();
      if (nextion_lcd.m_IsConnected)
      {
        nextion_lcd.m_IsConnected = false;
        nextion_lcd.LcdUpdate();
        err_cnt = 0;
      }
      else if (err_cnt > 30)
      {
        err_cnt = 0;
        nextion_lcd.CommRecovery();
        nextion_lcd.m_IsConnected = true;
        //mcu_reg.status[AP_REG_BANK_ERR_H][AP_REG_ERR_NO_RESP_LCD] = true;
      }
      else
      {
        nextion_lcd.LcdUpdate();
        err_cnt++;
      }
    }
  }
}

/*######################################################
   FastechMotor Communication Task
  ######################################################*/
void threadCmdFastechMotor(void const *argument)
{
  UNUSED(argument);
  uint32_t pre_cmd_fm_ms = millis();
  while (1)
  {
    if (millis() - pre_cmd_fm_ms >= 50)
    {
      pre_cmd_fm_ms = millis();    
    }
  }
}
