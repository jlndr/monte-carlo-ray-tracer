#pragma once
#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include <iostream>
#include <vector>
#include "Pixel.hpp"
#include "Scene.hpp"
#include "Ray.hpp"

// Image size
const int WIDTH = 800;
const int HEIGHT = 800;


class Camera {
public:
	Camera(){
		createPixels();
	};

	void createPixels();
	void render(const Scene& s);
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

	std::vector<Pixel> pixels{WIDTH*HEIGHT};
	
};

void Camera::createPixels() {

	for (int i = 0; i < HEIGHT; ++i) {
		for (int j = 0; j < WIDTH; ++j) {
			pixels[j + i * HEIGHT] = Pixel{topLeft + glm::vec3{0.0f, 0.0025 * j, -0.0025 * i}};
			// std::cout << pixels[j + i * HEIGHT].getPosition().y << " ";
		}
		// std::cout << "\n";
	}
}

void Camera::render(const Scene& s) {
	std::cout << "\nRENDER\n";
	for (int i = 0; i < HEIGHT; ++i) {
		for (int j = 0; j < WIDTH; ++j) {
			Ray r{CameraPos, glm::normalize(pixels[j + i * HEIGHT].getCenterPoint() - CameraPos)};
			Triangle hit = s.checkIntersections(r);
			pixels[j + i * HEIGHT].setColor(hit.getColor());
		}
	}
}