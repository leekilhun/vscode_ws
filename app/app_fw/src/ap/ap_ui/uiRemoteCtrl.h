/*
 * uiRemoteCtrl.h
 *
 *  Created on: 2022. 3. 6.
 *      Author: gns2l
 */

#ifndef SRC_AP_AP_UI_UIREMOTECTRL_H_
#define SRC_AP_AP_UI_UIREMOTECTRL_H_
#define UI_REMOTECTRL_MAX_BUFFER_LENGTH                     32
class uiRemoteCtrl
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
    Ap_reg* p_apReg;
    IIO* p_apIo;
    axis_dat* p_apAxisDat;
    cyl_dat* p_apCylDat;
    vac_dat* p_apVacDat;
    ap_dat* p_apCfgDat;
    seq_dat* p_apSeqDat;
    enFastechMotor* p_Fm;
    cnAutoManager* p_Auto;
    cnProcess* p_Ap;
    ap_log *p_Log;
    uint8_t ch;
    uint32_t baud;
  };

private:
  //cfg_t m_Cfg;
  Ap_reg*       m_pApReg;
  IIO* m_pApIo;
  axis_dat* m_pApAxisDat;
  cyl_dat* m_pApCylDat;
  vac_dat* m_pApVacDat;
  ap_dat* m_pApCfgDat;
  seq_dat* m_pApSeqDat;
  enFastechMotor* m_pFm;
  cnAutoManager* m_pAuto;
  cnProcess* m_pAp;
  ap_log* m_pLog;
  robotro_t m_Packet;

  bool m_IsRespCplt;
  uint8_t m_retryCnt;
  uint8_t  m_stepBuffer[UI_REMOTECTRL_MAX_BUFFER_LENGTH];
  uint32_t  m_pre_time;
  prc_step_t  m_step;
  /****************************************************
   *  Constructor
   ****************************************************/
public:
  uiRemoteCtrl ();
  virtual ~uiRemoteCtrl ();

  /****************************************************
   *  func
   ****************************************************/
private:
  void doRunStep();
public:
  bool ThreadJob();
  int Init(uiRemoteCtrl::cfg_t* cfg);
  void ProcessCmd();
  void SendAllState();
  void CommRecovery();

};

#endif /* SRC_AP_AP_UI_UIREMOTECTRL_H_ */
