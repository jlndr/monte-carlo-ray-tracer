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
		Material m{ColorDbl{0.989f, 0.929f, 0.773f}, LIGHTSOURCE};
		lightCenter = vec3{5.0f, 0.0f, 4.99f};
		vec3 p1{6.0f, -1.0f, 4.99f};
		vec3 p2{6.0f, 1.0f, 4.99f};
		vec3 p3{4.0f, -1.0f, 4.99f};
		vec3 p4{4.0f, 1.0f, 4.99f};
		lightTriangles.push_back(Triangle{p1, p2, p3, m});
		lightTriangles.push_back(Triangle{p4, p3, p2, m});
	}
	const std::vector<Triangle>  &getLightTriangles() const {return lightTriangles;}

	vec3 getLightCenter() {
		return lightCenter;
	}

private:
	std::vector<Triangle> lightTriangles;
	vec3 color;
	vec3 lightCenter;
};

