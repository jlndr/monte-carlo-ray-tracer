#pragma once

#include "typeDefinitions.hpp"

class Ray {
public:
	Ray(){};
	Ray(vec3 s, vec3 d): startPoint{s}, direction{glm::normalize(d)}{};

	vec3 getDirection(){return direction;}
	vec3 getStartPoint(){return startPoint;}
private:
	vec3 startPoint;
	vec3 direction;
};