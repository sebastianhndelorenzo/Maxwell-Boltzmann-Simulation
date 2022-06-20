#include <iostream>
#include <vector>

int main() {
	std::vector<int> integers = { 100, 200, 300, 400, 500 };
	std::ptrdiff_t index = std::distance(integers.data(), &integers[2]);
	std::cout << index;
}