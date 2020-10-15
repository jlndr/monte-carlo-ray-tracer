#pragma once

#include "glm/vec3.hpp"
#include "vector"
#include "Triangle.hpp"
#include "typeDefinitions.hpp"

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
	
		lightCenter = vec3{5.0f, 0.0f, 4.9f};
		color = m.getColor();
		emission = m.getColor();
		vec3 p1{6.0f, 1.0f, 4.99f};
		vec3 p2{6.0f, -1.0f, 4.99f};
		vec3 p3{4.0f, 1.0f, 4.99f};
		vec3 p4{4.0f, -1.0f, 4.99f};
		lightTriangles.push_back(Triangle{p1, p2, p3, m});
		lightTriangles.push_back(Triangle{p4, p3, p2, m});
		// Triangle test{p4, p3, p2, m};
		// std::cout << "Normal : " << test.getNormal().x << " " << test.getNormal().y << " " << test.getNormal().z << " \n";
	}
	const std::vector<Triangle>  &getLightTriangles() const {return lightTriangles;}

	vec3 getLightCenter() const {
		return lightCenter;
	}
	ColorDbl getEmission() const {
		return emission;
	}
	ColorDbl getColor() const {
		return color;
	}

private:
	std::vector<Triangle> lightTriangles;
	ColorDbl color;
	vec3 lightCenter;
	ColorDbl emission;
	Material m{ColorDbl{1.0f, 1.0f, 1.0f}, LIGHTSOURCE};
};

