#include <rational/rational.h>

#include <iostream>
#include <cctype>


Rational::Rational(int64_t numerator, int64_t denominator) 
  : numerator_(numerator), 
    denominator_(denominator) {
  Normalize();
  return;
}


Rational& Rational::operator+=(const Rational& rhs) {
  int64_t lcm = Lcm(denominator_, rhs.denominator_);

  numerator_ = numerator_ * (lcm / denominator_) +
               rhs.numerator_ * (lcm / rhs.denominator_);
  denominator_ = lcm;
  
  Normalize();
  return *this;
}

Rational& Rational::operator-=(const Rational& rhs) {
  return operator+=(-rhs);
}

Rational& Rational::operator*=(const Rational& rhs) {
  numerator_ *= rhs.numerator_;
  denominator_ *= rhs.denominator_;
  
  Normalize();
  return *this;
}

Rational& Rational::operator/=(const Rational& rhs) {
  Rational inv_rhs{ rhs.denominator_, rhs.numerator_ };
  return *this *= inv_rhs;
}


bool Rational::operator==(const Rational& rhs) const {
  return numerator_ == rhs.numerator_ && 
         denominator_ == rhs.denominator_;
}

bool Rational::operator!=(const Rational& rhs) const {
  return !(*this == rhs);
}

bool Rational::operator<(const Rational& rhs) const {
  int64_t lcm = Lcm(denominator_, rhs.denominator_);
  return numerator_ * (lcm / denominator_) < 
         rhs.numerator_ * (lcm / rhs.denominator_);
}

bool Rational::operator>(const Rational& rhs) const {
  return rhs < *this;
}

bool Rational::operator<=(const Rational& rhs) const {
  return !(*this > rhs);
}

bool Rational::operator>=(const Rational& rhs) const {
  return !(*this < rhs);
}


Rational Rational::operator-() const {
  return Rational{ -numerator_, denominator_ };
}

Rational Rational::operator+() const {
  return *this;
}


Rational& Rational::operator++() {
  numerator_ += denominator_;
  Normalize();
  return *this;
}

Rational& Rational::operator--() {
  numerator_ -= denominator_;
  Normalize();
  return *this;
}


Rational Rational::operator++(int) {
  Rational copy{ *this };
  operator++();
  return copy;
}

Rational Rational::operator--(int) {
  Rational copy{ *this };
  operator--();
  return copy;
}


Rational Rational::operator+(const Rational& rhs) const {
  Rational copy{ *this };
  return copy += rhs;
}

Rational Rational::operator-(const Rational& rhs) const {
  Rational copy{ *this };
  return copy -= rhs;
}

Rational Rational::operator*(const Rational& rhs) const {
  Rational copy{ *this };
  return copy *= rhs;
}

Rational Rational::operator/(const Rational& rhs) const {
  Rational copy{ *this };
  return copy /= rhs;
}


std::ostream& operator<<(std::ostream& ostream, const Rational& data) {
  ostream << data.numerator_ << data.kSeparator << data.denominator_;
  return ostream;
}

std::istream& operator>>(std::istream& istream, Rational& data) {
  int64_t numerator(0);
  char separator(0);
  int64_t denominator(0);

  istream >> numerator;
  istream.get(separator);
  if (!std::isdigit(istream.peek())) {
    istream >> denominator;
    istream.setstate(std::ios_base::failbit);
  } else {
    istream >> denominator;
  }
  if (!istream.bad()) {
    if (separator == Rational::kSeparator) {
      data.SetValues(numerator, denominator);
    } else {
      istream.setstate(std::ios_base::failbit);
    }
  }
  return istream;
}


void Rational::Normalize() {
  if (denominator_ == 0) {
    throw DivisionByZeroError();
  }
  if (denominator_ < 0) {
    numerator_ *= -1;
    denominator_ *= -1;
  }
  int64_t gcd = Gcd(numerator_, denominator_);
  numerator_ /= gcd;
  denominator_ /= gcd;
  return;
}


void Rational::SetValues(int64_t numerator, int64_t denominator) {
  numerator_ = numerator;
  denominator_ = denominator;
  Normalize();
  return;
}


int64_t Rational::Gcd(int64_t a, int64_t b) {
  while (a != 0) {
    b = b % a;
    std::swap(a, b);
  }
  return std::abs(b);
}

int64_t Rational::Lcm(int64_t a, int64_t b) {
  return a / Gcd(a, b) * b;
}
