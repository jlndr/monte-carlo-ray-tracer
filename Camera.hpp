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
#include "utils.hpp"

#include "glm/gtx/rotate_vector.hpp"
#include <iostream>
#include <cmath>

// Image size
const int WIDTH = 800;
const int HEIGHT = 800;
const int MAX_PASSES = 2;
int count = 0;
int count2 = 0;
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
	const vec3 topLeft{0.0f, 1.0f, 1.0f};
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

	double maxValue = 0;
	
};

ColorDbl Camera::traceRay(const Scene& s, Ray& r, int pass) {
	vec3 closestTriangle{1000.0, 0.0, 0.0};
	vec3 closestSphere{1000.0, 0.0, 0.0};
	ColorDbl color{};
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

	vec3 intersection = glm::distance(closestSphere, r.getStartPoint()) < glm::distance(closestTriangle, r.getStartPoint())  ? closestSphere : closestTriangle;
	vec3 intersectionNormal = glm::distance(closestSphere, r.getStartPoint()) < glm::distance(closestTriangle, r.getStartPoint())  ? intersectionNormalS : intersectionNormalT;
	// bool sphereHit = closestSphere.x < closestTriangle.x ? true : false;

	Material m = glm::distance(closestSphere, r.getStartPoint()) < glm::distance(closestTriangle, r.getStartPoint())  ? sp.getMaterial() : hit.getMaterial();
	color = m.reflect();

	ColorDbl light = s.calcLight(intersection, intersectionNormal);

	color *= light;
	if(pass != 0) {
		float dist = glm::distance(intersection, r.getStartPoint());
		
		if(dist > 1) color *=  1 / glm::pow(dist, 2.0); // no risk of dividing with zero
		else color *= 3.0;
		
		//else color *= 0;
	}
	//bouncing shit
	// check if we should bounce with russian roulette
	//if yes
	double contr = glm::max(glm::max(color.x, color.y), color.z);
	++count;
	if(pass < MAX_PASSES && uniformRand() < contr) {
		float inclAngle = (float)acos(uniformRand());
		float azimAngle = (float)2 * M_PI * uniformRand();
		
		vec3 rp = r.getDirection();
		vec3 rProj{glm::normalize(rp - glm::dot(rp, intersectionNormal) * intersectionNormal)};

		vec3 thirdAxis{glm::cross(rProj, intersectionNormal)};
		vec3 randDir = intersectionNormal;
		
		randDir = glm::normalize(glm::rotate(
			randDir,
			inclAngle,
			rProj
		));

		randDir = glm::normalize(glm::rotate(
			randDir,
			azimAngle,
			intersectionNormal
		));

		Ray reflectedRay{intersection, randDir};
		color += traceRay(s, reflectedRay, ++pass) * (1.0 / (pass + 1)); // + 1 is to avoid dividing with zero which is illegal
		count2++;
	}
	// call hemis func s.hemisU(intersection, normal mm)
	
	// if no

	return color;
}

void Camera::createPixels() {

	for (int i = 0; i < HEIGHT; ++i) {
		for (int j = 0; j < WIDTH; ++j) {
			pixels[j + i * HEIGHT] = Pixel{topLeft + vec3{0.0f, -0.0025 * j, -0.0025 * i}};
			// std::cout << pixels[j + i * HEIGHT].getPosition().y << " ";
		}
		// std::cout << "\n";
	}
	// Pixel center = pixels[400 + 400 * HEIGHT];
	// std::cout << center.getPosition().x << " " << center.getPosition().y << " " << center.getPosition().z << "\n";
}

void Camera::render(const Scene& s) {
	const int SUBPIXELS = 2; //In each direction
	const float subSideLength = Pixel::getSideLength() / SUBPIXELS; 
	std::cout << "\nRENDER\n";
	// double max = 800*800;
	// int counter = 0;
	for (int i = 0; i < HEIGHT; ++i) {
		for (int j = 0; j < WIDTH; ++j) {
			ColorDbl color{0.0, 0.0, 0.0};
			
			//Loop over subpixels
			for(int k = 0; k < SUBPIXELS; ++k) {
				for(int m = 0; m < SUBPIXELS; ++m) {
					
					double y = randMinMax(k * subSideLength, subSideLength + k * subSideLength);
					double z = randMinMax(m * subSideLength, subSideLength + m * subSideLength);
					vec3 point{0.0, y, z};
					
					// if(i == 400) std::cout << "y: " << point.y << " " << "z: " << point.z << "\n"; 
					Ray r{CameraPos, glm::normalize(pixels[j + i * HEIGHT].getPosition() - point - CameraPos)};
					for(int n = 0; n < 3; ++n) {
						color += traceRay(s, r, 0);
					}
				}
			}

			// Ray r{CameraPos, glm::normalize(pixels[j + i * HEIGHT].getCenterPoint() - CameraPos)};
			// ColorDbl color = traceRay(s, r, 0);
			
			pixels[j + i * HEIGHT].setColor(color);
			if(color.x > maxValue) maxValue = color.x;
			if(color.y > maxValue) maxValue = color.y;
			if(color.z > maxValue) maxValue = color.z;	
		// ++counter;
		// std::cout << "\r" << counter/max << "%";
		}
	}
	std::cout << "Counter1: " << count << "\n";
	std::cout << "Counter2: " << count2 << "\n";

	createImage();
}

void Camera::createImage() {

	//Open file
	FILE *f = fopen("image.ppm", "wb"); //b = binary mode
	fprintf(f, "P6\n %i %i 255\n", WIDTH, HEIGHT);
	std::cout << "FIle open\n";
	double norm = 255.0 / maxValue;
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