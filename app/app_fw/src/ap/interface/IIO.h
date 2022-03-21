/*
 * IIO.h
 *
 *  Created on: 2022. 2. 24.
 *      Author: gns2l
 */

#ifndef SRC_AP_INTERFACE_IIO_H_
#define SRC_AP_INTERFACE_IIO_H_


class IIO
{
public:

  virtual void SetBank_Out(uint8_t bank, uint8_t val) = 0;
  virtual uint8_t GetBank_Out(uint8_t bank) = 0;
  virtual uint8_t GetBank_In(uint8_t bank) = 0;

  virtual bool IsOn(uint32_t addr) = 0;
  virtual bool IsOff(uint32_t addr) = 0;
  virtual int OutputOn(uint32_t addr) = 0;
  virtual int OutputOff(uint32_t addr) = 0;
  virtual int OutputToggle(uint32_t addr) = 0;
};


#endif /* SRC_AP_INTERFACE_IIO_H_ */
