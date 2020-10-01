#pragma once

#include "glm/vec3.hpp"
#include "vector"
#include "Triangle.hpp"

class Light {
public:
	Light(){
		/* 
		*   p1  -   p2 
		*	 |   	/  |
		*	 |	/ 		|	
		*	 p3  -	p4
		*/
		//centrum (5 0 5)
		color = glm::vec3{0.989f, 0.929f, 0.773f};
		lightCenter = glm::vec3{5.0f, 0.0f, 4.99f};
		glm::vec3 p1{6.0f, -1.0f, 4.99f};
		glm::vec3 p2{6.0f, 1.0f, 4.99f};
		glm::vec3 p3{4.0f, -1.0f, 4.99f};
		glm::vec3 p4{4.0f, 1.0f, 4.99f};
		lightTriangles.push_back(Triangle{p1, p2, p3, color});
		lightTriangles.push_back(Triangle{p4, p3, p2, color});
	}
	const std::vector<Triangle>  &getLightTriangles() const {return lightTriangles;}

	glm::vec3 getLightCenter() {
		return lightCenter;
	}

private:
	std::vector<Triangle> lightTriangles;
	glm::vec3 color;
	glm::vec3 lightCenter;
};

