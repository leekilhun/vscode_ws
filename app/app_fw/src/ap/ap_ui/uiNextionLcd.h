/*
 * uiNextionLcd.h
 *
 *  Created on: 2022. 3. 4.
 *      Author: gns2l
 */


#ifndef SRC_AP_AP_UI_UINEXTIONLCD_H_
#define SRC_AP_AP_UI_UINEXTIONLCD_H_

#define UI_NEXTION_MAX_BUFFER_LENGTH                     32+1

class uiNextionLcd
{

  /****************************************************
   *  data
   ****************************************************/
  enum class cmd_e {
    _onoff,
    _max
  };
public:
  struct cfg_t  {
    // Ap_reg* p_apReg;
    // IIO* p_apIo;
    // axis_dat* p_apAxisDat;
    // cyl_dat* p_apCylDat;
    // vac_dat* p_apVacDat;
    // ap_dat* p_apCfgDat;
    // seq_dat* p_apSeqDat;
    // enFastechMotor* p_Fm;
    // cnProcess* p_Ap;
    // cnAutoManager* p_Auto;
    // ap_log* p_Log;
    uint8_t ch;
    uint32_t baud;
  };

private:
  nextionpage_t m_currPage;
  // enFastechMotor* m_pFm;
  // cnProcess* m_pAp;
  // cnAutoManager* m_pAuto;
  // Ap_reg*       m_pApReg;
  // ap_log* m_pLog;
  // IIO* m_pApIo;
  // axis_dat* m_pApAxisDat;
  // cyl_dat* m_pApCylDat;
  // vac_dat* m_pApVacDat;
  // ap_dat* m_pApCfgDat;
  // seq_dat* m_pApSeqDat;
  uint8_t       m_msgCnt;
  bkCmdLevel m_bkCmdLevel;

public:
  bool m_IsConnected;
  nextion_t m_Packet;

  bool m_IsRespCplt;
  uint8_t m_retryCnt;
  uint8_t  m_stepBuffer[UI_NEXTION_MAX_BUFFER_LENGTH];
  uint32_t  m_pre_time;
  prc_step_t  m_step;
  /****************************************************
   *  Constructor
   ****************************************************/
public:
  explicit uiNextionLcd(const uiNextionLcd& ref){}
  uiNextionLcd& operator=(const uiNextionLcd ref) { return *this; }
  uiNextionLcd();
  virtual~uiNextionLcd();


  /****************************************************
   *  func
   ****************************************************/
private:
  static void updateLcdReg(void* obj);
  void doRunStep();
public:
  bool ThreadJob();
  int Init(uiNextionLcd::cfg_t* cfg);
  void LcdUpdate();
  void ProcessCmd();
  void CommRecovery();

  bool CheckCommLcdCfg();

  void SendMotorData();
  int SaveMotorData();

  void SendAxisData();
  void SendApReg();
  void SendApIoReg();
  void SendApCfgData();
  void SendCylinderData();
  void SendVacuumData();
  void SendSequenceData();
  void SendLogData();

  void SendDataAll();
  void ClearDataAll();
  int ChangePage(nextionpage_t page);

  inline nextionpage_t GetcurrPage() const{
    return m_currPage;
  }

  // inline Ap_reg* GetApReg() const{
  //   return m_pApReg;
  // }








};

#endif /* SRC_AP_AP_UI_UINEXTIONLCD_H_ */
