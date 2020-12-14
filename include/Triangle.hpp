#pragma once
#include "utils.hpp"

#include "glm/glm.hpp"
#include "Ray.hpp"
#include "Material.hpp"

#include <array>

class Triangle {
public:
	Triangle(){};
   Triangle(const vec3& p1, const vec3& p2, const vec3& p3, const Material m){
		positions[0] = p1;
		positions[1] = p2;
		positions[2] = p3;
		material = m;            //
		normal = glm::normalize(glm::cross(p2-p1, p3-p1));
	};

	Triangle operator=(const Triangle &t) {
		positions = t.positions;
		normal = t.normal;
		material = t.material;
		return *this;
	}
    
	bool rayIntersection(Ray& r, vec3& intersection, vec3& intersectionNormal);
	
	vec3 &getNormal() {return normal;}
	Material getMaterial() {return material;}
	double calcArea() const {
		return 0.5 * glm::length(glm::cross(positions[1] - positions[0], positions[2] - positions[0]));
	}

	vec3 getRandomPoint();

private:
	std::array<vec3, 3> positions;
	vec3 normal;
	Material material;
};