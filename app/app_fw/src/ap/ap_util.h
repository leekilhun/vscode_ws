/*
 * status_reg.h
 *
 *  Created on: 2022. 1. 26.
 *      Author: gns2l
 */

#ifndef SRC_AP_AP_UTIL_H_
#define SRC_AP_AP_UTIL_H_


inline bool between(int amt, int low, int high) {
  bool ret = true;
  if (amt < low) {
    ret = false;
  }
  else if (amt > high) {
    ret = false;
  }
  return ret;
}

struct BankBit_t { uint8_t bank; uint8_t bit;  };


#define AP_UTIL_QUE_BUFF_SIZE               32
namespace buffer
{
  template <typename T>
  class _que
  {
    T m_Buff[AP_UTIL_QUE_BUFF_SIZE];
    uint8_t m_Head, m_Tail;

  public:
    _que(){m_Head = 0; m_Tail = 0 ;}
    virtual ~_que() { }

    inline void Init() {
      m_Head = m_Tail = 0;
    }

    inline bool Write(T* p_data, uint8_t length) {
      bool ret = true;
      uint32_t next_in;
      for (uint8_t i = 0; i < length; i++) {
        next_in = (m_Head + 1) % AP_UTIL_QUE_BUFF_SIZE;
        if (next_in != m_Tail) {
          m_Buff[m_Head] = p_data[i];
          m_Head = next_in;
        }
        else {
          ret = false;
          break;
        }
      }
      return ret;
    }

    inline bool Read(T* p_data, uint8_t length) {
      bool ret = true;
      for (uint8_t i = 0; i < length; i++) {
        p_data[i] = m_Buff[m_Tail];
        if (m_Tail != m_Head) {
          m_Tail = (m_Tail + 1) % AP_UTIL_QUE_BUFF_SIZE;
        }
        else {
          ret = false;
          break;
        }
      }
      return ret;
    }

    inline uint8_t Available() {
      return (m_Head - m_Tail) % AP_UTIL_QUE_BUFF_SIZE;
    }

    inline bool Put(T data) {
      bool ret = true;
      uint32_t next_in;
      next_in = (m_Head + 1) % AP_UTIL_QUE_BUFF_SIZE;
      if (next_in != m_Tail) {
        m_Buff[m_Head] = data;
        m_Head = next_in;
      }
      else
        ret = false;

      return ret;
    }

    inline bool Get(T* p_data) {
      bool ret = true;
      *(p_data) = m_Buff[m_Tail];
      if (m_Tail != m_Head) {
        m_Tail = (m_Tail + 1) % AP_UTIL_QUE_BUFF_SIZE;
      }
      else
        ret = false;

      return ret;
    }

    inline T Pop(uint32_t addr) {
      if (addr > (AP_UTIL_QUE_BUFF_SIZE - 1))
        return m_Buff[m_Tail];
      return m_Buff[addr];
    }



    inline void Pop(uint32_t addr, T* p_data) {
      if (addr > (AP_UTIL_QUE_BUFF_SIZE - 1))
        return;
      *(p_data) = m_Buff[addr];
    }

    inline uint32_t GetSize() {
      return ((m_Head - m_Tail) % AP_UTIL_QUE_BUFF_SIZE);
    }


    inline T operator[](uint32_t addr) {
      return Pop[addr];
    }

    inline void Flush() {
      m_Head = m_Tail = 0;
    }

  };
}

namespace trans
{
  inline uint8_t SplitArgs(char* arg_str, char** argv, const char* delim_chars, int max)
  {
    uint8_t argc = 0;
    char* tok = nullptr;
    char* next_ptr = nullptr;

    for (tok = strtok_r(arg_str, delim_chars, &next_ptr); tok; tok = strtok_r(NULL, delim_chars, &next_ptr))
    {
      argv[argc++] = tok;
      if (argc >= max)
        return argc - 1;
    }
    return argc;
  }
}







namespace conv
{
  inline int DwToInt(uint8_t* bytes) {
    int a = bytes[0] & 0xFF;
    a |= ((bytes[1] << 8) & 0xFF00);
    a |= ((bytes[2] << 16) & 0xFF0000);
    a |= ((bytes[3] << 24) & 0xFF000000);

    return a;
  }

  /// <summary>
  /// 4����Ʈ ������ bank(0~3), bit(0~7) ������ ���
  /// �����Ѵ�.
  /// </summary>
  /// <param name="data">4����Ʈ ����</param>
  /// <returns>�� 1����Ʈ bank, �� 1����Ʈ bit </returns>
  inline BankBit_t DwToBankBitNo(uint32_t data) {
    BankBit_t ret = {};
    /*div_t div_result = div(data,8);
    ret.bank = div_result.quot ;
    ret.bit = div_result.rem ;*/
    for (uint8_t i = 0; i < 4; i++) {
      uint8_t tmp = (uint8_t)(data >> (8 * i));
      if (tmp) {
        ret.bank = i;
        for (uint8_t a = 0; a < 8; a++) {
          if (tmp >> (1 * a)) {
            ret.bit = a;
            goto _out;
          }
        }
      }
    }
    _out:
    return ret;
  }
}



#define REG_BANK_MAX              8
enum class reg_bank{
  err_H,
  err_L,
  settingReg,
  stateReg,
  _max
};

enum class reg_ {
  b0,
  b1,
  b2,
  b3,
  b4,
  b5,
  b6,
  b7,
  _max
};

#define REG_BIT(x)          static_cast<reg_>(x)

class _Regist {
  class _elem {
    bool m_bit[static_cast<uint8_t>(reg_::_max)];

    explicit _elem(const _elem& ref):m_bit(){}
    _elem& operator=(const _elem& ref) { return *this; }
  public:
    explicit _elem() :m_bit() {

    };
    virtual ~_elem() {};

  public:
    inline bool& operator[](const reg_ elem_no) {
      return m_bit[static_cast<int>(elem_no)];
    }

    inline void operator = (const uint8_t in_data) {
      uint8_t data = 0;
      for (uint8_t i=0; i < 8; i++) {
        data = in_data;
        data = data >> i;
        data = 0xfe | data;
        data = ~(data);
        m_bit[i] = !data; //!(~(0xfe | (data >> i)));
      }
    }

    inline uint8_t get() {
      uint8_t data;
      data = (int)m_bit[0] | ((int)m_bit[1] * 2) | ((int)m_bit[2] * 4) | ((int)m_bit[3] * 8)
        | ((int)m_bit[4] * 16) | ((int)m_bit[5] * 32) | ((int)m_bit[6] * 64 | ((int)m_bit[7] * 128));
      return data;
    }

    inline uint8_t on_bit(const reg_ elem_no) {
      m_bit[static_cast<int>(elem_no)] = true;
      return get();
    }

    inline uint8_t off_bit(const reg_ elem_no) {
      m_bit[static_cast<int>(elem_no)] = false;
      return get();
    }

  };

  _elem m_byte[REG_BANK_MAX];
  uint8_t m_data[REG_BANK_MAX];
  uint8_t m_size;

  explicit _Regist(const _Regist& ref) :m_byte(), m_data(), m_size() {}
  _Regist& operator=(const _Regist ref) { return *this; }
public:
  _Regist(uint8_t size = REG_BANK_MAX) :m_byte(), m_data(),m_size(size) { };
  virtual ~_Regist() {};

public:
  inline void ClearAll() {
    for (uint8_t i =0;i<m_size;i++){
      Clear(i);
    }
  }

  inline void Clear(uint8_t bank) {
    m_byte[bank] = 0;
  }

  inline uint8_t SetOn(uint8_t bank, reg_ reg_no) {
    m_byte[bank][reg_no] = true;
    return m_byte[bank].get();
  }
  inline uint8_t SetOff(uint8_t bank, reg_ reg_no) {
    m_byte[bank][reg_no] = false;
    return m_byte[bank].get();
  }

  inline bool Get(uint8_t bank, reg_ bit) {
    return m_byte[bank][bit];
  }

  inline uint8_t* Out() {
    for (uint8_t i =0;i<m_size;i++){
      m_data[i] = m_byte[i].get();
    }
    return m_data;
  }

  inline uint8_t GetSize() const {
    return m_size;
  }

  inline _elem& operator[](uint8_t bank) {
    return m_byte[bank];
  }

  inline uint8_t operator[](const int reg) {
    return m_byte[reg].get();
  }
};





#endif /* SRC_AP_AP_UTIL_H_ */
