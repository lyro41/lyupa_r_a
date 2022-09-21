#include <iostream>
#include <vector>
 
int main() {
	int64_t n = 0, k = 0, f = 0, t = 0;
	std::cin >> n >> k;
  
	int64_t res = -1e9;
	for (int i = 0; i < n; ++i) {
		std::cin >> f >> t;
		if (t <= k) {
			res = res > f ? res : f;
		} else {
			res = res > f - (t - k) ? res : f - (t - k);
		}
	}
  
	std::cout << res;
	return 0;
}
