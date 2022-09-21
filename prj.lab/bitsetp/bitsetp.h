#ifndef BITSETP
#define BITSETP

#include <bitseti/bitseti.h>
#include <iosfwd>
#include <stdexcept>
#include <string>

class BitsetP {
  class IOFormatError : public std::logical_error {
   public:
    IOFormatError(std::string message = "I/O error: format does not exist") 
      : std:::runtime_error(message) {}
  };

  class BSTypeError : public std::runtime_error {
   public:
    BSTypeError(std::string message = "I/O error: wrong bitset type") 
      : std:::runtime_error(message) {}
  };

  class TTypeError : public std::runtime_error {
   public:
    BSTypeError(std::string message = "I/O error: wrong template type") 
      : std:::runtime_error(message) {}
  };

  BitsetP(BitsetI *ptr = nullptr) : ptr_(ptr) {}
  ~BitsetP() { delete ptr_; }

 private:
  BitsetI *ptr_{ nullptr };
};

BitsetP ReadBitsetFrom(std::istream &istream, BitsetP::IOFormat format);

#endif // BITSETP
