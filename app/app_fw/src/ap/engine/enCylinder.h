/*
 * enCylinder.h
 *
 *  Created on: 2022. 3. 1.
 *      Author: gns2l
 */

#ifndef SRC_AP_ENGINE_ENCYLINDER_H_
#define SRC_AP_ENGINE_ENCYLINDER_H_

#define EN_CYLINDER_SOL_MAX             2
#define EN_CYLINDER_SOL_UP              0
#define EN_CYLINDER_SOL_DOWN            1
#define EN_CYLINDER_SENSOR_MAX          2
#define EN_CYLINDER_SENSOR_UP           0
#define EN_CYLINDER_SENSOR_DOWN         1

class enCylinder:public ICylinder
{
public:
  struct cfg_t{
    uint8_t cyl_id;
    uint32_t sol_io[EN_CYLINDER_SOL_MAX];
    uint32_t sensor_io[EN_CYLINDER_SENSOR_MAX];
    type_e  cyl_type;
    sol_e sol_type;
    IIO* pApIo;
    cyl_dat::dat_t* pCylDat;
    inline void operator = (const cfg_t* p_cfg){
      this->cyl_id = p_cfg->cyl_id;
      this->cyl_type = p_cfg->cyl_type;
      this->sol_type = p_cfg->sol_type;
      this->pApIo=p_cfg->pApIo;
      this->pCylDat=p_cfg->pCylDat;

      for (uint8_t i = 0; i < EN_CYLINDER_SENSOR_MAX; i++) {
        this->sensor_io[i]=p_cfg->sensor_io[i];
      }
      for (uint8_t i = 0; i < EN_CYLINDER_SOL_MAX; i++) {
        this->sol_io[i]=p_cfg->sol_io[i];
      }

    }

  };
private:
  cfg_t m_Cfg;

public:
  enCylinder(){}
  enCylinder(cfg_t* p_cfg):m_Cfg(){
    m_Cfg = p_cfg;
  }
  virtual ~enCylinder(){}
  enCylinder& operator=(const enCylinder& obj) {
    this->m_Cfg = obj.m_Cfg;
    return *this;
  }

  void SetConfigData(cfg_t* p_cfg){
    m_Cfg = p_cfg;
  }
  /*----------- 실린더 상태 확인  -----------------------*/
  bool IsUp(){
    bool ret[EN_CYLINDER_SOL_MAX] = {false,};

    // UP Sensor가 On인지 확인
    if(m_Cfg.sensor_io[EN_CYLINDER_SOL_UP])
      ret[EN_CYLINDER_SOL_UP] = m_Cfg.pApIo->IsOn(m_Cfg.sensor_io[EN_CYLINDER_SOL_UP]);

    // Down Sensor가 Off인지 확인
    if(m_Cfg.sensor_io[EN_CYLINDER_SOL_DOWN])
      ret[EN_CYLINDER_SOL_DOWN] = m_Cfg.pApIo->IsOff(m_Cfg.sensor_io[EN_CYLINDER_SOL_DOWN]);

    //
    if( !ret[EN_CYLINDER_SOL_UP] || !ret[EN_CYLINDER_SOL_DOWN] )
      return false;

    return true;
  }

  bool IsDown() {
    bool ret[EN_CYLINDER_SOL_MAX] = {false,};

    // UP Sensor가 On인지 확인
    if(m_Cfg.sensor_io[EN_CYLINDER_SOL_UP])
      ret[EN_CYLINDER_SOL_UP] = m_Cfg.pApIo->IsOff(m_Cfg.sensor_io[EN_CYLINDER_SOL_UP]);

    // Down Sensor가 Off인지 확인
    if(m_Cfg.sensor_io[EN_CYLINDER_SOL_DOWN])
      ret[EN_CYLINDER_SOL_DOWN] = m_Cfg.pApIo->IsOn(m_Cfg.sensor_io[EN_CYLINDER_SOL_DOWN]);

    //
    if( !ret[EN_CYLINDER_SOL_UP] || !ret[EN_CYLINDER_SOL_DOWN] )
      return false;

    return true;
  }

  bool IsLeft(){
    return IsUp();
  }

  bool IsRight(){
    return IsDown();
  }

  bool IsForward(){
    return IsUp();
  }

  bool IsBackward(){
    return IsDown();
  }
  bool IsOpen(){
    return IsUp();
  }

  bool IsClose(){
    return IsDown();
  }

  bool IsLock(){
    return IsUp();
  }

  bool IsUnlock(){
    return IsDown();
  }

  bool IsTurn(){
    return IsUp();
  }

  bool IsReturn(){
    return IsDown();
  }
  /*----------- 실린더 이동   -----------------------*/

  int Up(bool skip_sensor = false){
    switch (m_Cfg.sol_type) {
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

    if(skip_sensor){
      return 0;
    }
    return Wait4UpCplt();
  }

  int Down(bool skip_sensor = false){
    switch (m_Cfg.sol_type) {
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

    if(skip_sensor){
      return 0;
    }
    return Wait4DownCplt();
  }

  int Left(bool skip_sensor = false){
    return Up();
  }

  int Right(bool skip_sensor = false){
    return Down();
  }

  int Forward(bool skip_sensor = false){
    return Up();
  }

  int Backward(bool skip_sensor = false){
    return Down();
  }

  int Open(bool skip_sensor = false){
    return Up();
  }

  int Close(bool skip_sensor = false){
    return Down();
  }

  int Lock(bool skip_sensor = false){
    return Up();
  }

  int Unlock(bool skip_sensor = false){
    return Down();
  }

  int Return(bool skip_sensor = false){
    return Up();
  }

  int Turn(bool skip_sensor = false){
    return Down();
  }

  /*----------- 실린더 이동 시작  -----------------------*/
  int Start(){
    return Up(true);
  }

  /*---------- 실린더 동작완료때까지 Sleep   --------------*/
  int Wait4UpCplt(){
    int ret = -1;
    uint32_t pre_ms = millis();
    while((millis() - pre_ms) < uint32_t(m_Cfg.pCylDat->timeout)) {
      if(IsUp()){
        ret = 0;
        break;
      }
    }
    return ret;
  }

  int Wait4DownCplt(){
    int ret = -1;
    uint32_t pre_ms = millis();
    while((millis() - pre_ms) < uint32_t(m_Cfg.pCylDat->timeout)) {
      if(IsDown()){
        ret = 0;
        break;
      }
    }
    return ret;
   }

  /*------------------------ Common Interface -----------------------*/

  int SetData(cyl_dat::dat_t* p_data){
    m_Cfg.pCylDat = p_data;
    return 0;
  }

  int GetData(cyl_dat::dat_t* p_data){
    p_data = m_Cfg.pCylDat;
    return 0;
  }

};


#endif /* SRC_AP_ENGINE_ENCYLINDER_H_ */
