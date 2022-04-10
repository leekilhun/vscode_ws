#pragma once

/*
 * ap_util.h
 *
 *  Created on: 2022. 4. 7.
 *      Author: gns2l
 */

/// <summary>
/// 입력 값 확인
/// </summary>
namespace scope
{
  /// <summary>
  /// 입력 값이 범위 내 있는지 확인
  /// </summary>
  /// <param name="amt">입력 값</param>
  /// <param name="low">하위 값</param>
  /// <param name="high">상위 값</param>
  /// <returns>true 범위 내, false 범위 외 </returns>
  bool between(int amt, int low, int high);
}

/// <summary>
/// data 변경
/// </summary>
namespace conv
{
  int DwToInt(uint8_t *bytes);
  uint32_t DwToUint(uint8_t *bytes);
}



/// <summary>
/// 데이터 버퍼 클래스
/// </summary>
namespace buffer
{
  #define AP_UTIL_QUE_BUFF_SIZE 32
  template <typename T>
  class _que
  {
    T m_Buff[AP_UTIL_QUE_BUFF_SIZE];
    uint8_t m_Head, m_Tail;

  public:
    _que()
    {
      m_Head = 0;
      m_Tail = 0;
    }
    virtual ~_que() {}

    inline void Init()
    {
      m_Head = m_Tail = 0;
    }

    inline bool Write(T *p_data, uint8_t length)
    {
      bool ret = true;
      uint32_t next_in;
      for (uint8_t i = 0; i < length; i++)
      {
        next_in = (m_Head + 1) % AP_UTIL_QUE_BUFF_SIZE;
        if (next_in != m_Tail)
        {
          m_Buff[m_Head] = p_data[i];
          m_Head = next_in;
        }
        else
        {
          ret = false;
          break;
        }
      }
      return ret;
    }

    inline bool Read(T *p_data, uint8_t length)
    {
      bool ret = true;
      for (uint8_t i = 0; i < length; i++)
      {
        p_data[i] = m_Buff[m_Tail];
        if (m_Tail != m_Head)
        {
          m_Tail = (m_Tail + 1) % AP_UTIL_QUE_BUFF_SIZE;
        }
        else
        {
          ret = false;
          break;
        }
      }
      return ret;
    }

    inline uint8_t Available()
    {
      return (m_Head - m_Tail) % AP_UTIL_QUE_BUFF_SIZE;
    }

    inline bool Put(T data)
    {
      bool ret = true;
      uint32_t next_in;
      next_in = (m_Head + 1) % AP_UTIL_QUE_BUFF_SIZE;
      if (next_in != m_Tail)
      {
        m_Buff[m_Head] = data;
        m_Head = next_in;
      }
      else
        ret = false;

      return ret;
    }

    inline bool Get(T *p_data)
    {
      bool ret = true;
      *(p_data) = m_Buff[m_Tail];
      if (m_Tail != m_Head)
      {
        m_Tail = (m_Tail + 1) % AP_UTIL_QUE_BUFF_SIZE;
      }
      else
        ret = false;

      return ret;
    }

    inline T Pop(uint32_t addr)
    {
      if (addr > (AP_UTIL_QUE_BUFF_SIZE - 1))
        return m_Buff[m_Tail];
      return m_Buff[addr];
    }

    inline void Pop(uint32_t addr, T *p_data)
    {
      if (addr > (AP_UTIL_QUE_BUFF_SIZE - 1))
        return;
      *(p_data) = m_Buff[addr];
    }

    inline uint32_t GetSize()
    {
      return ((m_Head - m_Tail) % AP_UTIL_QUE_BUFF_SIZE);
    }

    inline T operator[](uint32_t addr)
    {
      return Pop[addr];
    }

    inline void Flush()
    {
      m_Head = m_Tail = 0;
    }
  };
}

namespace conv
{
  int DwToInt(uint8_t *bytes);
  uint32_t DwToUint(uint8_t *bytes);
}

#define DEF_FLAG_BANK_MAX  8
enum class bit_e
{
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
#define REG_BIT(x) static_cast<bit_e>(x)

/// <summary>
/// 비트, 바이트 단위 플래그 데이터 형
/// </summary>
class fdata
{

  struct _flag
  {
    bool m_bit[static_cast<uint8_t>(bit_e::_max)]{0};
    explicit _flag(const _flag &ref) : m_bit() {}
    _flag &operator=(const _flag &ref) { return *this; }

  public:
    explicit _flag() : m_bit(){};
    virtual ~_flag(){};

    inline bool &operator[](const bit_e bit_no)
    {
      return m_bit[static_cast<int>(bit_no)];
    }

    inline void operator=(const uint8_t in_data)
    {
      uint8_t data = 0;
      for (uint8_t i = 0; i < 8; i++)
      {
        data = in_data;
        data = data >> i;
        data = 0xfe | data;
        data = ~(data);
        m_bit[i] = !data; //!(~(0xfe | (data >> i)));
      }
    }

    inline uint8_t get()
    {
      uint8_t data;
      data = (int)m_bit[0] | ((int)m_bit[1] * 2) | ((int)m_bit[2] * 4) | ((int)m_bit[3] * 8) | ((int)m_bit[4] * 16) | ((int)m_bit[5] * 32) | ((int)m_bit[6] * 64 | ((int)m_bit[7] * 128));
      return data;
    }

    inline uint8_t on_bit(const bit_e bit_no)
    {
      m_bit[static_cast<int>(bit_no)] = true;
      return get();
    }

    inline uint8_t off_bit(const bit_e bit_no)
    {
      m_bit[static_cast<int>(bit_no)] = false;
      return get();
    }
  };

public:
  _flag m_byte[DEF_FLAG_BANK_MAX];
  uint8_t m_size;

  explicit fdata(const fdata &ref) : m_byte(), m_size() {}
  fdata &operator=(const fdata ref) { return *this; }

public:
  fdata(uint8_t size = DEF_FLAG_BANK_MAX) : m_byte(), m_size(size){};
  virtual ~fdata(){};

public:
  inline void ClearAll()
  {
    for (uint8_t i = 0; i < m_size; i++)
    {
      Clear(i);
    }
  }

  inline void Clear(uint8_t bank)
  {
    m_byte[bank] = 0;
  }

  inline uint8_t SetOn(uint8_t bank, bit_e bit_no)
  {
    m_byte[bank][bit_no] = true;
    return m_byte[bank].get();
  }
  inline uint8_t SetOff(uint8_t bank, bit_e bit_no)
  {
    m_byte[bank][bit_no] = false;
    return m_byte[bank].get();
  }

  inline bool Get(uint8_t bank, bit_e bit)
  {
    return m_byte[bank][bit];
  }

  inline uint8_t GetSize() const
  {
    return m_size;
  }

  inline _flag &operator[](uint8_t bank)
  {
    return m_byte[bank];
  }

  inline uint8_t operator[](int bit)
  {
    return m_byte[bit].get();
  }
};
