/*
 * apiRemote.h
 *
 *  Created on: 2022. 4. 7.
 *      Author: gns2l
 * 
 *  pc프로그램으로 mcu를 제어하기 위한 클래스
 */

#pragma once


#define UI_REMOTECTRL_MAX_BUFFER_LENGTH                     (40+1)
class uiRemoteCtrl
{
  /****************************************************
   *  data
   ****************************************************/
  enum class cmd_e
  {
    _onoff,
    _max
  };

public:
  struct cfg_t
  {
    Ap_reg *p_apReg;
    iio *p_apIo;
    axis_dat *p_apAxisDat;
    cyl_dat *p_apCylDat;
    vac_dat *p_apVacDat;
    ap_dat *p_apCfgDat;
    seq_dat *p_apSeqDat;
    enFaxis *p_Fm;
    cnAuto *p_Auto;
    cnDo *p_Ap;
    ap_log *p_Log;
    uint8_t ch;
    uint32_t baud;
  };

private:
  // cfg_t m_Cfg;
  Ap_reg *m_pApReg;
  iio *m_pApIo;
  axis_dat *m_pApAxisDat;
  cyl_dat *m_pApCylDat;
  vac_dat *m_pApVacDat;
  ap_dat *m_pApCfgDat;
  seq_dat *m_pApSeqDat;
  enFaxis *m_pFm;
  cnAuto *m_pAuto;
  cnDo *m_pAp;
  ap_log *m_pLog;

  bool m_IsRespCplt;
  uint8_t m_retryCnt;
  uint8_t m_stepBuffer[UI_REMOTECTRL_MAX_BUFFER_LENGTH];
  uint32_t m_pre_time;
  prc_step_t m_step;
  bool m_lockSendMsg;

public:
  robotro_t m_Packet;

  /****************************************************
   *  Constructor
   ****************************************************/
public:
  uiRemoteCtrl();
  virtual ~uiRemoteCtrl();

  /****************************************************
   *  func
   ****************************************************/
private:
  void doRunStep();
  void okResponse();
  void retGetValue(uint8_t *p_data, uint8_t length);

public:
  int Init(uiRemoteCtrl::cfg_t *cfg);
  // step machine을 통해 nonblock으로 처리된다.
  bool ThreadJob();  
  // PC에서 Command 수신 처리, 수신 ack 신호를 리턴한다
  void ProcessCmd();  
  // mcu 상태 정보를 보낸다
  void PcUpdate();
  // 통신 연결을 재 시도한다
  void CommRecovery();
};
