#include <iostream>
#include "glm/glm.hpp"
#include "glm/vec3.hpp"
// #include "Triangle.hpp"
// #include "Scene.hpp"
#include "Camera.hpp"


int main() {
	std::cout << "hej";
	Camera c{};
	c.createPixels();
	// Scene s; 
	//s.drawRoom();
	std::cout << "Camera";

	return 0;
}
