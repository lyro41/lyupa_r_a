#include <complex/complex.h>

#include <iostream>
#include <stdexcept>


Complex::Complex(double real, double imaginary) 
  : re(real), im(imaginary) {
  return;
}


Complex& Complex::operator+=(const Complex& rhs) {
  re += rhs.re;
  im += rhs.im;
  return (*this);
}

Complex& Complex::operator-=(const Complex& rhs) {
  return *this += -rhs;
}

Complex& Complex::operator*=(const Complex& rhs) {
  Complex lhs = *this;
  re = lhs.re * rhs.re - lhs.im * rhs.im;
  im = lhs.re * rhs.im + lhs.im * rhs.re;
  return (*this);
}

Complex& Complex::operator/=(const Complex& rhs) {
  if (rhs == Complex{ 0.0, 0.0 }) {
    throw std::invalid_argument("division by zero");
  }
  Complex lhs = *this;
  re = (lhs.re * rhs.re + lhs.im * rhs.im) / 
             (rhs.re * rhs.re + rhs.im * rhs.im);
  im = (lhs.im * rhs.re - lhs.re * rhs.im) /
             (rhs.re * rhs.re + rhs.im * rhs.im);
  return (*this);
}


bool Complex::operator==(const Complex& rhs) const {
  return std::abs(re - rhs.re) < k2Eps && 
         std::abs(im - rhs.im) < k2Eps;
}

bool Complex::operator!=(const Complex& rhs) const {
  return !(*this == rhs);
}


Complex Complex::operator-() const {
  return Complex{ -re, -im };
}

Complex Complex::operator+() const {
  return (*this);
}


Complex operator+(const Complex& lhs, const Complex& rhs) {
  Complex lhs_copy = lhs;
  return lhs_copy += rhs; 
}

Complex operator-(const Complex& lhs, const Complex& rhs) {
  Complex lhs_copy = lhs;
  return lhs_copy -= rhs; 
}

Complex operator*(const Complex& lhs, const Complex& rhs) {
  Complex lhs_copy = lhs;
  return lhs_copy *= rhs; 
}

Complex operator/(const Complex& lhs, const Complex& rhs) {
  Complex lhs_copy = lhs;
  return lhs_copy /= rhs; 
}


std::ostream& operator<<(std::ostream& ostream, const Complex& data) {
  ostream << data.kLeftBracket << data.re << data.kSeparator << 
             data.im << data.kRightBracket;
  return ostream;
}

std::istream& operator>>(std::istream& istream, Complex& data) {
  char left_bracket(0);
  double real(0.0);
  char separator(0);
  double imaginary(0.0);
  char right_bracket(0);
  istream >> left_bracket >> real >> separator >> imaginary >> right_bracket;
  if (istream.good()) {
    if (left_bracket == Complex::kLeftBracket && 
        separator == Complex::kSeparator &&
        right_bracket == Complex::kRightBracket) {
        data.re = real;
        data.im = imaginary;
    } else {
      istream.setstate(std::ios_base::failbit);
    }
  }
  return istream;
}
