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
const int MAX_PASSES = 6;

class Camera {
public:
	Camera(){
		createPixels();
	};

	void createPixels();
	void render(const Scene& s);
	void createImage();
	ColorDbl traceRay(const Scene& s, Ray& r, int pass);
	
	
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

ColorDbl Camera::traceRay(const Scene& s, Ray& r, int pass) {
	vec3 closestTriangle{1000.0, 0.0, 0.0};
	vec3 closestSphere{1000.0, 0.0, 0.0};
	vec3 color{};
	vec3 intersectionNormalT{};
	vec3 intersectionNormalS{};
	Triangle hit = s.checkTriangleIntersections(r, closestTriangle, intersectionNormalT);
	Sphere sp = s.checkSphereIntersections(r, closestSphere, intersectionNormalS);

	if(glm::distance(r.getStartPoint(), closestTriangle) < glm::distance(r.getStartPoint(), closestSphere)) {
		Material m = hit.getMaterial();
		if(m.isType(LIGHTSOURCE)) return m.getColor();
		else if(m.isType(PERFECT_REFL)) {
			if(pass == MAX_PASSES) return m.reflect();
			vec3 newDirection = glm::reflect(r.getDirection(), glm::normalize(hit.getNormal()));
			Ray newRay{closestTriangle, newDirection};
			 return traceRay(s, newRay, ++pass);
		}
	}
	else { //sphere stuff
		Material m = sp.getMaterial();
		if(m.isType(LIGHTSOURCE)) return m.getColor();
		else if(m.isType(PERFECT_REFL)) {
			if(pass == MAX_PASSES) return m.reflect();
			vec3 newDirection = glm::reflect(r.getDirection(), glm::normalize(intersectionNormalS));
			Ray newRay{closestSphere + intersectionNormalS * EPSILON, newDirection};
			 return traceRay(s, newRay, ++pass);
		}
		// return sp.getMaterial().getColor();
	}

	vec3 intersection = closestSphere.x < closestTriangle.x ? closestSphere : closestTriangle;
	vec3 intersectionNormal = closestSphere.x < closestTriangle.x ? intersectionNormalS : intersectionNormalT;
	// bool sphereHit = closestSphere.x < closestTriangle.x ? true : false;
	
	color = closestSphere.x < closestTriangle.x ? sp.getMaterial().reflect() : hit.getMaterial().reflect();
	
	ColorDbl light = s.calcLight(intersection, intersectionNormal);

	color *= light;
	//Shadowray
	// if(!castShadowray(s, intersection, intersectionNormal)) {
	// 	color.x /= 2;
	// 	color.y /= 2;
	// 	color.z /= 2;
	// }
	return color;
}

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
			ColorDbl color = traceRay(s, r, 0);

			pixels[j + i * HEIGHT].setColor(color);
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
			vec3 color = pixels[j + i * HEIGHT].getColor();

			fputc((int) (color.x * norm), f);
			fputc((int) (color.y * norm), f);
			fputc((int) (color.z * norm), f);
		}
	}
	fclose(f);
	std::cout << "FIle closed\n";
}