#include "ap.h"
#include "enCyl.h"

enCyl::enCyl() {}
enCyl::enCyl(cfg_t *p_cfg) : m_Cfg()
{
  m_Cfg = p_cfg;
}
enCyl::~enCyl()
{
}
enCyl &enCyl::operator=(const enCyl &obj)
{
  this->m_Cfg = obj.m_Cfg;
  return *this;
}

void enCyl::SetConfigData(cfg_t *p_cfg)
{
  m_Cfg = p_cfg;
}
/*----------- 실린더 상태 확인  -----------------------*/
bool enCyl::IsUp()
{
  bool ret[EN_CYLINDER_SOL_MAX] = {
      false,
  };

  // UP Sensor가 On인지 확인
  if (m_Cfg.sensor_io[EN_CYLINDER_SOL_UP])
    ret[EN_CYLINDER_SOL_UP] = m_Cfg.pApIo->IsOn(m_Cfg.sensor_io[EN_CYLINDER_SOL_UP]);

  // Down Sensor가 Off인지 확인
  if (m_Cfg.sensor_io[EN_CYLINDER_SOL_DOWN])
    ret[EN_CYLINDER_SOL_DOWN] = m_Cfg.pApIo->IsOff(m_Cfg.sensor_io[EN_CYLINDER_SOL_DOWN]);

  //
  if (!ret[EN_CYLINDER_SOL_UP] || !ret[EN_CYLINDER_SOL_DOWN])
    return false;

  return true;
}

bool enCyl::IsDown()
{
  bool ret[EN_CYLINDER_SOL_MAX] = {
      false,
  };

  // UP Sensor가 On인지 확인
  if (m_Cfg.sensor_io[EN_CYLINDER_SOL_UP])
    ret[EN_CYLINDER_SOL_UP] = m_Cfg.pApIo->IsOff(m_Cfg.sensor_io[EN_CYLINDER_SOL_UP]);

  // Down Sensor가 Off인지 확인
  if (m_Cfg.sensor_io[EN_CYLINDER_SOL_DOWN])
    ret[EN_CYLINDER_SOL_DOWN] = m_Cfg.pApIo->IsOn(m_Cfg.sensor_io[EN_CYLINDER_SOL_DOWN]);

  //
  if (!ret[EN_CYLINDER_SOL_UP] || !ret[EN_CYLINDER_SOL_DOWN])
    return false;

  return true;
}

bool enCyl::IsLeft()
{
  return IsUp();
}

bool enCyl::IsRight()
{
  return IsDown();
}

bool enCyl::IsForward()
{
  return IsUp();
}

bool enCyl::IsBackward()
{
  return IsDown();
}
bool enCyl::IsOpen()
{
  return IsUp();
}

bool enCyl::IsClose()
{
  return IsDown();
}

bool enCyl::IsLock()
{
  return IsUp();
}

bool enCyl::IsUnlock()
{
  return IsDown();
}

bool enCyl::IsTurn()
{
  return IsUp();
}

bool enCyl::IsReturn()
{
  return IsDown();
}
/*----------- 실린더 이동   -----------------------*/

int enCyl::Up(bool skip_sensor)
{
  switch (m_Cfg.sol_type)
  {
  case one:
    m_Cfg.pApIo->OutputOn(m_Cfg.sol_io[EN_CYLINDER_SOL_UP]);
    break;
  case reserse_one:
    m_Cfg.pApIo->OutputOff(m_Cfg.sol_io[EN_CYLINDER_SOL_UP]);
    break;
  case two:
    m_Cfg.pApIo->OutputOn(m_Cfg.sol_io[EN_CYLINDER_SOL_UP]);
    m_Cfg.pApIo->OutputOff(m_Cfg.sol_io[EN_CYLINDER_SOL_DOWN]);
    break;
  default:
    break;
  }

  if (skip_sensor)
  {
    return 0;
  }
  return Wait4UpCplt();
}

int enCyl::Down(bool skip_sensor)
{
  switch (m_Cfg.sol_type)
  {
  case one:
    m_Cfg.pApIo->OutputOn(m_Cfg.sol_io[EN_CYLINDER_SOL_DOWN]);
    break;
  case reserse_one:
    m_Cfg.pApIo->OutputOff(m_Cfg.sol_io[EN_CYLINDER_SOL_DOWN]);
    break;
  case two:
    m_Cfg.pApIo->OutputOff(m_Cfg.sol_io[EN_CYLINDER_SOL_UP]);
    m_Cfg.pApIo->OutputOn(m_Cfg.sol_io[EN_CYLINDER_SOL_DOWN]);
    break;
  default:
    break;
  }

  if (skip_sensor)
  {
    return 0;
  }
  return Wait4DownCplt();
}

int enCyl::Left(bool skip_sensor)
{
  return Up(skip_sensor);
}

int enCyl::Right(bool skip_sensor)
{
  return Down(skip_sensor);
}

int enCyl::Forward(bool skip_sensor)
{
  return Up(skip_sensor);
}

int enCyl::Backward(bool skip_sensor)
{
  return Down(skip_sensor);
}

int enCyl::Open(bool skip_sensor)
{
  return Up(skip_sensor);
}

int enCyl::Close(bool skip_sensor)
{
  return Down(skip_sensor);
}

int enCyl::Lock(bool skip_sensor)
{
  return Up(skip_sensor);
}

int enCyl::Unlock(bool skip_sensor)
{
  return Down(skip_sensor);
}

int enCyl::Return(bool skip_sensor)
{
  return Up(skip_sensor);
}

int enCyl::Turn(bool skip_sensor)
{
  return Down(skip_sensor);
}

/*----------- 실린더 이동 시작  -----------------------*/
int enCyl::Start()
{
  return Up(true);
}

/*---------- 실린더 동작완료때까지 Sleep   --------------*/
int enCyl::Wait4UpCplt()
{
  int ret = -1;
  uint32_t pre_ms = millis();
  while ((millis() - pre_ms) < (uint32_t)(m_Cfg.pCylDat->timeout))
  {
    if (IsUp())
    {
      ret = 0;
      break;
    }
  }
  return ret;
}

int enCyl::Wait4DownCplt()
{
  int ret = -1;
  uint32_t pre_ms = millis();
  while ((millis() - pre_ms) < (uint32_t)(m_Cfg.pCylDat->timeout))
  {
    if (IsDown())
    {
      ret = 0;
      break;
    }
  }
  return ret;
}

/*------------------------ Common Interface -----------------------*/

int enCyl::SetData(cyl_dat::dat_t *p_data)
{
  m_Cfg.pCylDat = p_data;
  return 0;
}

int enCyl::GetData(cyl_dat::dat_t *p_data)
{
  p_data = m_Cfg.pCylDat;
  return 0;
}
