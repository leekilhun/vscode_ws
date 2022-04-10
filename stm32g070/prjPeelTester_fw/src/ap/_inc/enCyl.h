/*
 * enCyl.h
 *
 *  Created on: 2022. 4. 7.
 *      Author: gns2l
 */

#pragma once


#define EN_CYLINDER_SOL_MAX             2
#define EN_CYLINDER_SOL_UP              0
#define EN_CYLINDER_SOL_DOWN            1
#define EN_CYLINDER_SENSOR_MAX          2
#define EN_CYLINDER_SENSOR_UP           0
#define EN_CYLINDER_SENSOR_DOWN         1

class enCyl:public icylinder
{
public:
  struct cfg_t{
    uint8_t cyl_id;
    uint32_t sol_io[EN_CYLINDER_SOL_MAX];
    uint32_t sensor_io[EN_CYLINDER_SENSOR_MAX];
    type_e  cyl_type;
    sol_e sol_type;
    iio* pApIo;
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
  enCyl();
  enCyl(cfg_t* p_cfg);
  virtual ~enCyl();
  enCyl& operator=(const enCyl& obj);

  void SetConfigData(cfg_t* p_cfg);
  /*----------- 실린더 상태 확인  -----------------------*/
  bool IsUp();

  bool IsDown();

  bool IsLeft();

  bool IsRight();

  bool IsForward();

  bool IsBackward();

  bool IsOpen();

  bool IsClose();

  bool IsLock();

  bool IsUnlock();

  bool IsTurn();

  bool IsReturn();
  /*----------- 실린더 이동   -----------------------*/

  int Up(bool skip_sensor = false);

  int Down(bool skip_sensor = false);

  int Left(bool skip_sensor = false);

  int Right(bool skip_sensor = false);

  int Forward(bool skip_sensor = false);

  int Backward(bool skip_sensor = false);

  int Open(bool skip_sensor = false);

  int Close(bool skip_sensor = false);

  int Lock(bool skip_sensor = false);

  int Unlock(bool skip_sensor = false);

  int Return(bool skip_sensor = false);

  int Turn(bool skip_sensor = false);

  /*----------- 실린더 이동 시작  -----------------------*/
  int Start();
  /*---------- 실린더 동작완료때까지 Sleep   --------------*/
  int Wait4UpCplt();

  int Wait4DownCplt();

  /*------------------------ Common Interface -----------------------*/

  int SetData(cyl_dat::dat_t* p_data);

  int GetData(cyl_dat::dat_t* p_data);

};