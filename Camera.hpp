#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include <iostream>
#include "Pixel.hpp"

// Image size
const int WIDTH = 800;
const int HEIGHT = 800;


class Camera {
public:
	Camera(){
		std::cout << "Camera";
		createPixels();
	};

	void createPixels() {
		for (int i = 0; i < HEIGHT; ++i) {
		for (int j = 0; j < WIDTH; ++j) {
			pixels[i][j] = Pixel{topLeft + glm::vec3{0.0f, 0.0025 * j, -0.0025 * i}};
			std::cout << pixels[i][j].getPosition().y << " ";
		}
		std::cout << "\n";
	}
	}
private:
	// Camera plane
	const glm::vec3 topLeft{0.0f, -1.0f, 1.0f};
	const glm::vec3 topRight{0.0f, 1.0f, 1.0f};
	const glm::vec3 bottomLeft{0.0f, -1.0f, -1.0f};
	const glm::vec3 bottomRight{0.0f, 1.0f, -1.0f};

	// camera positions
	const glm::vec3 CameraPos = glm::vec3(-2.0f, 0.0f, 0.0f);
	const glm::vec3 CameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	const glm::vec3 CameraDirection = glm::normalize(CameraPos - CameraTarget);

	Pixel pixels[WIDTH][HEIGHT];

};

// void Camera::createPixels() {
	
// 	for (int i = 0; i < HEIGHT; ++i) {
// 		for (int j = 0; j < WIDTH; ++j) {
// 			pixels[i][j] = Pixel{topLeft + glm::vec3{0.0f, 0.0025 * j, -0.0025 * i}};
// 			std::cout << pixels[i][j].getPosition().y << " ";
// 		}
// 		std::cout << "\n";
// 	}
// }
