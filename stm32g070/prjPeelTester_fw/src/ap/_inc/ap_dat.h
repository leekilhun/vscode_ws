#pragma once
/*
 * ap_dat.h
 *
 * 작성자 : 이길훈
 * 22.04.05
 *  data 객체
 */


/****************************************************
 *	controller의 mcu eeprom data 정보를 읽는다
 ****************************************************/

/// <summary>
/// eeprom ap config data
/// </summary>
struct ap_dat
{
  struct dat_t
  {
    uint16_t  parm1;
    uint16_t  parm2;
  };

  enum class addr_e //
  {
    ap_mot_move, ap_mot_scale, ap_2, ap_3, ap_4, ap_5, ap_6, ap_7,
    _max
  };

  ap_dat::dat_t apcfg_dat[static_cast<uint8_t>(ap_dat::addr_e::_max)];

  void WriteData(ap_dat::addr_e addr, ap_dat::dat_t data);
  ap_dat::dat_t ReadData(ap_dat::addr_e addr);
  ap_dat::dat_t* GetData(ap_dat::addr_e addr);
  void LoadRomData();
  bool ClearRomData();
  void Write_MotCfg(ap_dat::dat_t data);
  ap_dat::dat_t Read_MotCfg();
};


struct axis_dat
{
  struct dat_t
  {
    int       cmd_pos{};
    uint32_t  cmd_vel{};
  };

  /*multiply by data length 8*/
  enum class addr_e
  {
    ready_pos, pos_clean_dust, pos_vinyl_suction, pos_vinyl_peel,
    pos_peel_cplt, pos_vinyl_reattach, pos_5, pos_6,
    _max
  };

  axis_dat::dat_t cmd_pos_dat[static_cast<uint8_t>(axis_dat::addr_e::_max)];

  void WriteData(axis_dat::addr_e addr, axis_dat::dat_t data);
  axis_dat::dat_t* GetData(axis_dat::addr_e addr);
  axis_dat::dat_t ReadData(axis_dat::addr_e addr);
  void LoadRomData();
  bool ClearRomData();

};

struct cyl_dat 
{
  struct dat_t
  {
    /** 동작 제한 시간 ms */
    uint16_t timeout;

    /** 동작 후 안정화 시간 ms*/
    uint16_t on_settling;
  };

  /* multiply by length 4*/
  enum class addr_e //
  {
    phone_clamp_cyl,
    drum_updown_cyl, drum_z_up_cyl, drum_stop_cyl,
    _max
  };

  cyl_dat::dat_t cyl_act_dat[static_cast<uint8_t>(cyl_dat::addr_e::_max)];

  void WriteData(cyl_dat::addr_e addr, cyl_dat::dat_t data);
  cyl_dat::dat_t* GetData(cyl_dat::addr_e addr);
  cyl_dat::dat_t ReadData(cyl_dat::addr_e addr);
  void LoadRomData();
  bool ClearRomData();
};

struct vac_dat {
  struct dat_t
  {
    /** 동작 제한 시간 */
    uint16_t timeout;
    /** ON 동작 후 안정화 시간 */
    uint16_t  on_settling;
  };

  enum class addr_e //
  {
    peel_drum_vac,
    peel_drum_tail_vac, peel_phone_jig, vac_3,
    _max
  };

  vac_dat::dat_t vac_act_dat[static_cast<uint8_t>(vac_dat::addr_e::_max)];

  void WriteData(vac_dat::addr_e addr, vac_dat::dat_t data);
  vac_dat::dat_t* GetData(vac_dat::addr_e addr);
  vac_dat::dat_t ReadData(vac_dat::addr_e addr);
  void LoadRomData();
  bool ClearRomData();

};



struct seq_dat {
  struct dat_t
  {
    uint16_t  parm1;
    uint16_t  parm2;
  };

  enum class addr_e //
  {
    seq_0, seq_1, seq_2, seq_3,
    _max
  };

  seq_dat::dat_t sequencing_dat[static_cast<uint8_t>(seq_dat::addr_e::_max)];

  void WriteData(seq_dat::addr_e addr, seq_dat::dat_t data);
  seq_dat::dat_t* GetData(seq_dat::addr_e addr);
  seq_dat::dat_t ReadData(seq_dat::addr_e addr);
  void LoadRomData();
  bool ClearRomData();
};





#define AP_LOG_DAT_BUFF_SIZE               60
#define AP_LOG_DAT_HEADER                  0x11
struct log_dat{
  struct head_t
  {
    uint8_t  header{};
    uint8_t  error_no{};
    uint8_t  obj_id{};
    uint8_t  step_no{};
  };
  struct dat_t
  {
    head_t head{};
    char  log[AP_LOG_DAT_BUFF_SIZE]{};

    void init(){
      head.header = AP_LOG_DAT_HEADER;
      head.error_no = 0;
      head.obj_id = 0;
      head.step_no = 0;
      memset(&log,0x00,AP_LOG_DAT_BUFF_SIZE);
    }

    inline void operator = (const log_dat::dat_t* p_dat){
      this->head =  p_dat->head;
      for (uint8_t i = 0; i < AP_LOG_DAT_BUFF_SIZE; i++) {
        this->log[i]=p_dat->log[i];
      }
    }

  };

  enum class addr_e //
  {
    log_00, log_01, log_02, log_03, log_04, log_05, log_06, log_07,
    log_10, log_11, log_12, log_13, log_14, log_15, log_16, log_17,
    log_20, log_21, log_22, log_23, log_24, log_25, log_26, log_27,
    log_30, log_31, log_32, log_33, log_34, log_35, log_36, log_37,
    _max
  };

  log_dat::dat_t log_buff;

  char* GetBuffer();

  uint8_t GetBufferLen();

  void WriteData(log_dat::addr_e addr, log_dat::dat_t data);

  log_dat::dat_t* GetData(log_dat::addr_e addr);

  uint8_t* Get(log_dat::addr_e addr);

  bool ClearRomData();
};
