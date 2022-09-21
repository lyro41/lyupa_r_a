#ifndef RATIONAL
#define RATIONAL

#include <cstdint>
#include <iosfwd>
#include <stdexcept>
#include <string>

class Rational {
 public:
  class DivisionByZeroError : public std::runtime_error {
   public:
    DivisionByZeroError(std::string message = "error: division by zero")
      : std::runtime_error(message) {}
  };

  Rational(int64_t numerator = 0, int64_t denominator = 1);
  Rational(const Rational& copy) = default;

  Rational& operator=(const Rational& rhs) = default;
  Rational& operator+=(const Rational& rhs);
  Rational& operator-=(const Rational& rhs);
  Rational& operator*=(const Rational& rhs);
  Rational& operator/=(const Rational& rhs);

  ~Rational() = default;

  bool operator==(const Rational& rhs) const;
  bool operator!=(const Rational& rhs) const;
  bool operator<(const Rational& rhs) const;
  bool operator>(const Rational& rhs) const;
  bool operator<=(const Rational& rhs) const;
  bool operator>=(const Rational& rhs) const;

  Rational operator-() const;
  Rational operator+() const;
  
  Rational& operator++();
  Rational& operator--();

  Rational operator++(int);
  Rational operator--(int);

  Rational operator+(const Rational& rhs) const;
  Rational operator-(const Rational& rhs) const;
  Rational operator/(const Rational& rhs) const;
  Rational operator*(const Rational& rhs) const;

  friend std::ostream& operator<<(std::ostream& ostream, const Rational& data);
  friend std::istream& operator>>(std::istream& istream, Rational& data);

  void Normalize();

  void SetValues(int64_t numerator, int64_t denominator);

  int64_t GetNumerator() const { return numerator_; }
  int64_t GetDenominator() const { return denominator_; }

  static int64_t Gcd(int64_t a, int64_t b);
  static int64_t Lcm(int64_t a, int64_t b);

 private:
  static const char kSeparator{ '/' };

  int64_t numerator_{ 0 };
  int64_t denominator_{ 1 };
};

#endif // RATIONAL
