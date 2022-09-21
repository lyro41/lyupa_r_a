#include <queuep/queuep.h>
#include <iostream>

int main() {
  QueueP test;
  std::cout << test << '\n';
  test.Push(17);
  std::cout << test << '\n';
  test.Push(16);
  std::cout << test << '\n';
  test.Push(23);
  std::cout << test << '\n';
  QueueP test_copy = test;
  std::cout << test_copy << ' ' << test << '\n';
  test.Pop();
  std::cout << test << '\n';
  return 0;
}