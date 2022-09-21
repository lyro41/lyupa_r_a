#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <bitset/bitset.h>
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

TEST_CASE("testing Bitset") {
  CHECK_THROWS_WITH_AS(Bitset(-3), 
                       Bitset::NegativeSizeError().what(),
                       Bitset::NegativeSizeError);
  CHECK_THROWS_WITH_AS(Bitset(0) &= Bitset(1),
                       Bitset::SizesMatchingError().what(),
                       Bitset::SizesMatchingError);
  CHECK_THROWS_WITH_AS(Bitset(0) |= Bitset(1),
                       Bitset::SizesMatchingError().what(),
                       Bitset::SizesMatchingError);
  CHECK_THROWS_WITH_AS(Bitset(0) ^= Bitset(1),
                       Bitset::SizesMatchingError().what(),
                       Bitset::SizesMatchingError);
  CHECK_THROWS_WITH_AS(Bitset(3)[-1], 
                       Bitset::IndexingError().what(),
                       Bitset::IndexingError);
  CHECK_THROWS_WITH_AS(Bitset(3)[3], 
                       Bitset::IndexingError().what(),
                       Bitset::IndexingError);

  Bitset b(24, 0);
  {
    Bitset a(24, 1);
    b = a;
  }

  CHECK(b == Bitset(24, 1));
  {
    b = b;
  }
  CHECK(b == Bitset(24, 1));

  Bitset a(24, 1);
  std::stringstream sstream("");
  sstream << a;
  CHECK(sstream.str() == std::string("111111111111111111111111"));

  a >>= 5;
  Bitset d("000001111111111111111111");
  CHECK(a == d);

  CHECK(~a == Bitset("111110000000000000000000"));
  a <<= 2;
  CHECK(~a == Bitset("111000000000000000000011"));

  Bitset c(13, 0);
  CHECK(~c == Bitset("1111111111111"));

  bool bool_buff = a[22];
  CHECK_FALSE(bool_buff);
  a[22] = true;
  CHECK(a == Bitset("010111111111111111111100"));

  bool_buff = a[17];
  CHECK(bool_buff);
  a[17] = false;
  CHECK(a == Bitset("010111011111111111111100"));

  a.resize(13, 0);
  CHECK(a == Bitset("1111111111100"));

  a.resize(17, 1);
  CHECK(a == Bitset("11111111111111100"));

  a.resize(19);
  CHECK(a == Bitset("0011111111111111100"));

  const Bitset con(17, 1);
  bool flag = con[2] == true;
  CHECK(flag); 

  sstream.clear();
  sstream.str("0101010");
  Bitset input;
  sstream >> input;
  CHECK(input == Bitset("0101010"));
  CHECK(sstream.rdstate() == std::ios_base::eofbit);

  sstream.clear();
  sstream.str("");
  sstream >> input;
  CHECK(input == Bitset("0101010"));
  CHECK(sstream.rdstate() == (std::ios_base::failbit | std::ios_base::eofbit));

  sstream.clear();
  sstream.str("a");
  sstream >> input;
  CHECK(input == Bitset("0101010"));
  CHECK(sstream.rdstate() == std::ios_base::failbit);

  sstream.clear();
  sstream.str("0a");
  sstream >> input;
  CHECK(input == Bitset("0"));
  CHECK(sstream.rdstate() == std::ios_base::goodbit);
}
