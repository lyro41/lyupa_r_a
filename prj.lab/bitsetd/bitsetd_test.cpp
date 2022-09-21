#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <bitsetd/bitsetd.h>
#include <iostream>
#include <sstream>

// int main() {
//   int i = 7;
//   for (i >>= 1; false;);
//   uint32_t val = 0xaabbccdd;
//   uint8_t *ptr = reinterpret_cast<uint8_t *>(&val);
//   bool b = true;
//   uint8_t *bool_ptr = reinterpret_cast<uint8_t *>(&b);
//   std::cout << (*bool_ptr == 0b11111111) << '\n' << 
//                (*bool_ptr == 0b00000001) << '\n';
//   return 0;
// }

TEST_CASE("testing BitsetD") {
  CHECK_THROWS_WITH_AS(BitsetD(-3), 
                       BitsetD::NegativeSizeError().what(),
                       BitsetD::NegativeSizeError);
  CHECK_THROWS_WITH_AS(BitsetD(0) &= BitsetD(1),
                       BitsetD::SizesMatchingError().what(),
                       BitsetD::SizesMatchingError);
  CHECK_THROWS_WITH_AS(BitsetD(0) |= BitsetD(1),
                       BitsetD::SizesMatchingError().what(),
                       BitsetD::SizesMatchingError);
  CHECK_THROWS_WITH_AS(BitsetD(0) ^= BitsetD(1),
                       BitsetD::SizesMatchingError().what(),
                       BitsetD::SizesMatchingError);
  CHECK_THROWS_WITH_AS(BitsetD(3)[-1], 
                       BitsetD::IndexingError().what(),
                       BitsetD::IndexingError);
  CHECK_THROWS_WITH_AS(BitsetD(3)[3], 
                       BitsetD::IndexingError().what(),
                       BitsetD::IndexingError);

  BitsetD b(24, 0);
  {
    BitsetD a(24, 1);
    b = a;
  }

  CHECK(b == BitsetD(24, 1));
  {
    b = b;
  }
  CHECK(b == BitsetD(24, 1));

  BitsetD a(24, 1);
  std::stringstream sstream("");
  sstream << a;
  CHECK(sstream.str() == std::string("111111111111111111111111"));

  a >>= 5;
  BitsetD d("000001111111111111111111");
  CHECK(a == d);

  CHECK(~a == BitsetD("111110000000000000000000"));
  a <<= 2;
  CHECK(~a == BitsetD("111000000000000000000011"));

  BitsetD c(13, 0);
  CHECK(~c == BitsetD("1111111111111"));

  bool bool_buff = a[22];
  CHECK_FALSE(bool_buff);
  a[22] = true;
  CHECK(a == BitsetD("010111111111111111111100"));

  bool_buff = a[17];
  CHECK(bool_buff);
  a[17] = false;
  CHECK(a == BitsetD("010111011111111111111100"));

  a.resize(13, 0);
  CHECK(a == BitsetD("1111111111100"));

  a.resize(17, 1);
  CHECK(a == BitsetD("11111111111111100"));

  a.resize(19);
  CHECK(a == BitsetD("0011111111111111100"));

  const BitsetD con(17, 1);
  bool flag = con[2] == true;
  CHECK(flag);

  sstream.clear();
  sstream.str("0101010");
  BitsetD input;
  sstream >> input;
  CHECK(input == BitsetD("0101010"));
  CHECK(sstream.rdstate() == std::ios_base::eofbit);

  sstream.clear();
  sstream.str("");
  sstream >> input;
  CHECK(input == BitsetD("0101010"));
  CHECK(sstream.rdstate() == (std::ios_base::failbit | std::ios_base::eofbit));

  sstream.clear();
  sstream.str("a");
  sstream >> input;
  CHECK(input == BitsetD("0101010"));
  CHECK(sstream.rdstate() == std::ios_base::failbit);

  sstream.clear();
  sstream.str("0a");
  sstream >> input;
  CHECK(input == BitsetD("0"));
  CHECK(sstream.rdstate() == std::ios_base::goodbit);
}
