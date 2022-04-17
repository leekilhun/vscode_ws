/*
 * ap_log.h
 *
 *  Created on: 2022. 3. 15.
 *      Author: gns2.lee
 * 
 *  eeprom에 log 정보를 기록한다
 */

#pragma once

#define AP_LOG_QUE_BUFF_SIZE 32

class ap_log
{
  /****************************************************
   *  data
   ****************************************************/
  log_dat m_log;

  uint8_t m_Head, m_Tail;
#ifdef _USE_HW_RTOS
  osMutexId ap_log_mutex_id;
#endif

  /****************************************************
   *  Constructor
   ****************************************************/
public:
  ap_log();
  virtual ~ap_log();

  /****************************************************
   *  func
   ****************************************************/
  void Init();
  uint8_t Available();
  uint8_t GetAddrNo();
  bool apLogWrite(log_dat::head_t *p_head_info, const char *fmt, ...);
  bool Get(log_dat::dat_t *p_data);
  uint8_t *Get();
};
