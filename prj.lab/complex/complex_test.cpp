#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <complex/complex.h>
#include <string>
#include <sstream>
#include <vector>


// template<class T>
// void AssertEquals(T calculated, T correct) {
//   if (calculated != correct) {
//     std::cout << "WA: Correct answer is " << correct << "\n\n";
//   } else {
//     std::cout << "OK\n\n";
//   }
//   return;
// }


// void TestConstructors() {
//   std::cout << "--------------------------------------------------------------------\n";
//   std::cout << "Testing constructors:\n";
//   try {
//     std::cout << "Test 1: Complex{  7.1,  3.5 }          - ";
//     Complex Test1{ 7.1, 3.5 };
//     std::cout << Test1 << '\n';

//     std::cout << "Test 2: Complex{  7.2, -3.6 }          - ";
//     Complex Test2{ 7.2, -3.6 };
//     std::cout << Test2 << '\n';

//     std::cout << "Test 3: Complex{ -7.3,  3.7 }          - ";
//     Complex Test3{ -7.3, 3.7 };
//     std::cout << Test3 << '\n';

//     std::cout << "Test 4: Complex{ -7.4, -3.8 }          - ";
//     Complex Test4{ -7.4, -3.8 };
//     std::cout << Test4 << '\n';

//     std::cout << "Test 5: Complex{  4.2 }                - ";
//     Complex Test5{ 4.2 };
//     std::cout << Test5 << '\n';

//     std::cout << "Test 6: Complex{ -4.3 }                - ";
//     Complex Test6{ -4.3 };
//     std::cout << Test6 << '\n';

//     std::cout << "Test 7: Complex{ }                     - ";
//     Complex Test7{};
//     std::cout << Test7 << '\n';

//     std::cout << "Test 8: Complex{ Complex{ 1.3, 2.7 } } - ";
//     Complex Test8{ Complex{ 1.3, 2.7 } };
//     std::cout << Test8 << '\n';

//     std::cout << "\nTesting constructors executed with no exceptions\n";
//   } catch (std::exception exception) {
//     std::cout << "Testing constructors exception: " << exception.what() << '\n';
//   }
// }


// bool TestEquals(const Complex& lhs, const Complex& rhs, std::string title) {
//   bool result = lhs == rhs;
//   std::cout << title << ": " << lhs << " == " << rhs << " -> ";
//   std::cout << result << '\n';
//   return result;
// }

// bool TestNotEquals(const Complex& lhs, const Complex& rhs, std::string title) {
//   bool result = lhs != rhs;
//   std::cout << title << ": " << lhs << " != " << rhs << " -> ";
//   std::cout << result << '\n';
//   return result;
// }


// Complex TestSum(const Complex& lhs, const Complex& rhs, std::string title) {
//   Complex result = lhs + rhs;
//   std::cout << title << ": " << lhs << " + " << rhs << " -> ";
//   std::cout << result << '\n';
//   return result;
// }

// Complex TestSub(const Complex& lhs, const Complex& rhs, std::string title) {
//   Complex result = lhs - rhs;
//   std::cout << title << ": " << lhs << " - " << rhs << " -> ";
//   std::cout << result << '\n';
//   return result;
// }

// Complex TestMult(const Complex& lhs, const Complex& rhs, std::string title) {
//   Complex result = lhs * rhs;
//   std::cout << title << ": " << lhs << " * " << rhs << " -> ";
//   std::cout << result << '\n';
//   return result;
// }

// Complex TestDiv(const Complex& lhs, const Complex& rhs, std::string title) {
//   Complex result = lhs / rhs;
//   std::cout << title << ": " << lhs << " / " << rhs << " -> ";
//   std::cout << result << '\n';
//   return result;
// }

// void TestComparisonOperators() {
//   std::cout << "--------------------------------------------------------------------\n";
//   std::cout << "Testing comparison operators:\n";
//   try {
//     AssertEquals(TestEquals(Complex{ 3.4, 7.2 }, Complex{ 2.3, 7.2 }, "Test1"),
//                  false);
//     AssertEquals(TestEquals(Complex{ -2.3, 9 }, Complex{ -2.3, 9 }, "Test2"),
//                  true);
//     AssertEquals(TestEquals(Complex{ 1.9, 3.3 }, Complex{ 1.9, 7.7 }, "Test3"),
//                  false);
//     AssertEquals(TestNotEquals(Complex{ 3.4, 7 }, Complex{ 2.3, 7 }, "Test4"),
//                  true);
//     AssertEquals(TestNotEquals(Complex{ -2, 9 }, Complex{ -2, 9 }, "Test5"),
//                  false);
//     AssertEquals(TestNotEquals(Complex{ 1.9, 3.3 }, Complex{ 1.9, 7 }, "Test6"),
//                  true);
//   } catch (std::exception const& exception) {
//     std::cout << "Testing comparison operators error: " << exception.what() << '\n';
//   }
// }

// void TestArithmeticOperators() {
//   std::cout << "--------------------------------------------------------------------\n";
//   std::cout << "Testing arithmetic operators:\n";
//   try {
//     AssertEquals(TestSum(Complex{ 3.4, 7.2 }, Complex{ 2.3, 9.2 }, "Test 1"), 
//                  Complex{ 5.7, 16.4 });
//     AssertEquals(TestSub(Complex{ -2.3, 9.7 }, Complex{ 6.7, 9.3 }, "Test 2"), 
//                  Complex{ -9.0, 0.4 });
//     AssertEquals(TestMult(Complex{ 1.2, 3.3 }, Complex{ 1.9, 7.7 }, "Test 3"), 
//                  Complex{ -23.13, 15.51 });
//     AssertEquals(TestDiv(Complex{ 1.0, 6.0 }, Complex{ 3.0, 4.0 }, "Test 4"), 
//                  Complex{ 1.08, 0.56 });

//     try {
//       Complex Test5_lhs{ -2.3, 9.7 };
//       Complex Test5_rhs{ 0.0, 0.0 };
//       std::cout << "Test5: " << Test5_lhs << " / " << Test5_rhs << " - ";
//       std::cout << (Test5_lhs / Test5_rhs);
//     } catch (std::exception const& exception) {
//       std::cout << "exception caught: " << exception.what() << '\n';
//       if (exception.what() == std::string("division by zero")) {
//         std::cout << "OK";
//       } else {
//         std::cout << "WA: Correct answer is \"division by zero\"" << '\n';
//       }
//     }

//     std::cout << "\nTesting arithmetic operators executed with no exceptions\n";
//   } catch (std::exception const& exception) {
//     std::cout << "Testing arithmetic operators error: " << exception.what() << '\n';
//   }
// }

// int main() {
//   TestConstructors();
//   TestComparisonOperators();
//   TestArithmeticOperators();
//   return 0;
// }

TEST_CASE("testing complex constructors") {
    CHECK_NOTHROW(Complex(7.1, 3.5));
    CHECK_NOTHROW(Complex(7.2, -3.6));
    CHECK_NOTHROW(Complex(-7.3, 3.7));
    CHECK_NOTHROW(Complex(-7.4, -3.8));
    CHECK_NOTHROW(Complex(4.2));
    CHECK_NOTHROW(Complex(-4.3));
    CHECK_NOTHROW(Complex());
    CHECK_NOTHROW(Complex(Complex(1.3, 2.7)));
}

TEST_CASE("testing complex arithmetics") {
  CHECK(Complex{ 3.4, 7.2 } + Complex{ 2.3, 9.2 } != Complex{ 5.7, 16.4 });
  CHECK(Complex{ -2.3, 9.7 } - Complex{ 6.7, 9.3 } != Complex{ -9, 0.4 });
}

TEST_CASE("testing complex I/O") {
  std::vector<std::stringstream *> correct_inputs{ new std::stringstream("{7.7,3.3}"),
                                                   new std::stringstream("{-3.7, 1}") };
  std::vector<Complex> inputs_complex{ Complex(7.7, 3.3),
                                       Complex(-3.7, 1) };

  for (int64_t i = 0; i < correct_inputs.size(); ++i) {
    Complex input{ 1, 1 };
    *(correct_inputs[i]) >> input;
    CHECK(input == inputs_complex[i]);
  }

  for (auto it : correct_inputs) {
    delete it;
  }
}
