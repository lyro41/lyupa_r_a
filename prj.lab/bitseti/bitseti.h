#ifndef BITSETI
#define BITSETI

#include <iosfwd>
#include <cstdint>

class BitsetI {
 public:
  enum class IOFormat { 
    txt,
    bin
  };
  
  enum class BSType {
    std,
    t,
    d
  };

  enum class TType {
    u8,
    u16,
    u32,
    u64
  };
  
  virtual std::ostream& WriteTo(std::ostream &ostream) = delete;
  virtual std::istream& ReadFrom(std::istream &istream) = delete;
  virtual int64_t size() = delete;
  virtual void resize() = delete;
};

#endif // BITSETI
