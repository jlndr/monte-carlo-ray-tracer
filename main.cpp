#include <iostream>

#include "typeDefinitions.hpp"
#include "utils.hpp"

#include "Scene.hpp"
#include "Camera.hpp"

int main() {
	Scene s{};
	s.drawRoom();
	

	Camera c{};
	std::cout << "****START****" << '\n';
	auto start = std::chrono::system_clock::now();
	/* do some work */

	c.render(s);

	auto end = std::chrono::system_clock::now();
	std::cout << "****END****" << '\n';
	auto elapsed = end - start;
	std::cout << "TIME: " << elapsed.count() << '\n';

	return 0;
}
//TIME (ms?)
//1	: 89015479
//2	: 52862842
//4	: 35298508
//6	: 
//8	: 39078006