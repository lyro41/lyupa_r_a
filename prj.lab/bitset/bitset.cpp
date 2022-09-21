#include <bitset/bitset.h>

#include <iostream>


Bitset::BitHolder::BitHolder(Bitset* bitset, const int64_t position)
  : bitset_(bitset),
    position_(position) {
  if (position_ < 0) {
    throw NegativePositionError();
  }
  return;
}


Bitset::BitHolder& Bitset::BitHolder::operator=(const bool rhs) {
  ClusterType& cluster_ptr = (bitset_->data_)[position_ / kClusterBitSize];
  if (rhs) {
    cluster_ptr |= 1 << (position_ % kClusterBitSize);
  } else {
    cluster_ptr &= ~(1 << (position_ % kClusterBitSize));
  }
  return *this;
}


Bitset::BitHolder::operator bool() const {
  return ((bitset_->data_)[position_ / kClusterBitSize] >>
          (position_ % kClusterBitSize)) & 1;
}


Bitset::Bitset(const int64_t size, const bool filler) 
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

Bitset::Bitset(const std::string& normal_form) 
  : Bitset(normal_form.size()) {
  for (int64_t i = 0; i < size_; ++i) {
    if (normal_form[size_ - i - 1] != '0' &&
        normal_form[size_ - i - 1] != '1') {
      throw std::invalid_argument("Bitset error: Bitset must consist of 0 and 1 only");
    }
    data_[i / kClusterBitSize] |= (normal_form[size_ - i - 1] - '0') << 
                                  (i % kClusterBitSize);
  }
  return;
}

Bitset::Bitset(const Bitset& copy) : Bitset(copy.size_) {
  for (int64_t i = 0; i < clusters_count_; ++i) {
    data_[i] = (copy.data_)[i];
  }
  return;
}


Bitset& Bitset::operator=(const Bitset& rhs) {
  Bitset tmp(rhs);
  this->Swap(tmp);
  return *this;
}


Bitset& Bitset::operator&=(const Bitset& rhs) {
  if (size_ != rhs.size_) {
    throw SizesMatchingError();
  }
  for (int64_t i = 0; i < clusters_count_; ++i) {
    data_[i] &= (rhs.data_)[i];
  }
  return *this;
}

Bitset& Bitset::operator|=(const Bitset& rhs) {
  if (size_ != rhs.size_) {
    throw SizesMatchingError();
  }
  for (int64_t i = 0; i < clusters_count_; ++i) {
    data_[i] |= (rhs.data_)[i];
  }
  return *this;
}

Bitset& Bitset::operator^=(const Bitset& rhs) {
  if (size_ != rhs.size_) {
    throw SizesMatchingError();
  }
  for (int64_t i = 0; i < clusters_count_; ++i) {
    data_[i] ^= (rhs.data_)[i];
  }
  return *this;
}

Bitset& Bitset::operator<<=(const int64_t rhs) {
  if (rhs < 0) {
    throw NegativeBitwiseShiftError();
  }
  if (size_ == 0) {
    return *this;
  } 
  if (rhs >= size_) {
    *this = Bitset(size_);
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

Bitset& Bitset::operator>>=(const int64_t rhs) {
  if (rhs < 0) {
    throw NegativeBitwiseShiftError();
  }
  if (size_  == 0) {
    return *this;;
  }
  if (rhs >= size_) {
    *this = Bitset(size_);
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


Bitset::~Bitset() {
  delete[] data_;
}


bool Bitset::operator==(const Bitset& rhs) const {
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

bool Bitset::operator!=(const Bitset& rhs) const {
  return !(*this == rhs);
}


Bitset Bitset::operator~() const {
  Bitset lhs(*this);
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


bool Bitset::operator[](const int64_t position) const {
  if (position < 0 || position >= size_) {
    throw IndexingError();
  }
  return ((this->data_)[position / kClusterBitSize] >>
          (position % kClusterBitSize)) & 1;
}

Bitset::BitHolder Bitset::operator[](const int64_t position) {
  if (position < 0 || position >= size_) {
    throw IndexingError();
  }
  return BitHolder(this, position);
}


int64_t Bitset::size() const {
  return size_;
}

void Bitset::resize(const int64_t size, const bool filler) {
  Bitset result(size, filler);
  for (int64_t i = 0; i < size && i < size_; ++i) {
    result[i] = static_cast<bool>((*this)[i]);
  }
  this->Swap(result);
  return;
}


std::ostream& Bitset::WriteTo(std::ostream& ostream) const {
  for (int64_t i = size_ - 1; i >= 0; --i) {
    ostream << ((data_[i / kClusterBitSize] >> (i % kClusterBitSize)) & 1);
  }
  return ostream;
}

std::istream& Bitset::ReadFrom(std::istream& istream) {
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

  *this = Bitset(buff);
  return istream;
}


void Bitset::Swap(Bitset& other) noexcept {
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  std::swap(clusters_count_, other.clusters_count_);
  return;
}


Bitset operator&(const Bitset& lhs, const Bitset& rhs) {
  Bitset lhs_copy(lhs);
  return lhs_copy &= rhs;
}

Bitset operator|(const Bitset& lhs, const Bitset& rhs) {
  Bitset lhs_copy(lhs);
  return lhs_copy |= rhs;
}

Bitset operator^(const Bitset& lhs, const Bitset& rhs) {
  Bitset lhs_copy(lhs);
  return lhs_copy ^= rhs;
}

Bitset operator<<(const Bitset& lhs, const int64_t rhs) {
  Bitset lhs_copy(lhs);
  return lhs_copy <<= rhs;
}

Bitset operator>>(const Bitset& lhs, const int64_t rhs) {
  Bitset lhs_copy(lhs);
  return lhs_copy >>= rhs;
}


std::ostream& operator<<(std::ostream& ostream, const Bitset& data) {
  return data.WriteTo(ostream);
}

std::istream& operator>>(std::istream& istream, Bitset& data) {
  return data.ReadFrom(istream);
}
