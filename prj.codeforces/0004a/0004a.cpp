#include <iostream>
 
int main() {
	int W = 0;
	std::cin >> W;
  
	if (W % 2 == 0 && W > 3) {
		std::cout << "YES";
	} else {
		std::cout << "NO";
	}

	return 0;
}
