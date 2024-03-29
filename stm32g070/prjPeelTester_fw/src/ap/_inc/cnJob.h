/*
 * cnJob.h
 *
 *  Created on: 2022. 4. 7.
 *      Author: gns2l
 * 
 *  시퀀싱 구성을 위한 클래스
 */

#pragma once


#define CN_JOB_MAX_BUFFER_LENGTH                40

#define CN_JOB_STEP_INIT                        0
#define CN_JOB_STEP_TODO                        1

#define CN_JOB_STEP_MOTOR_ON                    2
#define CN_JOB_STEP_MOTOR_ON_START              3
#define CN_JOB_STEP_MOTOR_ON_WAIT               4
#define CN_JOB_STEP_MOTOR_ON_END                5

#define CN_JOB_STEP_MOTOR_OFF                   6
#define CN_JOB_STEP_MOTOR_OFF_START             7
#define CN_JOB_STEP_MOTOR_OFF_WAIT              8
#define CN_JOB_STEP_MOTOR_OFF_END               9

#define CN_JOB_STEP_MOTOR_STOP                  10
#define CN_JOB_STEP_MOTOR_STOP_START            11
#define CN_JOB_STEP_MOTOR_STOP_WAIT             12
#define CN_JOB_STEP_MOTOR_STOP_END              13

#define CN_JOB_STEP_MOTOR_MOVE                  14
#define CN_JOB_STEP_MOTOR_MOVE_START            15
#define CN_JOB_STEP_MOTOR_MOVE_WAIT             16
#define CN_JOB_STEP_MOTOR_MOVE_END              17

#define CN_JOB_STEP_PHONE_JIG_OPEN              18
#define CN_JOB_STEP_PHONE_JIG_OPEN_START        19
#define CN_JOB_STEP_PHONE_JIG_OPEN_WAIT         20
#define CN_JOB_STEP_PHONE_JIG_OPEN_END          21

#define CN_JOB_STEP_PHONE_JIG_CLOSE             22
#define CN_JOB_STEP_PHONE_JIG_CLOSE_START       23
#define CN_JOB_STEP_PHONE_JIG_CLOSE_WAIT        24
#define CN_JOB_STEP_PHONE_JIG_CLOSE_END         25

#define CN_JOB_STEP_POS_ORIGIN                  26
#define CN_JOB_STEP_POS_ORIGIN_START            27
#define CN_JOB_STEP_POS_ORIGIN_WAIT             28
#define CN_JOB_STEP_POS_ORIGIN_END              29

#define CN_JOB_STEP_POS_READY                   30
#define CN_JOB_STEP_POS_READY_START             31
#define CN_JOB_STEP_POS_READY_WAIT              32
#define CN_JOB_STEP_POS_READY_END               33

#define CN_JOB_STEP_POS_CLEAN_DUST              34
#define CN_JOB_STEP_POS_CLEAN_DUST_START        35
#define CN_JOB_STEP_POS_CLEAN_DUST_WAIT         36
#define CN_JOB_STEP_POS_CLEAN_DUST_END          37

#define CN_JOB_STEP_POS_SUCTION_VINYL           38
#define CN_JOB_STEP_POS_SUCTION_VINYL_START     39
#define CN_JOB_STEP_POS_SUCTION_VINYL_WAIT      40
#define CN_JOB_STEP_POS_SUCTION_VINYL_END       41

#define CN_JOB_STEP_POS_PEEL_VINYL              42
#define CN_JOB_STEP_POS_PEEL_VINYL_START        43
#define CN_JOB_STEP_POS_PEEL_VINYL_WAIT         44
#define CN_JOB_STEP_POS_PEEL_VINYL_END          45

#define CN_JOB_STEP_POS_PEEL_VINYL_CPLT         46
#define CN_JOB_STEP_POS_PEEL_VINYL_CPLT_START   47
#define CN_JOB_STEP_POS_PEEL_VINYL_CPLT_WAIT    48
#define CN_JOB_STEP_POS_PEEL_VINYL_CPLT_END     49

#define CN_JOB_STEP_POS_REATTACH                50
#define CN_JOB_STEP_POS_REATTACH_START          51
#define CN_JOB_STEP_POS_REATTACH_WAIT           52
#define CN_JOB_STEP_POS_REATTACH_END            53

#define CN_JOB_STEP_POS_REATTACH_RUN            54
#define CN_JOB_STEP_POS_REATTACH_RUN_START      55
#define CN_JOB_STEP_POS_REATTACH_RUN_WAIT       56
#define CN_JOB_STEP_POS_REATTACH_RUN_END        57

#define CN_JOB_STEP_ROLLER_ORIGIN               70
#define CN_JOB_STEP_ROLLER_ORIGIN_START         71
#define CN_JOB_STEP_ROLLER_ORIGIN_WAIT          72
#define CN_JOB_STEP_ROLLER_ORIGIN_END           73

#define CN_JOB_STEP_DATA_CLEAR                  74
#define CN_JOB_STEP_DATA_CLEAR_START            75
#define CN_JOB_STEP_DATA_CLEAR_WAIT             76
#define CN_JOB_STEP_DATA_CLEAR_END              77

#define CN_JOB_STEP_WRITE_MOTOR_DATA            78
#define CN_JOB_STEP_WRITE_MOTOR_DATA_START      79
#define CN_JOB_STEP_WRITE_MOTOR_DATA_WAIT       80
#define CN_JOB_STEP_WRITE_MOTOR_DATA_END        81

#define CN_JOB_STEP_WRITE_AP_CFG                82
#define CN_JOB_STEP_WRITE_AP_CFG_START          83
#define CN_JOB_STEP_WRITE_AP_CFG_WAIT           84
#define CN_JOB_STEP_WRITE_AP_CFG_END            85

#define CN_JOB_STEP_WRITE_CYL_DATA              86
#define CN_JOB_STEP_WRITE_CYL_DATA_START        87
#define CN_JOB_STEP_WRITE_CYL_DATA_WAIT         88
#define CN_JOB_STEP_WRITE_CYL_DATA_END          89

#define CN_JOB_STEP_WRITE_VAC_DATA              90
#define CN_JOB_STEP_WRITE_VAC_DATA_START        91
#define CN_JOB_STEP_WRITE_VAC_DATA_WAIT         92
#define CN_JOB_STEP_WRITE_VAC_DATA_END          93

#define CN_JOB_STEP_WRITE_SEQ_DATA              94
#define CN_JOB_STEP_WRITE_SEQ_DATA_START        95
#define CN_JOB_STEP_WRITE_SEQ_DATA_WAIT         96
#define CN_JOB_STEP_WRITE_SEQ_DATA_END          97

#define CN_JOB_STEP_SEQ_INITIAL                 98
#define CN_JOB_STEP_SEQ_INITIAL_START           99
#define CN_JOB_STEP_SEQ_INITIAL_WAIT           100
#define CN_JOB_STEP_SEQ_INITIAL_END            111

#define CN_JOB_STEP_REL_MOVE                   112
#define CN_JOB_STEP_REL_MOVE_START             113
#define CN_JOB_STEP_REL_MOVE_WAIT              114
#define CN_JOB_STEP_REL_MOVE_END               115

#define CN_JOB_STEP_REATTACH_CPLT              116
#define CN_JOB_STEP_REATTACH_CPLT_START        117
#define CN_JOB_STEP_REATTACH_CPLT_WAIT         118
#define CN_JOB_STEP_REATTACH_CPLT_END          119

#define CN_JOB_STEP_WORK_COMPLETE              120
#define CN_JOB_STEP_WORK_COMPLETE_START        121
#define CN_JOB_STEP_WORK_COMPLETE_WAIT         122
#define CN_JOB_STEP_WORK_COMPLETE_END          123

#define CN_JOB_STEP_CHECK_VACUUM_ON            124
#define CN_JOB_STEP_CHECK_VACUUM_ON_RETRY      125

#define CN_JOB_STEP_TIMEOUT                    254
#define CN_JOB_STEP_END                        255

class cnJob
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
    enCyl *p_Cyl;
    enVac *p_Vac;
    cnAuto *p_AutoManger;
  };

private:
  cnAuto *m_pAuto;
  enFaxis *m_pFm;
  enCyl *m_pCyl;
  enVac *m_pVac;
  Ap_reg *m_pApReg;
  iio *m_pApIo;
  axis_dat *m_pApAxisDat;
  cyl_dat *m_pApCylDat;
  vac_dat *m_pApVacDat;
  ap_dat *m_pApCfgDat;
  seq_dat *m_pApSeqDat;
  uint8_t m_retryCnt;
  bool m_IsAutoReady;
  uint32_t m_AutoSpeed;
  uint8_t m_prcStep;

public:
  bool m_IsToDoState;

  uint8_t m_stepBuffer[CN_JOB_MAX_BUFFER_LENGTH];
  uint32_t m_pre_time;
  prc_step_t m_step;

  /****************************************************
   *  Constructor
   ****************************************************/
public:
  cnJob();
  virtual ~cnJob();

  /****************************************************
   *  func
   ****************************************************/
private:
  // for auto run
  void doRunStep();
  // for manual unit working
  void doOpJob();

public:
  int Init(cnJob::cfg_t *cfg);

  // machine process initialize
  int Initialize();
  
  // non-block event patrol
  void ThreadJob();

  // sequecing에 필요한 함수들
  void SetAutoSpeed(uint32_t speed);
  uint32_t GetAutoSpeed() const;
  uint8_t GetStep() const;
  bool IsCylOpen(uint8_t obj_id);
  bool IsCylClose(uint8_t obj_id);
  int CylOpen(uint8_t obj_id, bool skip_sensor = false);
  int CylClose(uint8_t obj_id, bool skip_sensor = false);
  bool IsVacOn(uint8_t obj_id);
  bool IsVacOff(uint8_t obj_id);
  int VacOn(uint8_t obj_id, bool skip_sensor = false);
  int VacOff(uint8_t obj_id, bool skip_sensor = false);
  bool IsMotorOn();
  bool IsMotorRun();
  bool IsMotorMoveCplt();
  void MotorOrigin();
  void MotorOnOff(bool on_off);
  void MotorStop();
  void MotorRun(int cmd_pos, uint32_t cmd_vel, uint32_t acc = 100, uint32_t decel = 100);
  void RelMove(int cmd_dist, uint32_t cmd_vel);

  /*  data format (12 bytes) addr 4byte, pos 4byte, vel 4byte */
  void WriteMotorData(uint8_t *p_data, uint8_t length);
  void WriteApCfgData(uint8_t *p_data, uint8_t length);
  void WriteCylData(uint8_t *p_data, uint8_t length);
  void WriteVacData(uint8_t *p_data, uint8_t length);
  void WriteSeqData(uint8_t *p_data, uint8_t length);
};