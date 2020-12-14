#pragma once

#include <vector>

#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include "typeDefinitions.hpp"
#include "Triangle.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "Light.hpp"
#include "Material.hpp"


class Scene {

public:
	Scene(){};
	void drawRoom();
	Triangle checkTriangleIntersections (Ray &ray, vec3& closestTriangle, vec3& intersectionNormal) const;
	Sphere checkSphereIntersections(Ray &ray, vec3& closestSphere, vec3& intersectionNormal) const;
	
	void addLight() {
		
		std::vector<Triangle> lightTriangles = l.getLightTriangles();
		
		for(auto t : lightTriangles) {
			room.push_back(t);
		}
	}

	ColorDbl calcLight(vec3& intersection, vec3& intersectionNormal, const vec3& dirToEye, const int& materialType) const;
	bool castShadowray(vec3& intersection, vec3& intersectionNormal, vec3& randLightPoint) const;
	bool caustic(vec3& p, vec3& in, Ray& r, int pass = 0) const;
	
private:
	std::vector<Triangle> room;
	std::vector<Sphere> spheres;
	
	Light l{};
	
};