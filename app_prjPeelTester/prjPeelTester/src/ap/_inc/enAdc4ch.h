/*
 * enADC4ch.h
 *
 *  Created on: Mar 16, 2022
 *      Author: gns2.lee
 */

#ifndef SRC_AP_ENGINE_ENADC4CH_H_
#define SRC_AP_ENGINE_ENADC4CH_H_

class enADC4ch
{

public:
  struct cfg_t  {
    Ap_reg* p_apReg;
    uint8_t ch;
    uint32_t id;
  };


public:
  enADC4ch ();
  virtual  ~enADC4ch ();


public:
  int Init(enADC4ch::cfg_t* cfg);

};
#endif /* SRC_AP_ENGINE_ENADC4CH_H_ */
