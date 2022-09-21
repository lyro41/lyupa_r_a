#ifndef COMPLEX
#define COMPLEX

#include <iosfwd>
#include <limits>

struct Complex {
 public:
  Complex(double real = 0.0, double imaginary = 0.0);
  Complex(const Complex& copy) = default;

  Complex& operator=(const Complex& rhs) = default;
  Complex& operator+=(const Complex& rhs);
  Complex& operator-=(const Complex& rhs);
  Complex& operator*=(const Complex& rhs);
  Complex& operator/=(const Complex& rhs);

  ~Complex() = default;

  bool operator==(const Complex& rhs) const;
  bool operator!=(const Complex& rhs) const;

  Complex operator-() const;
  Complex operator+() const;

  friend std::ostream& operator<<(std::ostream& ostream, const Complex& data);
  friend std::istream& operator>>(std::istream& istream, Complex& data);

  double re{ 0.0 };
  double im{ 0.0 };

  static constexpr double k2Eps{ 2.0 * std::numeric_limits<double>::epsilon() };

  static const char kLeftBracket{ '{' };
  static const char kSeparator{ ',' };
  static const char kRightBracket{ '}' };
};

Complex operator+(const Complex& lhs, const Complex& rhs);
Complex operator-(const Complex& lhs, const Complex& rhs);
Complex operator*(const Complex& lhs, const Complex& rhs);
Complex operator/(const Complex& lhs, const Complex& rhs);

#endif // COMPLEX
