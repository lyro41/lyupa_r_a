#ifndef BITSETT
#define BITSETT

#include <iostream>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <type_traits>

template<class ClusterType>
class BitsetT {
 public:
  static_assert(std::is_unsigned<ClusterType>::value, "BitsetT type must be unsigned arithmetic type");

  class BitHolder {
   public:
    friend class BitsetT;

    class NegativePositionError : public std::invalid_argument {
     public:
      NegativePositionError(std::string message = "BitHolder error: negative position is undefined")
        : std::invalid_argument(message) {}
    };

    BitHolder& operator=(const bool rhs);

    ~BitHolder() = default;

    operator bool() const;

   private:
    explicit BitHolder(BitsetT* bitset = nullptr, 
                       const int64_t position = 0);
    BitHolder(const BitHolder& copy) = default;

    BitsetT* bitset_{ nullptr };
    int64_t position_{ 0 };
  };

  class NegativeSizeError : public std::invalid_argument {
   public:
    NegativeSizeError(std::string message = "BitsetT error: BitsetT size can not be negative")
      : std::invalid_argument(message) {}
  };

  class IndexingError: public std::out_of_range {
   public:
    IndexingError(std::string message = "BitsetT error: index out of range")
      : std::out_of_range(message) {}
  };

  class NegativeBitwiseShiftError : public std::invalid_argument {
   public:
    NegativeBitwiseShiftError(std::string message = "BitsetT error: bitwise shift by negative count is undefined") 
      : std::invalid_argument(message) {}
  };

  class SizesMatchingError : public std::logic_error {
   public:
    SizesMatchingError(std::string message = "BitsetT error: sizes of operands do not match")
      : std::logic_error(message) {}
  };
 
  explicit BitsetT(const int64_t size = 0, const bool filler = false);
  BitsetT(const std::string& normal_form);
  BitsetT(const BitsetT& copy);
 
  BitsetT& operator=(const BitsetT& rhs);
 
  BitsetT& operator&=(const BitsetT& rhs);
  BitsetT& operator|=(const BitsetT& rhs);
  BitsetT& operator^=(const BitsetT& rhs);
  BitsetT& operator<<=(const int64_t rhs);
  BitsetT& operator>>=(const int64_t rhs);
 
  ~BitsetT();
 
  bool operator==(const BitsetT& rhs) const;
  bool operator!=(const BitsetT& rhs) const;
 
  BitsetT operator~() const;
 
  bool operator[](const int64_t position) const;
  BitHolder operator[](const int64_t position);
 
  int64_t size() const;
  void resize(const int64_t size, const bool filler = false);

  std::ostream& WriteTo(std::ostream& ostream) const;
  std::istream& ReadFrom(std::istream& istream);

 private:
  static const int64_t kByteSize{ 8 };
  static constexpr int64_t kClusterBitSize{ sizeof(ClusterType) * kByteSize };

  void Swap(BitsetT& other) noexcept;

  ClusterType* data_{ nullptr };
  int64_t size_{ 0 };
  int64_t clusters_count_{ 0 };
};

template<class ClusterType>
BitsetT<ClusterType> operator&(const BitsetT<ClusterType>& lhs, 
                               const BitsetT<ClusterType>& rhs);

template<class ClusterType>
BitsetT<ClusterType> operator|(const BitsetT<ClusterType>& lhs, 
                               const BitsetT<ClusterType>& rhs);

template<class ClusterType>
BitsetT<ClusterType> operator^(const BitsetT<ClusterType>& lhs, 
                               const BitsetT<ClusterType>& rhs);

template<class ClusterType>
BitsetT<ClusterType> operator<<(const BitsetT<ClusterType>& lhs, 
                               const BitsetT<ClusterType>& rhs);

template<class ClusterType>
BitsetT<ClusterType> operator>>(const BitsetT<ClusterType>& lhs, 
                               const BitsetT<ClusterType>& rhs);

template<class ClusterType>
std::ostream& operator<<(std::ostream& ostream, 
                         const BitsetT<ClusterType>& data);

template<class ClusterType>
std::istream& operator>>(std::istream& istream, BitsetT<ClusterType>& data);





template<class ClusterType>
BitsetT<ClusterType>::BitHolder::BitHolder(BitsetT<ClusterType>* bitset, 
                                           const int64_t position)
  : bitset_(bitset),
    position_(position) {
  if (position_ < 0) {
    throw NegativePositionError();
  }
  return;
}


template<class ClusterType>
typename BitsetT<ClusterType>::BitHolder& 
BitsetT<ClusterType>::BitHolder::operator=(const bool rhs) {
  ClusterType& cluster_ptr = (bitset_->data_)[position_ / kClusterBitSize];
  if (rhs) {
    cluster_ptr |= 1 << (position_ % kClusterBitSize);
  } else {
    cluster_ptr &= ~(1 << (position_ % kClusterBitSize));
  }
  return *this;
}


template<class ClusterType>
BitsetT<ClusterType>::BitHolder::operator bool() const {
  return ((bitset_->data_)[position_ / kClusterBitSize] >>
          (position_ % kClusterBitSize)) & 1;
}


template<class ClusterType>
BitsetT<ClusterType>::BitsetT(const int64_t size, const bool filler) 
  : size_(size),
    clusters_count_(size == 0 ? 0 : (size_ - 1) / kClusterBitSize + 1) {
  if (size_ < 0) {
    throw NegativeSizeError();
  }
  if (size_ == 0) {
    data_ = nullptr;
    return;
  }
  
  data_ = new ClusterType[clusters_count_];

  for (int64_t i = 0; i < clusters_count_; ++i) {
    data_[i] = 0;
  }

  if (filler) {
    for (int64_t i = 0; i < size_; ++i) {
      data_[i / kClusterBitSize] |= filler << (i % kClusterBitSize);
    }
  }
  return;
}

template<class ClusterType>
BitsetT<ClusterType>::BitsetT(const std::string& normal_form) 
  : BitsetT<ClusterType>(normal_form.size()) {
  for (int64_t i = 0; i < size_; ++i) {
    if (normal_form[size_ - i - 1] != '0' &&
        normal_form[size_ - i - 1] != '1') {
      throw std::invalid_argument("BitsetT error: BitsetT must consist of 0 and 1 only");
    }
    data_[i / kClusterBitSize] |= (normal_form[size_ - i - 1] - '0') << 
                                  (i % kClusterBitSize);
  }
  return;
}

template<class ClusterType>
BitsetT<ClusterType>::BitsetT(const BitsetT<ClusterType>& copy) 
  : BitsetT<ClusterType>(copy.size_) {
  for (int64_t i = 0; i < clusters_count_; ++i) {
    data_[i] = (copy.data_)[i];
  }
  return;
}


template<class ClusterType>
BitsetT<ClusterType>& 
BitsetT<ClusterType>::operator=(const BitsetT<ClusterType>& rhs) {
  BitsetT<ClusterType> tmp(rhs);
  this->Swap(tmp);
  return *this;
}


template<class ClusterType>
BitsetT<ClusterType>& 
BitsetT<ClusterType>::operator&=(const BitsetT<ClusterType>& rhs) {
  if (size_ != rhs.size_) {
    throw SizesMatchingError();
  }
  for (int64_t i = 0; i < clusters_count_; ++i) {
    data_[i] &= (rhs.data_)[i];
  }
  return *this;
}

template<class ClusterType>
BitsetT<ClusterType>& 
BitsetT<ClusterType>::operator|=(const BitsetT<ClusterType>& rhs) {
  if (size_ != rhs.size_) {
    throw SizesMatchingError();
  }
  for (int64_t i = 0; i < clusters_count_; ++i) {
    data_[i] |= (rhs.data_)[i];
  }
  return *this;
}

template<class ClusterType>
BitsetT<ClusterType>& 
BitsetT<ClusterType>::operator^=(const BitsetT<ClusterType>& rhs) {
  if (size_ != rhs.size_) {
    throw SizesMatchingError();
  }
  for (int64_t i = 0; i < clusters_count_; ++i) {
    data_[i] ^= (rhs.data_)[i];
  }
  return *this;
}

template<class ClusterType>
BitsetT<ClusterType>& 
BitsetT<ClusterType>::operator<<=(const int64_t rhs) {
  if (rhs < 0) {
    throw NegativeBitwiseShiftError();
  }
  if (size_ == 0) {
    return *this;
  } 
  if (rhs >= size_) {
    *this = BitsetT<ClusterType>(size_);
    return *this;
  }
  int64_t clusters_delta = rhs / kClusterBitSize;
  int64_t bit_delta = rhs % kClusterBitSize;
  {
    int64_t i = clusters_count_ - 1;
    for (; i > clusters_delta; --i) {
      data_[i] = static_cast<ClusterType>(data_[i - clusters_delta] << 
                                          bit_delta);
      data_[i] |= data_[i - clusters_delta - 1] >> 
                  (kClusterBitSize - bit_delta);
    }
    data_[i] = data_[i - clusters_delta] << bit_delta;
    --i;
    for (; i >= 0; --i) {
      data_[i] = 0;
    }
  }
  ClusterType mask = 0;
  mask = (~mask);
  mask >>= (kClusterBitSize - size_ % kClusterBitSize) % kClusterBitSize;
  data_[clusters_count_ - 1] &= mask;
  return *this;
}

template<class ClusterType>
BitsetT<ClusterType>& 
BitsetT<ClusterType>::operator>>=(const int64_t rhs) {
  if (rhs < 0) {
    throw NegativeBitwiseShiftError();
  }
  if (size_ == 0) {
    return *this;
  } 
  if (rhs >= size_) {
    *this = BitsetT<ClusterType>(size_);
    return *this;
  }
  int64_t clusters_delta = rhs / kClusterBitSize;
  int64_t bit_delta = rhs % kClusterBitSize;
  {
    int64_t i = 0;
    for (; i < clusters_count_ - clusters_delta - 1; ++i) {
      data_[i] = data_[i + clusters_delta] >> bit_delta;
      data_[i] |= static_cast<ClusterType>(data_[i + clusters_delta + 1] << 
                                           (kClusterBitSize - bit_delta));
    }
    data_[i] = data_[i + clusters_delta] >> bit_delta;
    ++i;
    for (; i < clusters_count_; ++i) {
      data_[i] = 0;
    }
  }
  ClusterType mask = 0;
  mask = (~mask);
  mask >>= (kClusterBitSize - size_ % kClusterBitSize) % kClusterBitSize;
  data_[clusters_count_ - 1] &= mask;
  return *this;
}


template<class ClusterType>
BitsetT<ClusterType>::~BitsetT() {
  delete[] data_;
}


template<class ClusterType>
bool 
BitsetT<ClusterType>::operator==(const BitsetT<ClusterType>& rhs) const {
  if (size_ != rhs.size_) {
    return false;
  }
  for (int64_t i = 0; i < clusters_count_; ++i) {
    if (data_[i] != (rhs.data_)[i]) {
      return false;
    }
  }
  return true;
}

template<class ClusterType>
bool 
BitsetT<ClusterType>::operator!=(const BitsetT<ClusterType>& rhs) const {
  return !(*this == rhs);
}


template<class ClusterType>
BitsetT<ClusterType> 
BitsetT<ClusterType>::operator~() const {
  BitsetT<ClusterType> lhs(*this);
  for (int64_t i = 0; i < clusters_count_; ++i) {
    lhs.data_[i] = ~(lhs.data_[i]);
  }
  if (size_ != clusters_count_ * kClusterBitSize) {
    lhs.data_[clusters_count_ - 1] &= static_cast<ClusterType>(~0) >>
                                      (clusters_count_ * kClusterBitSize - 
                                       size_);
  }
  return lhs;
}


template<class ClusterType>
bool BitsetT<ClusterType>::operator[](const int64_t position) const {
  if (position < 0 || position >= size_) {
    throw IndexingError();
  }
  return ((this->data_)[position / kClusterBitSize] >>
          (position % kClusterBitSize)) & 1;
}


template<class ClusterType>
typename BitsetT<ClusterType>::BitHolder 
BitsetT<ClusterType>::operator[](const int64_t position) {
  if (position < 0 || position >= size_) {
    throw IndexingError();
  }
  return BitHolder(this, position);
}


template<class ClusterType>
int64_t 
BitsetT<ClusterType>::size() const {
  return size_;
}

template<class ClusterType>
void 
BitsetT<ClusterType>::resize(const int64_t size, const bool filler) {
  BitsetT<ClusterType> result(size, filler);
  for (int64_t i = 0; i < size && i < size_; ++i) {
    result[i] = static_cast<bool>((*this)[i]);
  }
  this->Swap(result);
  return;
}


template<class ClusterType>
std::ostream& 
BitsetT<ClusterType>::WriteTo(std::ostream& ostream) const {
  for (int64_t i = size_ - 1; i >= 0; --i) {
    ostream << ((data_[i / kClusterBitSize] >> (i % kClusterBitSize)) & 1);
  }
  return ostream;
}

template<class ClusterType>
std::istream& 
BitsetT<ClusterType>::ReadFrom(std::istream& istream) {
  if (!istream.good()) {
    istream.setstate(std::ios_base::failbit);
    return istream;
  }

  std::string buff("");
  char peek = istream.peek();
  while (peek == '0' || peek == '1') {
    buff.push_back(istream.get());
    peek = istream.peek();
  }
  
  if (buff.size() == 0) {
    istream.setstate(std::ios_base::failbit);
    return istream;
  }

  *this = BitsetT<ClusterType>(buff);
  return istream;
}


template<class ClusterType>
void 
BitsetT<ClusterType>::Swap(BitsetT<ClusterType>& other) noexcept {
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  std::swap(clusters_count_, other.clusters_count_);
  return;
}


template<class ClusterType>
BitsetT<ClusterType> 
operator&(const BitsetT<ClusterType>& lhs, const BitsetT<ClusterType>& rhs) {
  BitsetT<ClusterType> lhs_copy(lhs);
  return lhs_copy &= rhs;
}

template<class ClusterType>
BitsetT<ClusterType> 
operator|(const BitsetT<ClusterType>& lhs, const BitsetT<ClusterType>& rhs) {
  BitsetT<ClusterType> lhs_copy(lhs);
  return lhs_copy |= rhs;
}

template<class ClusterType>
BitsetT<ClusterType> 
operator^(const BitsetT<ClusterType>& lhs, const BitsetT<ClusterType>& rhs) {
  BitsetT<ClusterType> lhs_copy(lhs);
  return lhs_copy ^= rhs;
}

template<class ClusterType>
BitsetT<ClusterType> 
operator<<(const BitsetT<ClusterType>& lhs, const int64_t rhs) {
  BitsetT<ClusterType> lhs_copy(lhs);
  return lhs_copy <<= rhs;
}

template<class ClusterType>
BitsetT<ClusterType> 
operator>>(const BitsetT<ClusterType>& lhs, const int64_t rhs) {
  BitsetT<ClusterType> lhs_copy(lhs);
  return lhs_copy >>= rhs;
}


template<class ClusterType>
std::ostream& 
operator<<(std::ostream& ostream, const BitsetT<ClusterType>& data) {
  return data.WriteTo(ostream);
}

template<class ClusterType>
std::istream& 
operator>>(std::istream& istream, BitsetT<ClusterType>& data) {
  return data.ReadFrom(istream);
}


#endif // BITSETT
