#include <iostream>
#include "../include/Scene.hpp"
#include "../include/Camera.hpp"

// #include "../include/typeDefinitions.hpp"
// #include "../include/utils.hpp"


#include <chrono>

int main() {
	Scene s{};
	s.drawRoom();
	
	Camera c{1}; // 10 spp
	std::cout << "****START****" << "\n";
	auto start = std::chrono::system_clock::now();
	/* do some work */

	c.render(s);

	auto end = std::chrono::system_clock::now();
	std::cout << "****END****" << "\n";
	auto elapsed = end - start;
	std::cout << "TIME: " << elapsed.count() << " microseconds.\n";

	return 0;
}