#include <iostream>
// #include "glm/glm.hpp"
// #include "glm/vec3.hpp"
// #include "Triangle.hpp"
#include "Scene.hpp"
#include "Camera.hpp"


int main() {
	Scene s{};
	s.drawRoom();
	Camera c{};
	c.render(s);

	return 0;
}
