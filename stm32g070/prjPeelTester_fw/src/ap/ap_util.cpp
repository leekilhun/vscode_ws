#include "ap_def.h"
#include "ap_util.h"


bool scope::between(int amt, int low, int high)
{
  bool ret = true;
  if (amt < low) {
    ret = false;
  }
  else if (amt > high) {
    ret = false;
  }
  return ret;
}

int conv::DwToInt(uint8_t* bytes)
{
  int a = bytes[0] & 0xFF;
  a |= ((bytes[1] << 8) & 0xFF00);
  a |= ((bytes[2] << 16) & 0xFF0000);
  a |= ((bytes[3] << 24) & 0xFF000000);

  return a;
}

uint32_t conv::DwToUint(uint8_t* bytes)
{
  uint32_t a = bytes[0] & 0xFF;
  a |= ((bytes[1] << 8) & 0xFF00);
  a |= ((bytes[2] << 16) & 0xFF0000);
  a |= ((bytes[3] << 24) & 0xFF000000);

  return a;
}
