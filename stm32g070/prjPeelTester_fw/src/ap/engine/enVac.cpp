#include "ap.h"
#include "enVac.h"

enVac::enVac()
{
}
enVac::enVac(cfg_t *p_cfg) : m_Cfg()
{
  m_Cfg = p_cfg;
}
enVac::~enVac()
{
}
enVac &enVac::operator=(const enVac &obj)
{
  this->m_Cfg = obj.m_Cfg;
  return *this;
}

void enVac::SetConfigData(cfg_t *p_cfg)
{
  m_Cfg = p_cfg;
}
/*----------- Vacuum 상태 확인  -----------------------*/
bool enVac::IsOn()
{
  bool ret = false;
  if (m_Cfg.sensor_io)
  {
    ret = m_Cfg.pApIo->IsOn(m_Cfg.sensor_io);
  }
  return ret;
}

bool enVac::IsOff()
{
  bool ret = false;
  if (m_Cfg.sensor_io)
  {
    ret = m_Cfg.pApIo->IsOff(m_Cfg.sensor_io);
  }
  return ret;
}

/*----------- Vacuum 동작 시작  -----------------------*/
int enVac::BlowOn()
{
  return m_Cfg.pApIo->OutputOn(m_Cfg.sol_io[EN_VACUUM_SOL_BLOW]);
}

int enVac::BlowOff()
{
  m_Cfg.pApIo->OutputOn(m_Cfg.sol_io[EN_VACUUM_SOL_BLOW]);
  delay(50);
  m_Cfg.pApIo->OutputOff(m_Cfg.sol_io[EN_VACUUM_SOL_BLOW]);
  return 0;
}

/*----------- Vacuum 동작  -----------------------*/
int enVac::On(bool skip_sensor)
{
  switch (m_Cfg.vac_type)
  {
  case suction:
    m_Cfg.pApIo->OutputOn(m_Cfg.sol_io[EN_VACUUM_SOL_SUCTION]);
    break;
  case suction_blow:
    m_Cfg.pApIo->OutputOn(m_Cfg.sol_io[EN_VACUUM_SOL_SUCTION]);
    m_Cfg.pApIo->OutputOff(m_Cfg.sol_io[EN_VACUUM_SOL_BLOW]);
    break;
  default:
    break;
  }

  if (skip_sensor)
  {
    return 0;
  }
  return Wait4OnCplt();
}

int enVac::Off(bool skip_sensor)
{
  switch (m_Cfg.vac_type)
  {
  case suction:
    m_Cfg.pApIo->OutputOff(m_Cfg.sol_io[EN_VACUUM_SOL_SUCTION]);
    break;
  case suction_blow:
    m_Cfg.pApIo->OutputOff(m_Cfg.sol_io[EN_VACUUM_SOL_SUCTION]);
    BlowOff();
    break;
  default:
    break;
  }

  if (skip_sensor)
  {
    return 0;
  }
  return Wait4OffCplt();
}

/*----------- Vacuum 동작완료시까지 Sleep  -----------------------*/
int enVac::Wait4OnCplt()
{
  int ret = -1;
  uint32_t pre_ms = millis();
  while ((millis() - pre_ms) < (uint32_t)(m_Cfg.pVacDat->timeout))
  {
    if (IsOn())
    {
      ret = 0;
      break;
    }
  }
  return ret;
}

int enVac::Wait4OffCplt()
{
  int ret = -1;
  uint32_t pre_ms = millis();
  while ((millis() - pre_ms) < (uint32_t)(m_Cfg.pVacDat->timeout))
  {
    if (IsOff())
    {
      ret = 0;
      break;
    }
  }
  return ret;
}
/*------------------------ Common Interface -----------------------*/
int enVac::SetData(vac_dat::dat_t *p_data)
{
  m_Cfg.pVacDat = p_data;
  return 0;
}

int enVac::GetData(vac_dat::dat_t *p_data)
{
  p_data = m_Cfg.pVacDat;
  return 0;
}
