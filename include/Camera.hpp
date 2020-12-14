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
#include "omp/omp.h"

#include "glm/gtx/rotate_vector.hpp"
#include <iostream>
#include <cmath>

// Image size
const int WIDTH = 800;
const int HEIGHT = 800;
const int MAX_PASSES = 5;

class Camera {
public:
	Camera(int _spp){
		spp = _spp;
		createPixels();
	};

	void createPixels();
	void render(const Scene& s);
	void createImage();
	ColorDbl traceRay(const Scene& s, Ray& r, int pass);
	ColorDbl sampleIndirectRay(const Scene &s, const vec3& point, const vec3& normal);
	
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
	int spp;
	
};
