#pragma once
#include "glm/glm.hpp"
#include "Ray.hpp"

class Sphere {
public:
	Sphere(glm::vec3 _position = glm::vec3{}, double _radius = 0, glm::vec3 _color = glm::vec3{})
	:	position{_position},
		radius{_radius},
		color{_color}{};

	Sphere(const Sphere& _s) {
		position = _s.position;
		radius = _s.radius;
		color = _s.color;
	}

	Sphere operator=(const Sphere& _s) { 
		position = _s.position;	
		radius = _s.radius;
		color = _s.color;
		return *this;
	}

	bool rayIntersection(Ray& r, glm::vec3& intersection) const;
	glm::vec3 getColor(){return color;}

private:
	glm::vec3 position;
	double radius;
	glm::vec3 color{0.0f, 0.0f, 0.0f};
};

bool Sphere::rayIntersection(Ray& r, glm::vec3& intersection) const {
	
	return true;
}