#pragma once

class Ray {
public:
	Ray(){};
	Ray(vec3 s, vec3 d): startPoint{s}, direction{glm::normalize(d)}{};
	void setHitPoint(vec3 hit);
	vec3 getHitPoint();
	vec3 getDirection(){return direction;}
	vec3 getStartPoint(){return startPoint;}
private:
	vec3 startPoint;
	vec3 direction;
	vec3 hitPoint;

};

vec3 Ray::getHitPoint() {
	return hitPoint;
}

void Ray::setHitPoint(vec3 hit) {
	hitPoint = hit;
}