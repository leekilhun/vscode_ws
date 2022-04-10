/*
 * iio.h
 *
 *  Created on: 2022. 4. 7.
 *      Author: gns2l
 */

#pragma once

class iio
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

