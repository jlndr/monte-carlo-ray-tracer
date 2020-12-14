#pragma once
#include "glm/glm.hpp"
#include "Ray.hpp"
#include "Material.hpp"

class Sphere {
public:
	Sphere(vec3 _position = vec3{0}, double _radius = 0, const Material& m = Material{})
	:	position{_position},
		radius{_radius},
		material{m}{};

	Sphere(const Sphere& _s) {
		position = _s.position;
		radius = _s.radius;
		material = _s.material;
	}

	Sphere operator=(const Sphere& _s) { 
		position = _s.position;	
		radius = _s.radius;
		material = _s.material;
		return *this;
	}

	bool rayIntersection(Ray& r, vec3& intersection, vec3& intersectionNormal) const;
	Material getMaterial() {return material;}

private:
	vec3 position;
	double radius;
	Material material;
};