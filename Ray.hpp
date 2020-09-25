#pragma once
#include "glm/vec3.hpp"
#include "Triangle.hpp"
class Triangle;

class Ray {
public:
	Ray(){};
	Ray(glm::vec3 s, glm::vec3 e): startPoint{s}, direction{e}{};
	void setHitPoint(Triangle& t);
	glm::vec3 getDirection(){return direction;}
	glm::vec3 getStartPoint(){return startPoint;}
private:
	glm::vec3 startPoint;
	glm::vec3 direction;
	Triangle hitPoint;
};