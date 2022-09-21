#include <iostream>
 
int main() {
  int n = 0;
  std::cin >> n;
  
  int a = 0;
  for (int i = 0; i < n; ++i) {
    std::cin >> a;
    if (a % 2 == 0) {
      std::cout << a - 1 << ' ';
    } else {
      std::cout << a << ' ';
    }
  }
  
  return 0;
}
