/*
 * cnDo.h
 *
 *  Created on: 2022. 4. 20.
 *      Author: gns2l
 */

#pragma once

#define CN_DO_MAX_BUFFER_LENGTH                40

class cnDo
{
  /****************************************************
   *  data
   ****************************************************/
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
    enOp* p_Op;
    enCyl *p_Cyl;
    enVac *p_Vac;
    cnAuto *p_AutoManger;
  };

private:
  cnAuto *m_pAuto;
  enFaxis *m_pFm;
  enCyl *m_pCyl;
  enVac *m_pVac;
  enOp *m_pOp;
  Ap_reg *m_pApReg;
  iio *m_pApIo;
  axis_dat *m_pApAxisDat;
  cyl_dat *m_pApCylDat;
  vac_dat *m_pApVacDat;
  ap_dat *m_pApCfgDat;
  seq_dat *m_pApSeqDat;

  uint8_t m_retryCnt;

public:

  uint8_t m_stepBuffer[CN_DO_MAX_BUFFER_LENGTH];
  uint32_t m_pre_time;
  prc_step_t m_step;

  /****************************************************
   *  Constructor
   ****************************************************/
public:
  cnDo();
  virtual ~cnDo();

  /****************************************************
   *  func
   ****************************************************/
private:
  // for auto run
  void doRunStep();

public:
  int Init(cnDo::cfg_t *cfg);

  // non-block event patrol
  void ThreadJob();

  void Initialize();
  void MotorOrigin();
  void MotorOnOff(bool on_off);
  bool IsMotorOn();
  bool IsMotorRun();
  bool IsMotorMoveCplt();

  void MotorStop();
  void MotorRun(int cmd_pos, uint32_t cmd_vel, uint32_t acc = 100, uint32_t decel = 100);
  void RelMove(int cmd_dist, uint32_t cmd_vel);
  int CylOpen(uint8_t obj_id, bool skip_sensor = false);
  int CylClose(uint8_t obj_id, bool skip_sensor = false);
  int VacOn(uint8_t obj_id, bool skip_sensor = false);
  int VacOff(uint8_t obj_id, bool skip_sensor = false);
  bool IsGripPhone(uint8_t obj_id);
  int GripPhone(uint8_t obj_id);

  /*  data format (12 bytes) addr 4byte, pos 4byte, vel 4byte */
  void WriteMotorData(uint8_t *p_data, uint8_t length);
  void WriteApCfgData(uint8_t *p_data, uint8_t length);
  void WriteCylData(uint8_t *p_data, uint8_t length);
  void WriteVacData(uint8_t *p_data, uint8_t length);
  void WriteSeqData(uint8_t *p_data, uint8_t length);
};
