#pragma once
#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include <iostream>
#include <vector>
#include "Pixel.hpp"
#include "Scene.hpp"
#include "Ray.hpp"
#include <cstdio>
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
	void createImage();
private:
	// Camera plane
	const glm::vec3 topLeft{-2.0f, -1.0f, 1.0f};
	const glm::vec3 topRight{-2.0f, 1.0f, 1.0f};
	const glm::vec3 bottomLeft{-2.0f, -1.0f, -1.0f};
	const glm::vec3 bottomRight{-2.0f, 1.0f, -1.0f};

	// camera positions
	const glm::vec3 CameraPos1 = glm::vec3(-3.0f, 0.0f, 0.0f);
	const glm::vec3 CameraPos2 = glm::vec3(-20.0f, 0.0f, 7.0f);
	const glm::vec3 CameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	const glm::vec3 CameraDirection = glm::normalize(CameraPos1 - CameraTarget);
	//Variable to switch between 2 eyepoints
	const glm::vec3 CameraPos = CameraPos1;
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
	// Pixel center = pixels[400 + 400 * HEIGHT];
	// std::cout << center.getPosition().x << " " << center.getPosition().y << " " << center.getPosition().z << "\n";
}

void Camera::render(const Scene& s) {
	std::cout << "\nRENDER\n";
	for (int i = 0; i < HEIGHT; ++i) {
		for (int j = 0; j < WIDTH; ++j) {
			Ray r{CameraPos, glm::normalize(pixels[j + i * HEIGHT].getCenterPoint() - CameraPos)};
			Triangle hit = s.checkTriangleIntersections(r);
			pixels[j + i * HEIGHT].setColor(hit.getColor());
		}
	}
	createImage();
}

void Camera::createImage() {
	
	//Open file
	FILE *f = fopen("image.ppm", "wb"); //b = binary mode
	fprintf(f, "P6\n %i %i 255\n", WIDTH, HEIGHT);
	std::cout << "FIle open\n";
	float norm = 255;
	//Normera till 255
	for (int i = 0; i < HEIGHT; ++i) {
		for (int j = 0; j < WIDTH; ++j) {
			glm::vec3 color = pixels[j + i * HEIGHT].getColor();

			fputc((int) (color.x * norm), f);
			fputc((int) (color.y * norm), f);
			fputc((int) (color.z * norm), f);
		}
	}
	fclose(f);
	std::cout << "FIle closed\n";
}