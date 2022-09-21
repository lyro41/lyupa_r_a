#ifndef BITSET
#define BITSET

#include <iosfwd>
#include <cstdint>
#include <stdexcept>
#include <string>


class Bitset {
 public:
  typedef uint8_t ClusterType; 

  class BitHolder {
   public:
    friend class Bitset;

    class NegativePositionError : public std::invalid_argument {
     public:
      NegativePositionError(std::string message = "BitHolder error: negative position is undefined")
        : std::invalid_argument(message) {}
    };

    BitHolder& operator=(const bool rhs);

    ~BitHolder() = default;

    operator bool() const;

   private:
    explicit BitHolder(Bitset* bitset = nullptr, 
                       const int64_t position = 0);
    BitHolder(const BitHolder& copy) = default;

    Bitset* bitset_{ nullptr };
    int64_t position_{ 0 };
  };

  class NegativeSizeError : public std::invalid_argument {
   public:
    NegativeSizeError(std::string message = "Bitset error: Bitset size can not be negative")
      : std::invalid_argument(message) {}
  };

  class IndexingError: public std::out_of_range {
   public:
    IndexingError(std::string message = "Bitset error: index out of range")
      : std::out_of_range(message) {}
  };

  class NegativeBitwiseShiftError : public std::invalid_argument {
   public:
    NegativeBitwiseShiftError(std::string message = "Bitset error: bitwise shift by negative count is undefined") 
      : std::invalid_argument(message) {}
  };

  class SizesMatchingError : public std::logic_error {
   public:
    SizesMatchingError(std::string message = "Bitset error: sizes of operands do not match")
      : std::logic_error(message) {}
  };
 
  explicit Bitset(const int64_t size = 0, const bool filler = false);
  Bitset(const std::string& normal_form);
  Bitset(const Bitset& copy);
 
  Bitset& operator=(const Bitset& rhs);
 
  Bitset& operator&=(const Bitset& rhs);
  Bitset& operator|=(const Bitset& rhs);
  Bitset& operator^=(const Bitset& rhs);
  Bitset& operator<<=(const int64_t rhs);
  Bitset& operator>>=(const int64_t rhs);
 
  ~Bitset();
 
  bool operator==(const Bitset& rhs) const;
  bool operator!=(const Bitset& rhs) const;
 
  Bitset operator~() const;
 
  bool operator[](const int64_t position) const;
  BitHolder operator[](const int64_t position);
 
  int64_t size() const;
  void resize(const int64_t size, const bool filler = false);

  std::ostream& WriteTo(std::ostream& ostream) const;
  std::istream& ReadFrom(std::istream& istream);

 private:
  static const int64_t kByteSize{ 8 };
  static constexpr int64_t kClusterBitSize{ sizeof(ClusterType) * kByteSize };

  void Swap(Bitset& other) noexcept;

  ClusterType* data_{ nullptr };
  int64_t size_{ 0 };
  int64_t clusters_count_{ 0 };
};

Bitset operator&(const Bitset& lhs, const Bitset& rhs);
Bitset operator|(const Bitset& lhs, const Bitset& rhs);
Bitset operator^(const Bitset& lhs, const Bitset& rhs);
Bitset operator<<(const Bitset& lhs, const int64_t rhs);
Bitset operator>>(const Bitset& lhs, const int64_t rhs);

std::ostream& operator<<(std::ostream& ostream, const Bitset& data);
std::istream& operator>>(std::istream& istream, Bitset& data);

#endif // BITSET
