#include "ap.h"

static void threadCmdLcd(void const *argument);
static void threadCmdFastechMotor(void const *argument);
static void threadEvent(void const *argument);

#define AP_DEF_ERROR_CNT_MAX 10
uint16_t refresh_time = 1000;
uint8_t errCnt = 0;
uint8_t lcd_errCnt = 0;
uint8_t fm_errCnt = 0;
uint32_t lamp_ms = 0;

void updateApReg();
void updateErr();
void eventOpPanel();
void updateLamp();


/****************************************************
  0. mcu data and register 
 ****************************************************/
Ap_reg mcu_reg;
ap_io mcu_io;
ap_dat apCfg_data;
axis_dat axis_data;
cyl_dat cyl_data;
vac_dat vac_data;
seq_dat seq_data;
ap_log mcu_log;
/****************************************************
  1. ap에서 생성할 객체 
 ****************************************************/
// engine layer (물리 계층)
enFaxis fastech_motor(AP_DEF_OBJ_MOTOR_ID_0);
enCyl cyl[AP_DEF_OBJ_CYLINDER_ID_MAX];
enVac vac[AP_DEF_OBJ_VACUUM_ID_MAX];
enOp op_sw;

// control  (로직 계층)
cnAuto autoManager;
cnJob process;

// application interface.
uiNextionLcd nextion_lcd;
uiRemoteCtrl remote_ctrl;

//시간 측정용 임시
uint32_t check_pass_ms = 0;


void apInit(void)
{
 /* rtos initial*/
  {
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


  /* operating panel sw initial */
  {
    enOp::cfg_t cfg ={0,};
    cfg.sw_pin_start = _GPIO_OP_SW_START;
    cfg.sw_pin_stop =_GPIO_OP_SW_STOP;
    cfg.sw_pin_reset=_GPIO_OP_SW_RESET;
    cfg.sw_pin_estop=_GPIO_OP_SW_ESTOP;
    op_sw.Init(&cfg);
  }
  /* motor initial */
  {
    enFaxis::cfg_t fm_cfg = {0, };
    fm_cfg.p_apReg = &mcu_reg;
    fm_cfg.p_apCfgDat = &apCfg_data;
    fm_cfg.p_apIo = &mcu_io;
    fm_cfg.p_apDat = &axis_data;
    fm_cfg.ch = _DEF_UART1;
    fm_cfg.baud = 115200;
    fastech_motor.Init(&fm_cfg);
  }
  /* cylinder initial */
  {
    /*phone jig*/
    enCyl::cfg_t cyl_cfg = {0, };
    cyl_cfg.cyl_id = AP_DEF_OBJ_CYLINDER_ID_PHONE_JIG;
    cyl_cfg.cyl_type = icylinder::type_e::open_close;
    cyl_cfg.sol_type = icylinder::sol_e::two;
    cyl_cfg.pApIo = (iio *)&mcu_io;
    cyl_cfg.pCylDat = cyl_data.GetData(cyl_dat::addr_e::phone_clamp_cyl);
    cyl_cfg.sensor_io[EN_CYLINDER_SENSOR_UP] = static_cast<uint32_t>(ap_io::out_e::mcu_out_cyl_1_on);
    cyl_cfg.sensor_io[EN_CYLINDER_SENSOR_DOWN] = static_cast<uint32_t>(ap_io::out_e::mcu_out_cyl_1_off);
    cyl_cfg.sol_io[EN_CYLINDER_SOL_UP] = static_cast<uint32_t>(ap_io::in_e::mcu_in_cyl_1_on);
    cyl_cfg.sol_io[EN_CYLINDER_SOL_DOWN] = static_cast<uint32_t>(ap_io::in_e::mcu_in_cyl_1_off);

    cyl[AP_DEF_OBJ_CYLINDER_ID_PHONE_JIG].SetConfigData(&cyl_cfg);

    /*drum updown jig*/
    cyl_cfg.cyl_id = AP_DEF_OBJ_CYLINDER_ID_DRUM_UPDOWN;
    cyl_cfg.cyl_type = icylinder::type_e::up_down;
    cyl_cfg.sol_type = icylinder::sol_e::two;
    cyl_cfg.pApIo = (iio *)&mcu_io;
    cyl_cfg.pCylDat = cyl_data.GetData(cyl_dat::addr_e::drum_updown_cyl);
    cyl_cfg.sensor_io[EN_CYLINDER_SENSOR_UP] = static_cast<uint32_t>(ap_io::out_e::mcu_out_cyl_2_on);
    cyl_cfg.sensor_io[EN_CYLINDER_SENSOR_DOWN] = static_cast<uint32_t>(ap_io::out_e::mcu_out_cyl_2_off);
    cyl_cfg.sol_io[EN_CYLINDER_SOL_UP] = static_cast<uint32_t>(ap_io::in_e::mcu_in_cyl_2_on);
    cyl_cfg.sol_io[EN_CYLINDER_SOL_DOWN] = static_cast<uint32_t>(ap_io::in_e::mcu_in_cyl_2_off);

    cyl[AP_DEF_OBJ_CYLINDER_ID_DRUM_UPDOWN].SetConfigData(&cyl_cfg);

    /*drum z-down jig*/
    cyl_cfg.cyl_id = AP_DEF_OBJ_CYLINDER_ID_DRUM_Z_UP;
    cyl_cfg.cyl_type = icylinder::type_e::up_down;
    cyl_cfg.sol_type = icylinder::sol_e::two;
    cyl_cfg.pApIo = (iio *)&mcu_io;
    cyl_cfg.pCylDat = cyl_data.GetData(cyl_dat::addr_e::drum_z_up_cyl);
    cyl_cfg.sensor_io[EN_CYLINDER_SENSOR_UP] = static_cast<uint32_t>(ap_io::out_e::fm_out_cyl_3_on);
    cyl_cfg.sensor_io[EN_CYLINDER_SENSOR_DOWN] = static_cast<uint32_t>(ap_io::out_e::fm_out_cyl_3_off);
    cyl_cfg.sol_io[EN_CYLINDER_SOL_UP] = static_cast<uint32_t>(ap_io::in_e::fm_in_cyl_3_on);
    cyl_cfg.sol_io[EN_CYLINDER_SOL_DOWN] = static_cast<uint32_t>(ap_io::in_e::fm_in_cyl_3_off);

    cyl[AP_DEF_OBJ_CYLINDER_ID_DRUM_Z_UP].SetConfigData(&cyl_cfg);

    /*drum stop jig*/
    cyl_cfg.cyl_id = AP_DEF_OBJ_CYLINDER_ID_DRUM_STOP;
    cyl_cfg.cyl_type = icylinder::type_e::lock_unlock;
    cyl_cfg.sol_type = icylinder::sol_e::two;
    cyl_cfg.pApIo = (iio *)&mcu_io;
    cyl_cfg.pCylDat = cyl_data.GetData(cyl_dat::addr_e::drum_stop_cyl);
    cyl_cfg.sensor_io[EN_CYLINDER_SENSOR_UP] = static_cast<uint32_t>(ap_io::out_e::fm_out_cyl_4_on);
    cyl_cfg.sensor_io[EN_CYLINDER_SENSOR_DOWN] = static_cast<uint32_t>(ap_io::out_e::fm_out_cyl_4_off);
    cyl_cfg.sol_io[EN_CYLINDER_SOL_UP] = static_cast<uint32_t>(ap_io::in_e::fm_in_cyl_4_on);
    cyl_cfg.sol_io[EN_CYLINDER_SOL_DOWN] = static_cast<uint32_t>(ap_io::in_e::fm_in_cyl_4_off);

    cyl[AP_DEF_OBJ_CYLINDER_ID_DRUM_STOP].SetConfigData(&cyl_cfg);
  }

  /* vacuum initial */
  {
    /*drum head vacuum*/
    enVac::cfg_t vac_cfg = { 0, };
    vac_cfg.vac_id = AP_DEF_OBJ_VACUUM_ID_DRUM_HEAD;
    vac_cfg.vac_type = ivacuum::type_e::suction_blow;
    vac_cfg.pApIo = (iio *)&mcu_io;
    vac_cfg.pVacDat = vac_data.GetData(vac_dat::addr_e::peel_drum_vac);
    vac_cfg.sensor_io = static_cast<uint32_t>(ap_io::in_e::mcu_in_drum_vac_on);
    vac_cfg.sol_io[EN_VACUUM_SOL_SUCTION] = static_cast<uint32_t>(ap_io::out_e::mcu_out_vac_1_on);
    vac_cfg.sol_io[EN_VACUUM_SOL_BLOW] = static_cast<uint32_t>(ap_io::out_e::mcu_out_vac_1_off);

    vac[AP_DEF_OBJ_VACUUM_ID_DRUM_HEAD].SetConfigData(&vac_cfg);

    /*drum tail vacuum*/
    vac_cfg.vac_id = AP_DEF_OBJ_VACUUM_ID_DRUM_TAIL;
    vac_cfg.vac_type = ivacuum::type_e::suction_blow;
    vac_cfg.pApIo = (iio *)&mcu_io;
    vac_cfg.pVacDat = vac_data.GetData(vac_dat::addr_e::peel_drum_tail_vac);
    vac_cfg.sensor_io = static_cast<uint32_t>(ap_io::in_e::mcu_in_drum_tail_vac_on);
    vac_cfg.sol_io[EN_VACUUM_SOL_SUCTION] = static_cast<uint32_t>(ap_io::out_e::mcu_out_vac_2_on);
    vac_cfg.sol_io[EN_VACUUM_SOL_BLOW] = static_cast<uint32_t>(ap_io::out_e::mcu_out_vac_2_off);

    vac[AP_DEF_OBJ_VACUUM_ID_DRUM_TAIL].SetConfigData(&vac_cfg);

    /*phone jig vacuum*/
    vac_cfg.vac_id = AP_DEF_OBJ_VACUUM_ID_PHONE_JIG;
    vac_cfg.vac_type = ivacuum::type_e::suction_blow;
    vac_cfg.pApIo = (iio *)&mcu_io;
    vac_cfg.pVacDat = vac_data.GetData(vac_dat::addr_e::peel_phone_jig);
    vac_cfg.sensor_io = static_cast<uint32_t>(ap_io::in_e::mcu_in_phonejig_vac_on);
    vac_cfg.sol_io[EN_VACUUM_SOL_SUCTION] = static_cast<uint32_t>(ap_io::out_e::fm_out_vac_3_on);
    vac_cfg.sol_io[EN_VACUUM_SOL_BLOW] = static_cast<uint32_t>(ap_io::out_e::fm_out_vac_3_off);

    vac[AP_DEF_OBJ_VACUUM_ID_PHONE_JIG].SetConfigData(&vac_cfg);
  }

  /* automanager initial */
  {
    cnAuto::cfg_t auto_cfg = {0, };
    auto_cfg.p_apReg = &mcu_reg;
    auto_cfg.p_apLog = &mcu_log;
    auto_cfg.p_ApIo = (iio *)&mcu_io;;
    autoManager.Init(auto_cfg);
  }

  /* sequence process initial */
  {
    cnJob::cfg_t prc_cfg = {0, };
    prc_cfg.p_apReg = &mcu_reg;
    prc_cfg.p_apIo = &mcu_io;
    prc_cfg.p_Fm = &fastech_motor;
    prc_cfg.p_Cyl = cyl;
    prc_cfg.p_Vac = vac;
    prc_cfg.p_AutoManger = &autoManager;
    prc_cfg.p_apAxisDat = &axis_data;
    prc_cfg.p_apCylDat = &cyl_data;
    prc_cfg.p_apVacDat = &vac_data;
    prc_cfg.p_apCfgDat = &apCfg_data;
    prc_cfg.p_apSeqDat = &seq_data;
    process.Init(&prc_cfg);
  }
}



void apMain(void)
{
/*user display object*/
  uiNextionLcd::cfg_t lcd_cfg = {0, };
  lcd_cfg.p_apReg = &mcu_reg;
  lcd_cfg.p_apIo = &mcu_io;
  lcd_cfg.p_Fm = &fastech_motor;
  lcd_cfg.p_apAxisDat = &axis_data;
  lcd_cfg.p_apCylDat = &cyl_data;
  lcd_cfg.p_apVacDat = &vac_data;
  lcd_cfg.p_apCfgDat = &apCfg_data;
  lcd_cfg.p_apSeqDat = &seq_data;
  lcd_cfg.p_Ap=&process;
  lcd_cfg.p_Auto = &autoManager;
  lcd_cfg.p_Log = &mcu_log;
  lcd_cfg.ch = _DEF_UART3;
  lcd_cfg.baud = 115200; // 250000;//;
  nextion_lcd.Init(&lcd_cfg);

  /* remote control and monitor */
  uiRemoteCtrl::cfg_t remote_cfg = {0, };
  remote_cfg.p_apReg = &mcu_reg;
  remote_cfg.p_apIo = &mcu_io;
  remote_cfg.p_apAxisDat = &axis_data;
  remote_cfg.p_apCylDat = &cyl_data;
  remote_cfg.p_apVacDat = &vac_data;
  remote_cfg.p_apCfgDat = &apCfg_data;
  remote_cfg.p_apSeqDat = &seq_data;
  remote_cfg.p_Ap=&process;
  remote_cfg.p_Auto = &autoManager;
  remote_cfg.p_Fm = &fastech_motor;
  remote_cfg.p_Log = &mcu_log;
  remote_cfg.ch = _DEF_UART2;
  remote_cfg.baud = 115200;

  remote_ctrl.Init(&remote_cfg);

  /****************************/
  /* loading eeprom data*/
  apCfg_data.LoadRomData();
  delay(5);
  axis_data.LoadRomData();
  delay(5);
  cyl_data.LoadRomData();
  delay(5);
  vac_data.LoadRomData();
  delay(5);
  seq_data.LoadRomData();

  /* set default system config */
  mcu_reg.status[AP_REG_BANK_SETTING][AP_REG_USE_BEEP] = true;

  /****************************/
  process.SetAutoSpeed(seq_data.GetMaxSpeed());
  /****************************/


  /*Assign Obj */
  mcu_io.assignObj((iio *)&fastech_motor);

  uint32_t pre_main_ms = millis();
  while (1)
  {
    if (millis() - pre_main_ms >= refresh_time)
    {
      ledToggle(_DEF_LED1);
      pre_main_ms = millis();
    }
    updateErr();
    updateApReg();
    updateLamp();

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
    /*1. op panel check event -> update autoManager flag and mcu register */
    eventOpPanel();

    /*2. check auto ready state */
    autoManager.ThreadJob();

    /*3. job process  */
    process.ThreadJob();

    /*4. remote control */
    remote_ctrl.ThreadJob();

    /* send all state */
    /* remote control step 진행 중에는 SendAllState를 Lock하여
       패킷 중복을 방지하며 제어쪽은 ack신호를 기다리며
       응답이 없으면 retry 3회 진행된다
     */
    if (millis() - pre_cmd_event_ms >= 50)
    {
      pre_cmd_event_ms = millis();
      remote_ctrl.PcUpdate();
      check_pass_ms = millis() - pre_cmd_event_ms;
    }
  }
}

/*######################################################
   Nextion Lcd Communication
  ######################################################*/
void threadCmdLcd(void const *argument)
{
  UNUSED(argument);

  uint32_t pre_cmd_lcd_ms = millis();
  while (1)
  {
    /*lcd에서 수신된 패킷이 있으면 ack를 리턴하고 lcdUpdate는 처리 완료까지 lock한다*/
    if (cmdNextion_ReceivePacket(&nextion_lcd.m_Packet))
    {
      lcd_errCnt = 0;
      nextion_lcd.m_IsConnected = true;
      mcu_reg.status[AP_REG_BANK_ERR_H][AP_REG_ERR_NO_RESP_LCD] = false;
      nextion_lcd.ProcessCmd();
    }

    nextion_lcd.ThreadJob();

    /*
    lcd update 정보를 보낸 후 errCnt증가한다 (수신 act 시 rest) 
    연결 상태 문제 경우 recovery를 시도한다. 
    */
    if (millis() - pre_cmd_lcd_ms >= 50)
    {
      pre_cmd_lcd_ms = millis();
      if (nextion_lcd.m_IsConnected)
      {
        nextion_lcd.m_IsConnected = false;
        nextion_lcd.LcdUpdate();
        lcd_errCnt = 0;
      }
      else if (lcd_errCnt > 30)
      {
        lcd_errCnt = 0;
        nextion_lcd.CommRecovery();
        nextion_lcd.m_IsConnected = true;
        mcu_reg.status[AP_REG_BANK_ERR_H][AP_REG_ERR_NO_RESP_LCD] = true;
      }
      else
      {
        nextion_lcd.LcdUpdate();
        lcd_errCnt++;
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
    /*
    fastech motor의 상태정보를 update하기 위해 GetAllStatus패킷을 보낸 후 
    errCnt를 리셋하며  수신이 완료되면 다시 리셋하여 통신 상태를 체크한다
    */
    if (cmdFastech_ReceivePacket(&fastech_motor.m_Packet))
    {
      fm_errCnt = 0;
      fastech_motor.m_IsConnected = true;
      mcu_reg.status[AP_REG_BANK_ERR_H][AP_REG_ERR_NO_RESP_MOT] = false;

      fastech_motor.ProcessCmd();
    }

    /*
    fastech motor의 상태정보를 update하기 위해 GetAllStatus패킷을 보낸 후
    errCnt를 증가 시킨다 수신이 완료되면 다시 리셋하여 통신 상태를 체크한다
    */
    /* request all state */
    if (millis() - pre_cmd_fm_ms >= 100)
    {
      pre_cmd_fm_ms = millis();
      if (fastech_motor.m_IsConnected)
      {
        fastech_motor.m_IsConnected = false;
        fastech_motor.SendGetAllStatus();
        fm_errCnt = 0;
      }
      else if (fm_errCnt > 30)
      {
        fm_errCnt = 0;
        fastech_motor.CommRecovery();
        fastech_motor.m_IsConnected = true;
        mcu_reg.status[AP_REG_BANK_ERR_H][AP_REG_ERR_NO_RESP_MOT] = true;
      }
      else
      {
        fastech_motor.SendGetAllStatus();
        fm_errCnt++;
      }
    }
  }
}

/*######################################################
   main thread api
  ######################################################*/
void eventOpPanel()
{
   if (op_sw.GetPressed(enOp::panel::SW_ESTOP))
  {
    mcu_reg.SetRunState(AP_REG_EMG_STOP, true);
    return;
  }
  else
  {
    mcu_reg.SetRunState(AP_REG_EMG_STOP, false);
  }

  if (op_sw.GetPressed(enOp::panel::SW_START))
  {
    autoManager.StartSw();
  }
  else if (op_sw.GetPressed(enOp::panel::SW_STOP))
  {
    mcu_reg.SetRunState(AP_REG_AUTO_RUNNING, false);
    autoManager.StopSw();
  }
  else if (op_sw.GetPressed(enOp::panel::SW_RESET))
  {
    mcu_reg.SetRunState(AP_REG_ALARM_STATUS, false);
    autoManager.ResetSw();
  }
}

void updateLamp()
{
  enOp::status op_state = autoManager.GetOPStatus();

  switch (op_state)
  {
  case enOp::status::INIT:
    if (millis() - lamp_ms >= 1000)
    {
      lamp_ms = millis();
      mcu_io.OutputToggle(AP_IO_DEF_IO_ADDR_MCU_OUT_LAMP_START);
    }
    break;

  case enOp::status::ERR_STOP:
    if (millis() - lamp_ms >= 300)
    {
      lamp_ms = millis();
      mcu_io.OutputToggle(AP_IO_DEF_IO_ADDR_MCU_OUT_LAMP_STOP);
    }
    mcu_io.OutputOff(AP_IO_DEF_IO_ADDR_MCU_OUT_LAMP_START);
    mcu_io.OutputOff(AP_IO_DEF_IO_ADDR_MCU_OUT_LAMP_RESET);
    mcu_reg.SetRunState(AP_REG_ERROR_STOP, true);
    break;

  case enOp::status::STEP_STOP:
    mcu_io.OutputOn(AP_IO_DEF_IO_ADDR_MCU_OUT_LAMP_STOP);
    mcu_io.OutputOff(AP_IO_DEF_IO_ADDR_MCU_OUT_LAMP_START);
    mcu_io.OutputOff(AP_IO_DEF_IO_ADDR_MCU_OUT_LAMP_RESET);
    mcu_reg.SetRunState(AP_REG_ERROR_STOP, true);
    break;

  case enOp::status::RUN_READY:
    if (millis() - lamp_ms >= 1000)
    {
      lamp_ms = millis();
      mcu_io.OutputToggle(AP_IO_DEF_IO_ADDR_MCU_OUT_LAMP_START);
    }

    mcu_io.OutputOff(AP_IO_DEF_IO_ADDR_MCU_OUT_LAMP_STOP);
    mcu_io.OutputOff(AP_IO_DEF_IO_ADDR_MCU_OUT_LAMP_RESET);
    mcu_reg.SetRunState(AP_REG_AUTO_READY, true);
    break;

  case enOp::status::RUN:
    mcu_io.OutputOn(AP_IO_DEF_IO_ADDR_MCU_OUT_LAMP_START);
    mcu_reg.SetRunState(AP_REG_AUTO_RUNNING, true);
    break;
  default:
    break;
  }
}

void updateErr()
{
  // Check the error status of the constructed unit
  // auto run 상태의 process-step 운영에서 발생되는 에러 정지는 포함하지 않는다
  if (mcu_reg.status[AP_REG_BANK_ERR_H][AP_REG_ERR_NO_RESP_MOT])
  {
    if (errCnt >= AP_DEF_ERROR_CNT_MAX)
    {
      errCnt = 0;
      mcu_reg.status[AP_REG_BANK_ERR_H][AP_REG_ERR_CLEAR] = false;
      refresh_time = 100;
    }
    else
    {
      errCnt++;
    }
  }
  else if (mcu_reg.status[AP_REG_BANK_ERR_H][AP_REG_ERR_NO_RESP_LCD])
  {
    if (errCnt >= AP_DEF_ERROR_CNT_MAX)
    {
      errCnt = 0;
      mcu_reg.status[AP_REG_BANK_ERR_H][AP_REG_ERR_CLEAR] = false;
      refresh_time = 300;
    }
    else
    {
      errCnt++;
    }
  }
  else
  {
    errCnt = 0;
    mcu_reg.status[AP_REG_BANK_ERR_H][AP_REG_ERR_CLEAR] = true;
    refresh_time = 1000;
    // process(sequence) alarm 상태를 체크
    if (mcu_reg.GetAlarmState() != 0)
    {
      mcu_reg.SetRunState(AP_REG_ALARM_STATUS, true);
    }
    else
    {
      mcu_reg.SetRunState(AP_REG_ALARM_STATUS, false);
    }
  }
}

void updateApReg()
{
  mcu_io.Update_io();
    if (autoManager.IsDetectAreaSensor()) //
  {
    mcu_reg.SetRunState(AP_REG_DETECT_AREA_SEN, true);
  }
  else
  {
    mcu_reg.SetRunState(AP_REG_DETECT_AREA_SEN, false);
  }
}