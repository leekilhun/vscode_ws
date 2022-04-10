/*
 * enVac.h
 *
 *  Created on: 2022. 4. 7.
 *      Author: gns2l
 *  
 *  vacuum 제어를 위한 클래스
 */

#pragma once

#define EN_VACUUM_SOL_MAX             2
#define EN_VACUUM_SOL_SUCTION         0
#define EN_VACUUM_SOL_BLOW            1

class enVac:public ivacuum
{
public:
  struct cfg_t{
    uint8_t vac_id;
    uint32_t sol_io[EN_VACUUM_SOL_MAX];
    uint32_t sensor_io;
    type_e  vac_type;
    iio* pApIo;
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
  enVac();
  enVac(cfg_t* p_cfg);
  virtual ~enVac();

  enVac& operator=(const enVac& obj);

  void SetConfigData(cfg_t* p_cfg);
  /*----------- Vacuum 상태 확인  -----------------------*/
  bool IsOn();

  bool IsOff();

  /*----------- Vacuum 동작 시작  -----------------------*/
  int BlowOn();

  int BlowOff();

  /*----------- Vacuum 동작  -----------------------*/
  int On(bool skip_sensor = false);

  int Off(bool skip_sensor = false);

  /*----------- Vacuum 동작완료시까지 Sleep  -----------------------*/
  int Wait4OnCplt();

  int Wait4OffCplt();
  /*------------------------ Common Interface -----------------------*/
  int SetData(vac_dat::dat_t* p_data);

  int GetData(vac_dat::dat_t* p_data);


};
