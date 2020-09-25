#pragma once
#include "glm/vec3.hpp"

class Ray {
public:
	Ray(){};
	Ray(glm::vec3 s, glm::vec3 e): startPoint{s}, direction{e}{};
	void setHitPoint(glm::vec3 hit);
	glm::vec3 getHitPoint();
	glm::vec3 getDirection(){return direction;}
	glm::vec3 getStartPoint(){return startPoint;}
private:
	glm::vec3 startPoint;
	glm::vec3 direction;
	glm::vec3 hitPoint;

};

glm::vec3 Ray::getHitPoint() {
	return hitPoint;
}

void Ray::setHitPoint(glm::vec3 hit) {
	hitPoint = hit;
}