#pragma once
#include "glm/vec3.hpp"
#include "glm/glm.hpp"


class Pixel {
public:
	Pixel(): position{glm::vec3{}}, centerPoint{glm::vec3{}}{}

	Pixel(glm::vec3 pos){
		position = pos;
		centerPoint = glm::vec3{position.x, position.y + 0.0025/2, position.z - 0.0025/2};
	}

	Pixel operator=(const Pixel &p) {
		centerPoint = p.centerPoint;
		position = p.position;
		return *this;
	};

	float getSideLength() {return sideLength;}
	glm::vec3 getPosition() {return position;}
	glm::vec3 getCenterPoint() {return centerPoint;}
	void setColor(glm::vec3 c) { color = c;}
	glm::vec3 getColor() {return color;}

private:
	const float sideLength = 0.0025;
	glm::vec3 position;
	glm::vec3 centerPoint;
	glm::vec3 color;

};


