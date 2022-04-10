/*
 * apiNextion.h
 *
 *  Created on: 2022. 4. 7.
 *      Author: gns2l
 * 
 *  ��ġ lcd�� �����ϱ� ���� Ŭ����
 */

#pragma once



#define UI_NEXTION_MAX_BUFFER_LENGTH                     (32+1)

class uiNextionLcd
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
    cnProcess *p_Ap;
    cnAutoManager *p_Auto;
    ap_log *p_Log;
    uint8_t ch;
    uint32_t baud;
  };

private:
  nextionpage_t m_currPage;
  enFaxis *m_pFm;
  cnProcess *m_pAp;
  cnAutoManager *m_pAuto;
  Ap_reg *m_pApReg;
  ap_log *m_pLog;
  iio *m_pApIo;
  axis_dat *m_pApAxisDat;
  cyl_dat *m_pApCylDat;
  vac_dat *m_pApVacDat;
  ap_dat *m_pApCfgDat;
  seq_dat *m_pApSeqDat;
  uint8_t m_msgCnt;
  bkCmdLevel m_bkCmdLevel;

public:
  bool m_IsConnected;
  nextion_t m_Packet;

  bool m_IsRespCplt;
  uint8_t m_retryCnt;
  uint8_t m_stepBuffer[UI_NEXTION_MAX_BUFFER_LENGTH];
  uint32_t m_pre_time;
  prc_step_t m_step;
  bool m_lockSendMsg;
  /****************************************************
   *  Constructor
   ****************************************************/
public:
  uiNextionLcd();
  virtual ~uiNextionLcd();

  /****************************************************
   *  func
   ****************************************************/
private:
  void doRunStep();
  void okResponse();
  static void cbUpdateReg(void *obj);

public:
  int Init(uiNextionLcd::cfg_t *cfg);
  // step machine�� ���� nonblock���� ó���ȴ�. 
  bool ThreadJob();
  // lcd���� Command ���� ó��, ���� ack ��ȣ�� �����Ѵ�
  void ProcessCmd();
  // lcdȭ�� ���¿� ���� ������ ������
  void LcdUpdate();
  // ��� ������ �� �õ��Ѵ�
  void CommRecovery();
  
  bool CheckCommLcdCfg();
  void SendMotorData();
  int SaveMotorData();
  void SendAxisIoData();
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
  nextionpage_t GetcurrPage() const;

};
