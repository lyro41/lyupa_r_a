#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <rational/rational.h>
#include <sstream>
#include <vector>

void RationalEqual(const Rational& rational, 
                   int64_t numerator, int64_t denominator) {
  REQUIRE(rational.GetNumerator() == numerator);
  REQUIRE(rational.GetDenominator() == denominator);
}

TEST_CASE("testing rational constructor") {
  RationalEqual(Rational(7), 7, 1);
  RationalEqual(Rational(7, -1), -7, 1);
  RationalEqual(Rational(-7), -7, 1);
  RationalEqual(Rational(24, 8), 3, 1);
  RationalEqual(Rational(24, -8), -3, 1);
  RationalEqual(Rational(-32, -10), 16, 5);
  RationalEqual(Rational(0, 7), 0, 1);
  CHECK_THROWS_WITH_AS(Rational(27, 0), 
                       Rational::DivisionByZeroError().what(), 
                       Rational::DivisionByZeroError);
}

TEST_CASE("testing rational operators") {
  Rational a{ 6, 21 };
  Rational b = -1;
  Rational c(2, -4);

  a += b;
  b -= c;
  c *= a;
  b /= a;

  RationalEqual(+a, -5, 7);
  RationalEqual(-b, -7, 10);
  RationalEqual(c, 5, 14);

  RationalEqual(a + c, -5, 14);
  RationalEqual(a - b, -99, 70);
  RationalEqual(b * c, 1, 4);
  RationalEqual(c / a, -1, 2);

  RationalEqual(++b, 17, 10);
  RationalEqual(b++, 17, 10);
  RationalEqual(b--, 27, 10);
  RationalEqual(--b, 7, 10);

  REQUIRE(a < b);
  REQUIRE(b > c);
  REQUIRE_FALSE(a == c);
}

TEST_CASE("testing rational IO") {
  std::vector<std::stringstream *> correct_inputs {
    new std::stringstream("-7/3"),
    new std::stringstream("-7/3 "),
    new std::stringstream("4/6"),
    new std::stringstream("-0/3"),
    new std::stringstream("7/0") 
  };

  std::vector<Rational> correct_rationals {
    Rational(-7, 3),
    Rational(-7, 3),
    Rational(2, 3),
    Rational(0, 1)
  };

  std::vector<std::string> correct_outputs {
    std::string("-7/3"),
    std::string("-7/3"),
    std::string("2/3"),
    std::string("0/1")
  };

  std::vector<std::ios_base::iostate> correct_states {
    std::ios_base::eofbit,
    std::ios_base::goodbit,
    std::ios_base::eofbit,
    std::ios_base::eofbit
  };

  std::stringstream output("");

  {
    int64_t i = 0;
    Rational input;
    
    for (; i < correct_rationals.size(); ++i) {
      *(correct_inputs[i]) >> input;
      CHECK(input == correct_rationals[i]);
      CHECK((*(correct_inputs[i])).rdstate() == correct_states[i]);
      output << input;
      CHECK(output.str() == correct_outputs[i]);
      output.str("");
    }

    for (; i < correct_inputs.size(); ++i) {
      CHECK_THROWS_WITH_AS(*(correct_inputs[i]) >> input, 
                           Rational::DivisionByZeroError().what(), 
                           Rational::DivisionByZeroError);
    }
  }

  std::vector<std::stringstream *> incorrect_inputs {
    new std::stringstream("-7 /3"),
    new std::stringstream("4/ 6"),
    new std::stringstream("1/-3"),
    new std::stringstream("1 /-3"),
    new std::stringstream("7 / 0"),
    new std::stringstream("7 / -0") 
  };

  for (auto it : incorrect_inputs) {
    Rational buff;
    *it >> buff;
    CHECK(!(*it).good());
  }

  for (auto it : correct_inputs) {
    delete it;
  }

  for (auto it : incorrect_inputs) {
    delete it;
  }
}
