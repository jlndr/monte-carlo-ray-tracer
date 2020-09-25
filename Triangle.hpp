#pragma once
#include "glm/glm.hpp"
#include "Ray.hpp"

#include <array>

class Triangle {
public:
	Triangle(){};
   Triangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& _color){
		positions[0] = p1;
		positions[1] = p2;
		positions[2] = p3;
		color = _color;

		//  normal = glm::normalize(glm::vec3(1,1,1));
		normal = glm::normalize(glm::cross(p2-p1, p3-p2));
	};

	Triangle operator=(const Triangle &t) {
		positions = t.positions;
		normal = t.normal;
		color = t.color;
		return *this;
	};
    
	//referens eller kopia
  glm::vec3 &getNormal() {return normal;}
	glm::vec3 getColor() {return color;}
	bool rayIntersection(Ray& r);

private:
	std::array<glm::vec3, 3> positions;
	glm::vec3 normal;
	glm::vec3 color;
};

bool Triangle::rayIntersection(Ray& r) {
	/*
	*	Möllet Trumbore intersection algorithm
	*/
	const float EPSILON = 0.0000001;
	//Ta fram edges på triangeln
	glm::vec3 edge1 = positions[1] - positions[0];
	glm::vec3 edge2 = positions[2] - positions[0];
	glm::vec3 rayDirection = r.getDirection();
	//DETERMINANTEN
	//Kryssprodukten KP mellan edge1 och rayDirection
	//determinanten = skalärprodukten mellan kryssprodukten och edge2
	//OM determinant VÄLDIGT liten -> raydirection är parallel med/ligget i triangelplanet
	//return false
	glm::vec3 crossEdge2 = glm::cross(rayDirection, edge2);

	double det = glm::dot(edge1, crossEdge2);
	if (det > -EPSILON && det < EPSILON) return false;

	//invertera determinant
	//Ta fram vektor S från point1 till raystartpunkt
	// U = Skalärprodukten mellan S och kryssprodukten KP multiplicerat med inv determinant
	//om u är < 0 eller > 1  - return false
	double invDet = 1 / det;
	glm::vec3 eyeToVertex = r.getStartPoint() - positions[0];
	double u = invDet * glm::dot(eyeToVertex, crossEdge2);
	if (u < 0.0 || u > 1.0) return false;

	//Vektor Q = S kryssprodukt med edge1
	//v = raydirection skalärprodukt Q * invDeter
	//om v < 0 eller u + v > 1   -  return false
	glm::vec3 crossEdge1 = glm::cross(eyeToVertex, edge1);
	double v = invDet * glm::dot(rayDirection, crossEdge1);
	if (v < 0.0 || u + v > 1.0) return false;

	//done, kan ta reda på t om vi vill
	double t = invDet * glm::dot(edge2, crossEdge1);
	if (t > EPSILON) {
		//TODO Räkna ut exakta träffpunkten där nästa ray ska studsa ifrån
		return true;
	}
	
	return false;
}