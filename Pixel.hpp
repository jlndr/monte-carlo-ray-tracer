#pragma once
#include "glm/vec3.hpp"
#include "glm/glm.hpp"


class Pixel {
public:
	Pixel(): position{vec3{}}, centerPoint{vec3{}}{}

	Pixel(vec3 pos){
		position = pos;
		centerPoint = vec3{position.x, position.y + 0.0025/2, position.z - 0.0025/2};
	}

	Pixel operator=(const Pixel &p) {
		centerPoint = p.centerPoint;
		position = p.position;
		return *this;
	};

	float getSideLength() {return sideLength;}
	vec3 getPosition() {return position;}
	vec3 getCenterPoint() {return centerPoint;}
	void setColor(vec3 c) { color = c;}
	vec3 getColor() {return color;}

private:
	const float sideLength = 0.0025;
	vec3 position;
	vec3 centerPoint;
	vec3 color;

};


