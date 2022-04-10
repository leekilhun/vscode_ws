#include "ap_def.h"
#include "ap_dat.h"




#define APDAT_START_ADDRESS               0x0000

#define APDAT_START_ADDR                  APDAT_START_ADDRESS
#define APDAT_LENGTH                      4
#define APDAT_ADDR(x)                     APDAT_START_ADDR + ((x) * APDAT_LENGTH)
#define APDAT_CNT_MAX                     8
#define APDAT_END_ADDR                    APDAT_START_ADDR + (APDAT_LENGTH * APDAT_CNT_MAX)

#define APDAT_AXIS_POS_START_ADDR         APDAT_END_ADDR
#define APDAT_AXIS_POS_DATA_LENGTH        8
#define APDAT_AXIS_POS_DATA_ADDR(x)       APDAT_AXIS_POS_START_ADDR + ((x) * APDAT_AXIS_POS_DATA_LENGTH)
#define APDAT_AXIS_POS_DATA_CNT_MAX       8
#define APDAT_AXIS_POS_END_ADDR           APDAT_AXIS_POS_START_ADDR + (APDAT_AXIS_POS_DATA_LENGTH * APDAT_AXIS_POS_DATA_CNT_MAX)

#define APDAT_CYL_ACT_START_ADDR          APDAT_AXIS_POS_END_ADDR
#define APDAT_CYL_ACT_DATA_LENGTH         4
#define APDAT_CYL_ACT_DATA_ADDR(x)        APDAT_CYL_ACT_START_ADDR + ((x) * APDAT_CYL_ACT_DATA_LENGTH)
#define APDAT_CYL_ACT_DATA_CNT_MAX        8
#define APDAT_CYL_ACT_END_ADDR            APDAT_CYL_ACT_START_ADDR + (APDAT_CYL_ACT_DATA_LENGTH * APDAT_CYL_ACT_DATA_CNT_MAX)

#define APDAT_VAC_ACT_START_ADDR          APDAT_CYL_ACT_END_ADDR
#define APDAT_VAC_ACT_DATA_LENGTH         4
#define APDAT_VAC_ACT_DATA_ADDR(x)        APDAT_VAC_ACT_START_ADDR + ((x) * APDAT_VAC_ACT_DATA_LENGTH)
#define APDAT_VAC_ACT_DATA_CNT_MAX        8
#define APDAT_VAC_ACT_END_ADDR            APDAT_VAC_ACT_START_ADDR + (APDAT_VAC_ACT_DATA_LENGTH * APDAT_VAC_ACT_DATA_CNT_MAX)

#define APDAT_SEQ_START_ADDR              APDAT_VAC_ACT_END_ADDR
#define APDAT_SEQ_DATA_LENGTH             4
#define APDAT_SEQ_DATA_ADDR(x)            APDAT_SEQ_START_ADDR + ((x) * APDAT_SEQ_DATA_LENGTH)
#define APDAT_SEQ_DATA_CNT_MAX            8
#define APDAT_SEQ_END_ADDR                APDAT_SEQ_START_ADDR + (APDAT_SEQ_DATA_LENGTH * APDAT_SEQ_DATA_CNT_MAX)


#define APDAT_LOG_START_ADDR              APDAT_SEQ_END_ADDR
#define APDAT_LOG_DATA_LENGTH             64
#define APDAT_LOG_DATA_ADDR(x)            APDAT_LOG_START_ADDR + ((x) * APDAT_LOG_DATA_LENGTH)
#define APDAT_LOG_DATA_CNT_MAX            32
#define APDAT_LOG_END_ADDR                APDAT_LOG_START_ADDR + (APDAT_LOG_DATA_LENGTH * APDAT_LOG_DATA_CNT_MAX)


/****************************************************
 *  ap data
 ****************************************************/  
void ap_dat::WriteData(ap_dat::addr_e addr, ap_dat::dat_t data)
{
  apcfg_dat[static_cast<uint8_t>(addr)] = data;
  uint16_t rcv_data[2] = { data.parm1, data.parm2 };
  at24c64Write(APDAT_ADDR(static_cast<uint8_t>(addr)), (uint8_t *)&rcv_data[0], APDAT_LENGTH); 
}

ap_dat::dat_t ap_dat::ReadData(ap_dat::addr_e addr)
{
  return apcfg_dat[static_cast<uint8_t>(addr)];
}

ap_dat::dat_t* ap_dat::GetData(ap_dat::addr_e addr)
{
  return &apcfg_dat[static_cast<uint8_t>(addr)];
}

void ap_dat::LoadRomData()
{
  uint16_t rcv_data[2] = { 0, };
  for (uint8_t i = 0; i < static_cast<uint8_t>(ap_dat::addr_e::_max); i++)
  {
    at24c64Read(APDAT_ADDR(i), (uint8_t *)&rcv_data, APDAT_LENGTH);
    apcfg_dat[i].parm1 = rcv_data[0];
    apcfg_dat[i].parm2 = rcv_data[1];
  }
}

bool ap_dat::ClearRomData()
{
  uint8_t data[APDAT_LENGTH] = { 0, };
  bool ret = false;
  for (uint8_t i = 0; i < static_cast<uint8_t>(ap_dat::addr_e::_max); i++) {
     ret = at24c64Write(APDAT_ADDR(i), (uint8_t*)&data, APDAT_LENGTH);
  }
  return ret;
}

void ap_dat::Write_MotCfg(ap_dat::dat_t data)
{
  apcfg_dat[static_cast<uint8_t>(ap_dat::addr_e::ap_mot_move)] = data;
  uint16_t rcv_data[2] = {data.parm1, data.parm2};
  at24c64Write(APDAT_ADDR(static_cast<uint8_t>(ap_dat::addr_e::ap_mot_move)), (uint8_t *)&rcv_data[0], APDAT_LENGTH);
}

ap_dat::dat_t ap_dat::Read_MotCfg()
{
  return apcfg_dat[static_cast<uint8_t>(ap_dat::addr_e::ap_mot_move)];
}

/****************************************************
 *  axis data
 ****************************************************/  
void axis_dat::WriteData(axis_dat::addr_e addr, axis_dat::dat_t data)
{
  cmd_pos_dat[static_cast<uint8_t>(addr)] = data;
  uint32_t rcv_data[2] = {(uint32_t)data.cmd_pos,data.cmd_vel};
  at24c64Write(APDAT_AXIS_POS_DATA_ADDR(static_cast<uint8_t>(addr)),(uint8_t *)&rcv_data[0], APDAT_AXIS_POS_DATA_LENGTH);
}

axis_dat::dat_t* axis_dat::GetData(axis_dat::addr_e addr)
{
  return &cmd_pos_dat[static_cast<uint8_t>(addr)];
}

axis_dat::dat_t axis_dat::ReadData(axis_dat::addr_e addr)
{
  return cmd_pos_dat[static_cast<uint8_t>(addr)];
}

void axis_dat::LoadRomData()
{
  uint32_t rcv_data[2] = { 0, };
  for (uint8_t i = 0; i < static_cast<uint8_t>(axis_dat::addr_e::_max); i++)
  {
    at24c64Read(APDAT_AXIS_POS_DATA_ADDR(i), (uint8_t *)&rcv_data, APDAT_AXIS_POS_DATA_LENGTH);
    cmd_pos_dat[i].cmd_pos = (int)rcv_data[0];
    cmd_pos_dat[i].cmd_vel = rcv_data[1];
  }
}

bool axis_dat::ClearRomData()
{
  uint8_t data[8] = { 0, };
  bool ret = false;
  for (uint8_t i = 0; i < static_cast<uint8_t>(axis_dat::addr_e::_max); i++)
  {
    ret = at24c64Write(APDAT_AXIS_POS_DATA_ADDR(i), (uint8_t *)&data, APDAT_AXIS_POS_DATA_LENGTH);
  }
  return ret;
}


/****************************************************
 *  cylinder data
 ****************************************************/  
void cyl_dat::WriteData(cyl_dat::addr_e addr, cyl_dat::dat_t data)
{
  cyl_act_dat[static_cast<uint8_t>(addr)] = data;
  uint16_t rcv_data[2] = { data.timeout, data.on_settling };
  at24c64Write(APDAT_CYL_ACT_DATA_ADDR(static_cast<uint8_t>(addr)), (uint8_t *)&rcv_data[0], APDAT_CYL_ACT_DATA_LENGTH);
}

cyl_dat::dat_t* cyl_dat::GetData(cyl_dat::addr_e addr)
{
  return &cyl_act_dat[static_cast<uint8_t>(addr)];
}

cyl_dat::dat_t cyl_dat::ReadData(cyl_dat::addr_e addr)
{
  return cyl_act_dat[static_cast<uint8_t>(addr)];
}

void cyl_dat::LoadRomData()
{
  uint16_t rcv_data[2] = {0, };
  for (uint8_t i = 0; i < static_cast<uint8_t>(cyl_dat::addr_e::_max); i++)
  {
    at24c64Read(APDAT_CYL_ACT_DATA_ADDR(i), (uint8_t *)&rcv_data, APDAT_CYL_ACT_DATA_LENGTH);
    cyl_act_dat[i].timeout = rcv_data[0];
    cyl_act_dat[i].on_settling = rcv_data[1];
  }
}

bool cyl_dat::ClearRomData()
{
  uint8_t data[APDAT_CYL_ACT_DATA_LENGTH] = { 0, };
  bool ret = false;
  for (uint8_t i = 0; i < static_cast<uint8_t>(cyl_dat::addr_e::_max); i++)
  {
    ret = at24c64Write(APDAT_CYL_ACT_DATA_ADDR(i), (uint8_t *)&data, APDAT_CYL_ACT_DATA_LENGTH);
  }
  return ret;
}


/****************************************************
 *  vacuum data
 ****************************************************/  
void vac_dat::WriteData(vac_dat::addr_e addr, vac_dat::dat_t data)
{
  vac_act_dat[static_cast<uint8_t>(addr)] = data;
  uint16_t rcv_data[2] = { data.timeout, data.on_settling };   
  at24c64Write(APDAT_VAC_ACT_DATA_ADDR(static_cast<uint8_t>(addr)), (uint8_t *)&rcv_data[0], APDAT_VAC_ACT_DATA_LENGTH);
}

vac_dat::dat_t* vac_dat::GetData(vac_dat::addr_e addr)
{
  return &vac_act_dat[static_cast<uint8_t>(addr)];
}

vac_dat::dat_t vac_dat::ReadData(vac_dat::addr_e addr)
{
  return vac_act_dat[static_cast<uint8_t>(addr)];
}

void vac_dat::LoadRomData()
{
  uint16_t rcv_data[2] = { 0, };
  for (uint8_t i = 0; i < static_cast<uint8_t>(vac_dat::addr_e::_max); i++)
  {
    at24c64Read(APDAT_VAC_ACT_DATA_ADDR(i), (uint8_t *)&rcv_data, APDAT_VAC_ACT_DATA_LENGTH);
    vac_act_dat[i].timeout = rcv_data[0];
    vac_act_dat[i].on_settling = rcv_data[1];
  }
}

bool vac_dat::ClearRomData()
{
  uint8_t data[APDAT_VAC_ACT_DATA_LENGTH] = { 0, };
  bool ret = false;
  for (uint8_t i = 0; i < static_cast<uint8_t>(vac_dat::addr_e::_max); i++)
  {
    ret = at24c64Write(APDAT_VAC_ACT_DATA_ADDR(i), (uint8_t *)&data, APDAT_VAC_ACT_DATA_LENGTH);
  }
  return ret;
}


/****************************************************
 *  sequence data
 ****************************************************/  
void seq_dat::WriteData(seq_dat::addr_e addr, seq_dat::dat_t data)
{
  sequencing_dat[static_cast<uint8_t>(addr)] = data;
  uint16_t rcv_data[2] = {data.parm1, data.parm2};
  at24c64Write(APDAT_SEQ_DATA_ADDR(static_cast<uint8_t>(addr)), (uint8_t *)&rcv_data[0], APDAT_SEQ_DATA_LENGTH);
}

seq_dat::dat_t* seq_dat::GetData(seq_dat::addr_e addr)
{
  return &sequencing_dat[static_cast<uint8_t>(addr)];
}

seq_dat::dat_t seq_dat::ReadData(seq_dat::addr_e addr)
{
  return sequencing_dat[static_cast<uint8_t>(addr)];
}

void seq_dat::LoadRomData()
{
  uint16_t rcv_data[2] = {0, };
  for (uint8_t i = 0; i < static_cast<uint8_t>(seq_dat::addr_e::_max); i++)
  {
    at24c64Read(APDAT_SEQ_DATA_ADDR(i), (uint8_t *)&rcv_data, APDAT_SEQ_DATA_LENGTH);
    sequencing_dat[i].parm1 = rcv_data[0];
    sequencing_dat[i].parm2 = rcv_data[1];
  }
}

bool seq_dat::ClearRomData()
{
  uint8_t data[APDAT_SEQ_DATA_LENGTH] = { 0, };
  bool ret = false;
  for (uint8_t i = 0; i < static_cast<uint8_t>(seq_dat::addr_e::_max); i++)
  {
    ret = at24c64Write(APDAT_SEQ_DATA_ADDR(i), (uint8_t *)&data, APDAT_SEQ_DATA_LENGTH);
  }
  return ret;
}


/****************************************************
 *  log data
 ****************************************************/
char *log_dat::GetBuffer()
{
  return &log_buff.log[0];
}

uint8_t log_dat::GetBufferLen()
{
  return AP_LOG_DAT_BUFF_SIZE;
}

void log_dat::WriteData(log_dat::addr_e addr, log_dat::dat_t data)
{
  log_buff = data;
  at24c64Write(APDAT_LOG_DATA_ADDR(static_cast<uint8_t>(addr)), (uint8_t *)&log_buff, APDAT_LOG_DATA_LENGTH);
}

log_dat::dat_t *log_dat::GetData(log_dat::addr_e addr)
{
  at24c64Read(APDAT_LOG_DATA_ADDR(static_cast<uint8_t>(addr)), (uint8_t *)&log_buff, APDAT_LOG_DATA_LENGTH);
  return &log_buff;
}

uint8_t* log_dat::Get(log_dat::addr_e addr)
{
  at24c64Read(APDAT_LOG_DATA_ADDR(static_cast<uint8_t>(addr)), (uint8_t *)&log_buff, APDAT_LOG_DATA_LENGTH);
  return (uint8_t *)&log_buff;
}

bool log_dat::ClearRomData()
{
  log_buff.init();
  bool ret = false;
  for (uint8_t i = 0; i < static_cast<uint8_t>(vac_dat::addr_e::_max); i++)
  {
    ret = at24c64Write(APDAT_LOG_DATA_ADDR(i), (uint8_t *)&log_buff, APDAT_LOG_DATA_LENGTH);
  }
  return ret;
}
