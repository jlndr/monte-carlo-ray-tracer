#include <iostream>

#include "typeDefinitions.hpp"
#include "utils.hpp"

#include "Scene.hpp"
#include "Camera.hpp"

int main() {
	Scene s{};
	s.drawRoom();
	
	Camera c{};
	c.render(s);

	return 0;
}
