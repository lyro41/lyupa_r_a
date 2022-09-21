
#include <iostream>
 
int main() {
	int year = 0;
	std::cin >> year;

	for (int y = year + 1; y <= 9012; ++y) {
		int digit4 = y % 10;
		int digit3 = y / 10 % 10;
		int digit2 = y / 100 % 10;
		int digit1 = y / 1000;

		if (digit1 == digit2 || digit1 == digit3 || digit1 == digit4 ||
			digit2 == digit3 || digit2 == digit4 || digit3 == digit4) {
			continue;
		}

		std::cout << y;
		break;
	}
  
	return 0;
}
