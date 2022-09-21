#define VARIANT 2
#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>

template<class Type>
class Set {
 public:
  Set() = default;

  Set(const Set<Type>& copy) = default;

  ~Set() = default;


  Set<Type>& operator=(const Set<Type>& rhs) = default;

  Set<Type>& operator+=(const Type& rhs) {
    for (auto it : data_) {
      if (it == rhs) return *this;
    }
    data_.push_back(rhs);
    std::stable_sort(data_.begin(), data_.end());
    return *this;
  }

  Set<Type>& operator-=(const Type& rhs) {
    for (int64_t i = 0; i < data_.size(); ++i) {
      if (data_[i] == rhs) {
        for (int64_t j = i + 1; j < data_.size(); ++j) {
          data_[j - 1] = data_[j];
        }
        data_.resize(data_.size() - 1);
        break;
      }
    }
    return *this;
  }

  Set<Type>& operator-=(const Set<Type>& rhs) {
    for (auto it_rhs : rhs.data_) {
      *this -= it_rhs;
    }
    return *this;
  }
  
  Set<Type>& operator&=(const Set<Type>& rhs) {
    Set<Type> lhs = *this;
    this->data_.resize(0);
    for (auto it_lhs : lhs.data_) {
      for (auto it_rhs : rhs.data_) {
        if (it_lhs == it_rhs) {
          *this += it_lhs;
          break;
        }
      }
    }
    return *this;
  }

  Set<Type>& operator|=(const Set<Type>& rhs) {
    for (auto it_rhs : rhs.data_) {
      *this += it_rhs;
    }
    return *this;
  }

  Set<Type>& operator^=(const Set<Type>& rhs) {
    *this = (rhs | *this) - (rhs & *this);
    return *this;
  }


  Set<Type> operator+(const Type& rhs) const {
    Set<Type> lhs = *this;
    return lhs += rhs;
  }

  Set<Type> operator-(const Type& rhs) const {
    Set<Type> lhs = *this;
    return lhs -= rhs;
  }

  Set<Type> operator-(const Set<Type>& rhs) const {
    Set<Type> lhs = *this;
    return lhs -= rhs;
  }

  Set<Type> operator&(const Set<Type>& rhs) const {
    Set<Type> lhs = *this;
    return lhs &= rhs;
  }

  Set<Type> operator|(const Set<Type>& rhs) const {
    Set<Type> lhs = *this;
    return lhs |= rhs;
  }

  Set<Type> operator^(const Set<Type>& rhs) const {
    Set<Type> lhs = *this;
    return lhs ^= rhs;
  }

  void Clear() {
    data_.resize(0);
    return;
  }

  std::ostream& Print(std::ostream& ostream) const {
    ostream << kOpenBracket << ' ';
    for (auto it : data_) {
      ostream << it << ' ';
    }
    ostream << kCloseBracket << '\n';
    return ostream;
  }

 private:
  static const char kOpenBracket{ '{' };
  static const char kCloseBracket{ '}' };
  std::vector<Type> data_;
};

int main() {
  Set<int> a;
  (a + 7).Print(std::cout);
  a += 7;
  a.Print(std::cout);
  (a + 3).Print(std::cout);
  a += 3;
  a.Print(std::cout);
  (a + 3).Print(std::cout);
  a += 3;
  a.Print(std::cout);
  (a - 3).Print(std::cout);
  a -= 3;
  a.Print(std::cout);
  (a - 3).Print(std::cout);
  a -= 3;
  a.Print(std::cout);

  std::cout << "\n\n";

  Set<int> c;
  (((c += 2) += 1) += 17) += 4;

  Set<int> b;
  ((((b += 7) += 2) += 12) += 5) += 1;

  std::cout << "b: ";
  b.Print(std::cout);
  std::cout << "c: ";
  c.Print(std::cout);

  std::cout << "c - b: ";
  (c - b).Print(std::cout);

  std::cout << "b - c: ";
  (b - c).Print(std::cout);

  std::cout << "c ^ b: ";
  (c ^ b).Print(std::cout);

  std::cout << "b ^ c: ";
  (b ^ c).Print(std::cout);

  {
    Set<int> lhs;
    (((lhs += 1) += 2) += 3) += 7;
    std::cout << "lhs: ";
    lhs.Print(std::cout);

    Set<int> rhs;
    ((rhs += 2) += 7) += 4;
    std::cout << "rhs: ";
    rhs.Print(std::cout);

    Set<int> result = lhs;

    std::cout << "result & rhs: ";
    (result & rhs).Print(std::cout);

    std::cout << "result &= rhs: ";
    (result &= rhs).Print(std::cout);

    std::cout << "result.Clear(): ";
    result.Clear();
    result.Print(std::cout);

    result = lhs;

    std::cout << "result | rhs: ";
    (result | rhs).Print(std::cout);
    
    std::cout << "result |= rhs: ";
    (result |= rhs).Print(std::cout);
  }
  return 0;
}
