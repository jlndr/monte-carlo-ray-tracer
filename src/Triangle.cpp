#include "../include/Triangle.hpp"

vec3 Triangle::getRandomPoint() {
	double area = calcArea();
	float u = randMinMax(0.0, 1.0 / area);
	float v = randMinMax(0.0, 1.0 / area);

	if(u + v > 1.0) return getRandomPoint();
	vec3 pos = (1.0f - u - v) * positions[0] + u * positions[1] + v * positions[2];
	return pos + 0.001f * vec3(0.0f, 0.0f, -1.0f);
}

bool Triangle::rayIntersection(Ray& r, vec3& intersection, vec3& intersectionNormal) {
	/*
	*	Möller Trumbore intersection algorithm
	*/
	// const double EPSILON = 0.00001;
	//Ta fram edges på triangeln
	vec3 edge1 = positions[1] - positions[0];
	vec3 edge2 = positions[2] - positions[0];
	vec3 rayDirection = r.getDirection();
	//DETERMINANTEN
	//Kryssprodukten KP mellan edge1 och rayDirection
	//determinanten = skalärprodukten mellan kryssprodukten och edge2
	//OM determinant VÄLDIGT liten -> raydirection är parallel med/ligger i triangelplanet
	//return false
	vec3 crossEdge2 = glm::cross(rayDirection, edge2);

	double det = glm::dot(edge1, crossEdge2);
	// if(det < 1 && det > -1) std::cout << "DET: " << det << "\n";
	if (det > -EPSILON && det < EPSILON) {
		// std::cout << " Miss pga av det\n "; 
		return false;
	}

	//invertera determinant
	//Ta fram vektor S från point1 till raystartpunkt
	// U = Skalärprodukten mellan S och kryssprodukten KP multiplicerat med inv determinant
	//om u är < 0 eller > 1  - return false
	double invDet = 1 / det;
	vec3 eyeToVertex = r.getStartPoint() - positions[0];
	double u = invDet * glm::dot(eyeToVertex, crossEdge2);
	if (u < 0.0 || u > 1.0) return false;

	//Vektor Q = S kryssprodukt med edge1
	//v = raydirection skalärprodukt Q * invDeter
	//om v < 0 eller u + v > 1   -  return false
	vec3 crossEdge1 = glm::cross(eyeToVertex, edge1);
	double v = invDet * glm::dot(rayDirection, crossEdge1);
	if (v < 0.0 || u + v > 1.0) return false;

	//done, kan ta reda på t om vi vill
	double t = invDet * glm::dot(edge2, crossEdge1);
	if (t > EPSILON) {
		// glm::
		intersection = r.getStartPoint() + rayDirection * (float) t;
		intersectionNormal = normal;
		return true;
	}

	return false;
}