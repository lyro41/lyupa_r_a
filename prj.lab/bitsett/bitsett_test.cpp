#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <bitsett/bitsett.h>
#include <iostream>
#include <sstream>
#include <cstdint>


TEST_CASE_TEMPLATE("testing BitsetT", Type, uint8_t, uint16_t, uint32_t, uint64_t) {
  CHECK_THROWS_WITH_AS(BitsetT<Type>(-3), 
                       typename BitsetT<Type>::NegativeSizeError().what(),
                       typename BitsetT<Type>::NegativeSizeError);
  CHECK_THROWS_WITH_AS(BitsetT<Type>(0) &= BitsetT<Type>(1),
                       typename BitsetT<Type>::SizesMatchingError().what(),
                       typename BitsetT<Type>::SizesMatchingError);
  CHECK_THROWS_WITH_AS(BitsetT<Type>(0) |= BitsetT<Type>(1),
                       typename BitsetT<Type>::SizesMatchingError().what(),
                       typename BitsetT<Type>::SizesMatchingError);
  CHECK_THROWS_WITH_AS(BitsetT<Type>(0) ^= BitsetT<Type>(1),
                       typename BitsetT<Type>::SizesMatchingError().what(),
                       typename BitsetT<Type>::SizesMatchingError);
  CHECK_THROWS_WITH_AS(BitsetT<Type>(3)[-1], 
                       typename BitsetT<Type>::IndexingError().what(),
                       typename BitsetT<Type>::IndexingError);
  CHECK_THROWS_WITH_AS(BitsetT<Type>(3)[3], 
                       typename BitsetT<Type>::IndexingError().what(),
                       typename BitsetT<Type>::IndexingError);

  BitsetT<Type> b(24, 0);
  {
    BitsetT<Type> a(24, 1);
    b = a;
  }

  CHECK(b == BitsetT<Type>(24, 1));
  {
    b = b;
  }
  CHECK(b == BitsetT<Type>(24, 1));

  BitsetT<Type> a(24, 1);
  std::stringstream sstream("");
  sstream << a;
  CHECK(sstream.str() == std::string("111111111111111111111111"));

  a >>= 5;
  BitsetT<Type> d("000001111111111111111111");
  CHECK(a == d);

  CHECK(~a == BitsetT<Type>("111110000000000000000000"));
  a <<= 2;
  CHECK(~a == BitsetT<Type>("111000000000000000000011"));

  BitsetT<Type> c(13, 0);
  CHECK(~c == BitsetT<Type>("1111111111111"));

  bool bool_buff = a[22];
  CHECK_FALSE(bool_buff);
  a[22] = true;
  CHECK(a == BitsetT<Type>("010111111111111111111100"));

  bool_buff = a[17];
  CHECK(bool_buff);
  a[17] = false;
  CHECK(a == BitsetT<Type>("010111011111111111111100"));

  a.resize(13, 0);
  CHECK(a == BitsetT<Type>("1111111111100"));

  a.resize(17, 1);
  CHECK(a == BitsetT<Type>("11111111111111100"));

  a.resize(19);
  CHECK(a == BitsetT<Type>("0011111111111111100"));

  const BitsetT<Type> con(17, 1);
  bool flag = con[2] == true;
  CHECK(flag);

  sstream.clear();
  sstream.str("0101010");
  BitsetT<Type> input;
  sstream >> input;
  CHECK(input == BitsetT<Type>("0101010"));
  CHECK(sstream.rdstate() == std::ios_base::eofbit);

  sstream.clear();
  sstream.str("");
  sstream >> input;
  CHECK(input == BitsetT<Type>("0101010"));
  CHECK(sstream.rdstate() == (std::ios_base::failbit | std::ios_base::eofbit));

  sstream.clear();
  sstream.str("a");
  sstream >> input;
  CHECK(input == BitsetT<Type>("0101010"));
  CHECK(sstream.rdstate() == std::ios_base::failbit);

  sstream.clear();
  sstream.str("0a");
  sstream >> input;
  CHECK(input == BitsetT<Type>("0"));
  CHECK(sstream.rdstate() == std::ios_base::goodbit);
}
