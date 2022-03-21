/*
 * enVacuum.h
 *
 *  Created on: 2022. 3. 1.
 *      Author: gns2l
 */

#ifndef SRC_AP_ENGINE_ENVACUUM_H_
#define SRC_AP_ENGINE_ENVACUUM_H_

#define EN_VACUUM_SOL_MAX             2
#define EN_VACUUM_SOL_SUCTION         0
#define EN_VACUUM_SOL_BLOW            1

class enVacuum:public IVacuum
{
public:
  struct cfg_t{
    uint8_t vac_id;
    uint32_t sol_io[EN_VACUUM_SOL_MAX];
    uint32_t sensor_io;
    type_e  vac_type;
    IIO* pApIo;
    vac_dat::dat_t* pVacDat;
    inline void operator = (const cfg_t* p_cfg){
      this->vac_id = p_cfg->vac_id;
      this->vac_type = p_cfg->vac_type;
      this->pApIo=p_cfg->pApIo;
      this->pVacDat=p_cfg->pVacDat;

      this->sensor_io=p_cfg->sensor_io;

      for (uint8_t i = 0; i < EN_VACUUM_SOL_MAX; i++) {
        this->sol_io[i]=p_cfg->sol_io[i];
      }

    }

  };
private:
  cfg_t m_Cfg;

public:
  enVacuum(){}
  enVacuum(cfg_t* p_cfg):m_Cfg(){
    m_Cfg = p_cfg;
  }
  virtual ~enVacuum(){}
  enVacuum& operator=(const enVacuum& obj) {
    this->m_Cfg = obj.m_Cfg;
    return *this;
  }

  void SetConfigData(cfg_t* p_cfg){
    m_Cfg = p_cfg;
  }
  /*----------- Vacuum 상태 확인  -----------------------*/
  bool IsOn() {
    bool ret = false;
    if (m_Cfg.sensor_io){
      ret = m_Cfg.pApIo->IsOn(m_Cfg.sensor_io);
    }
    return ret;
  }

  bool IsOff(){
    bool ret = false;
    if (m_Cfg.sensor_io){
      ret = m_Cfg.pApIo->IsOff(m_Cfg.sensor_io);
    }
    return ret;
  }

  /*----------- Vacuum 동작 시작  -----------------------*/
  int BlowOn(){
    return m_Cfg.pApIo->OutputOn(m_Cfg.sol_io[EN_VACUUM_SOL_BLOW]);
  }

  int BlowOff(){
    m_Cfg.pApIo->OutputOn(m_Cfg.sol_io[EN_VACUUM_SOL_BLOW]);
    delay(50);
    m_Cfg.pApIo->OutputOff(m_Cfg.sol_io[EN_VACUUM_SOL_BLOW]);
    return 0;
  }

  /*----------- Vacuum 동작  -----------------------*/
  int On(bool skip_sensor = false){
    switch (m_Cfg.vac_type) {
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

    if(skip_sensor){
      return 0;
    }
    return Wait4OnCplt();
  }

  int Off(bool skip_sensor = false){
    switch (m_Cfg.vac_type) {
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

    if(skip_sensor){
      return 0;
    }
    return Wait4OffCplt();
  }


  /*----------- Vacuum 동작완료시까지 Sleep  -----------------------*/
  int Wait4OnCplt(){
    int ret = -1;
    uint32_t pre_ms = millis();
    while((millis() - pre_ms) < uint32_t(m_Cfg.pVacDat->timeout)) {
      if(IsOn()){
        ret = 0;
        break;
      }
    }
    return ret;
  }

  int Wait4OffCplt(){
    int ret = -1;
    uint32_t pre_ms = millis();
    while((millis() - pre_ms) < uint32_t(m_Cfg.pVacDat->timeout)) {
      if(IsOff()){
        ret = 0;
        break;
      }
    }
    return ret;
  }
  /*------------------------ Common Interface -----------------------*/
  int SetData(vac_dat::dat_t* p_data){
    m_Cfg.pVacDat = p_data;
    return 0;
  }

  int GetData(vac_dat::dat_t* p_data){
    p_data = m_Cfg.pVacDat;
    return 0;
  }


};


#endif /* SRC_AP_ENGINE_ENVACUUM_H_ */
