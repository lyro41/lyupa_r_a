#include <iostream>
#include <string>

int main() {
  int n = 0;
  std::cin >> n;
  
  std::string t = "";
  std::cin >> t;
  
  for (int i = 2; i <= n; ++i) {
    if (n % i == 0) {
      for (int j = 0; j < i / 2; ++j) {
        std::swap(t[j], t[i - j - 1]);
      }
    }
  }
  
  std::cout << t;
  
  return 0;
}
