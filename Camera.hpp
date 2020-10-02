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

	bool castShadowray(const Scene&s , vec3& intersection, vec3& intersectionNormal);
private:
	// Camera plane
	const vec3 topLeft{0.0f, -1.0f, 1.0f};
	const vec3 topRight{-2.0f, 1.0f, 1.0f};
	const vec3 bottomLeft{-2.0f, -1.0f, -1.0f};
	const vec3 bottomRight{-2.0f, 1.0f, -1.0f};

	// camera positions
	const vec3 CameraPos1 = vec3(-3.0f, 0.0f, 0.0f);
	const vec3 CameraPos2 = vec3(-1.0f, 0.0f, 0.0f);
	const vec3 CameraTarget = vec3(0.0f, 0.0f, 0.0f);
	const vec3 CameraDirection = glm::normalize(CameraPos1 - CameraTarget);
	//Variable to switch between 2 eyepoints
	const vec3 CameraPos = CameraPos2;
	std::vector<Pixel> pixels{WIDTH*HEIGHT};
	
};

void Camera::createPixels() {

	for (int i = 0; i < HEIGHT; ++i) {
		for (int j = 0; j < WIDTH; ++j) {
			pixels[j + i * HEIGHT] = Pixel{topLeft + vec3{0.0f, 0.0025 * j, -0.0025 * i}};
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
			vec3 closestTriangle{1000.0, 0.0, 0.0};
			vec3 closestSphere{1000.0, 0.0, 0.0};
			vec3 intersectionNormal{};
			Triangle hit = s.checkTriangleIntersections(r, closestTriangle, intersectionNormal);
			Sphere sp = s.checkSphereIntersections(r, closestSphere, intersectionNormal);

			vec3 intersection = closestSphere.x < closestTriangle.x ? closestSphere : closestTriangle;
			// bool sphereHit = closestSphere.x < closestTriangle.x ? true : false;
			
			vec3 color = closestSphere.x < closestTriangle.x ? sp.getMaterial().reflect() : hit.getMaterial().reflect();
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

bool Camera::castShadowray(const Scene&s , vec3& intersection, vec3& intersectionNormal) {
	Light l = s.getLight();
	// const float EPSILON = 0.0001;
	if(intersection.z + EPSILON > l.getLightCenter().z) return false;

	//SHADOW BIAS
	vec3 startPoint = intersection + EPSILON * intersectionNormal;
	Ray r{intersection, glm::normalize(l.getLightCenter() - intersection)};
	Ray rBias{startPoint, glm::normalize(l.getLightCenter() - startPoint)};

	
	vec3 closestTriangle{1000.0f, 0.0f, 0.0f};
	vec3 closestSphere{1000.0f, 0.0f, 0.0f};

	Triangle hit = s.checkTriangleIntersections(r, closestTriangle, intersectionNormal);
	Sphere sp = s.checkSphereIntersections(rBias, closestSphere, intersectionNormal);
	
	double distToLight = glm::distance(l.getLightCenter(), intersection);
	double distToTriangle = glm::distance(closestTriangle, intersection);
	double distToSphere = glm::distance(closestSphere, startPoint);


	if(distToTriangle + EPSILON < distToLight ) return false;
	if(distToSphere + EPSILON < distToLight && glm::abs(distToSphere) > EPSILON) return false;
	return true;
	
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
			vec3 color = pixels[j + i * HEIGHT].getColor();

			fputc((int) (color.x * norm), f);
			fputc((int) (color.y * norm), f);
			fputc((int) (color.z * norm), f);
		}
	}
	fclose(f);
	std::cout << "FIle closed\n";
}