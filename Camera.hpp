#pragma once
#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include <iostream>
#include <vector>
#include "Pixel.hpp"
#include "Scene.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
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

	bool castShadowray(const Scene&s , glm::vec3& intersection, glm::vec3& intersectionNormal);
private:
	// Camera plane
	const glm::vec3 topLeft{0.0f, -1.0f, 1.0f};
	const glm::vec3 topRight{-2.0f, 1.0f, 1.0f};
	const glm::vec3 bottomLeft{-2.0f, -1.0f, -1.0f};
	const glm::vec3 bottomRight{-2.0f, 1.0f, -1.0f};

	// camera positions
	const glm::vec3 CameraPos1 = glm::vec3(-3.0f, 0.0f, 0.0f);
	const glm::vec3 CameraPos2 = glm::vec3(-1.0f, 0.0f, 0.0f);
	const glm::vec3 CameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	const glm::vec3 CameraDirection = glm::normalize(CameraPos1 - CameraTarget);
	//Variable to switch between 2 eyepoints
	const glm::vec3 CameraPos = CameraPos2;
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
			glm::vec3 closestTriangle{1000.0, 0.0, 0.0};
			glm::vec3 closestSphere{1000.0, 0.0, 0.0};
			glm::vec3 intersectionNormal{};
			Triangle hit = s.checkTriangleIntersections(r, closestTriangle, intersectionNormal);
			Sphere sp = s.checkSphereIntersections(r, closestSphere, intersectionNormal);

			glm::vec3 intersection = closestSphere.x < closestTriangle.x ? closestSphere : closestTriangle;
			// bool sphereHit = closestSphere.x < closestTriangle.x ? true : false;
			
			glm::vec3 color = closestSphere.x < closestTriangle.x ? sp.getColor() : hit.getColor();
			//Shadowray
			if(!castShadowray(s, intersection, intersectionNormal)) {
				color.x /= 2;
				color.y /= 2;
				color.z /= 2;
			}

			pixels[j + i * HEIGHT].setColor(color);
		}
	}
	createImage();
}

bool Camera::castShadowray(const Scene&s , glm::vec3& intersection, glm::vec3& intersectionNormal) {
	Light l = s.getLight();
	const float EPSILON = 0.00001;

	//SHADOW BIAS
	glm::vec3 startPoint = intersection + EPSILON * intersectionNormal;
	Ray r{startPoint, glm::normalize(l.getLightCenter() - startPoint)};

	
	glm::vec3 closestTriangle{1000.0f, 0.0f, 0.0f};
	glm::vec3 closestSphere{1000.0f, 0.0f, 0.0f};

	Triangle hit = s.checkTriangleIntersections(r, closestTriangle, intersectionNormal);
	Sphere sp = s.checkSphereIntersections(r, closestSphere, intersectionNormal);
	
		// if(glm::distance(closestSphere, intersection) < 2.0f) {
		// 	// std::cout << "SPhere hit: " << glm::distance(closestSphere, intersection) << "\n";
		// }
	//TODO SE ÖVER IFSATSER
	if(glm::distance(startPoint, closestSphere) > -EPSILON && glm::distance(startPoint, closestSphere) < EPSILON) return true;
	
	if(glm::distance(startPoint, closestTriangle) > glm::distance(startPoint, closestSphere)) return false;
	if(closestTriangle.z > 4.9 && closestTriangle.z < 4.99999) return true;
	double diff = glm::distance(startPoint, closestTriangle) - glm::distance(l.getLightCenter(), startPoint);
	if(diff > -EPSILON && diff < EPSILON) return true;
	return false;
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