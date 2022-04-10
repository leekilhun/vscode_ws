#include "ap_def.h"
#include "ap_log.h"

#ifdef _USE_HW_RTOS
#define AP_LOG_LOCK_BEGIN osMutexWait(ap_log_mutex_id, osWaitForever)
#define AP_LOG_LOCK_END osMutexRelease(ap_log_mutex_id)
#else
#define AP_LOG_LOCK_BEGIN
#define AP_LOG_LOCK_END
#endif

ap_log::ap_log()
{
}

ap_log::~ap_log()
{
}

void ap_log::Init()
{
  m_Head = m_Tail = 0;
}

uint8_t ap_log::Available()
{
  return (m_Head - m_Tail) % AP_LOG_QUE_BUFF_SIZE;
}

uint8_t ap_log::GetAddrNo()
{
  return (m_Head - (m_Head - m_Tail)) % AP_LOG_QUE_BUFF_SIZE;
}

bool ap_log::apLogWrite(log_dat::head_t head_info, const char *fmt, ...)
{
  bool ret = true;
  AP_LOG_LOCK_BEGIN;

  va_list args;
  int len;

  va_start(args, fmt);
  len = vsnprintf(m_log.GetBuffer(), m_log.GetBufferLen(), fmt, args);
  UNUSED(len);
  m_log.log_buff.head = head_info;

  uint8_t next_in;
  next_in = (m_Head + 1) % AP_UTIL_QUE_BUFF_SIZE;
  if (next_in != m_Tail)
  {
    // write
    m_log.WriteData(static_cast<log_dat::addr_e>(m_Head), m_log.log_buff);
    m_Head = next_in;
  }
  else
    ret = false;

  va_end(args);
  AP_LOG_LOCK_END;
  return ret;
}

bool ap_log::Get(log_dat::dat_t *p_data)
{
  bool ret = true;
  // AP_LOG_LOCK_BEGIN;

  uint8_t addr_no = GetAddrNo();
  p_data = m_log.GetData(static_cast<log_dat::addr_e>(addr_no));
  if (m_Tail != m_Head)
  {
    m_Tail = (m_Tail + 1) % AP_UTIL_QUE_BUFF_SIZE;
  }
  else
    ret = false;

  // AP_LOG_LOCK_END;
  return ret;
}

uint8_t *ap_log::Get()
{
  // AP_LOG_LOCK_BEGIN;

  uint8_t *ret = 0;
  // read *(p_data) = m_Buff[m_Tail];
  uint8_t addr_no = GetAddrNo();
  ret = m_log.Get(static_cast<log_dat::addr_e>(addr_no));

  if (m_Tail != m_Head)
  {
    m_Tail = (m_Tail + 1) % AP_UTIL_QUE_BUFF_SIZE;
  }
  else
    ret = 0;

  // AP_LOG_LOCK_END;
  return ret;
}