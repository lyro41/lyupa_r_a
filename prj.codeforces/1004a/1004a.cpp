#include <iostream>
 
int main() {
  int n = 0;
  int d = 0;
  std::cin >> n >> d;
  
  int result = 2;                       // левее и правее крайних отелей
  int prev_x = 0;
  std::cin >> prev_x;
  
  int now_x = 0;
  for (int i = 1; i < n; ++i) {
    std::cin >> now_x;
    if (now_x - prev_x >= d + d) {
      ++result;
      if (now_x - prev_x != d + d) {
        ++result;
      }
    }
    prev_x = now_x;
  }
  
  std::cout << result;
  return 0;
}
