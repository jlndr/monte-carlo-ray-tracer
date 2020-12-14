#include "../include/Sphere.hpp"

bool Sphere::rayIntersection(Ray& r, vec3& intersection, vec3& intersectionNormal) const {
	double p1, p2; // intersection points

	vec3 L = r.getStartPoint() - position; 
	float a = glm::dot(r.getDirection(), r.getDirection()); 
	float b = 2 * glm::dot(r.getDirection(), L);
	float c = glm::dot(L, L) - radius * radius; 

	float discr = b * b - 4 * a * c; 
    if (discr < 0) return false; 
    else if (discr == 0) p1 = p2 = - 0.5 * b / a; 
    else { 
        float q = (b > 0) ? 
            -0.5 * (b + sqrt(discr)) : 
            -0.5 * (b - sqrt(discr)); 
        p1 = q / a; 
        p2 = c / q; 
    } 
   //  if (p1 > p2) std::swap(p1, p2); 
        
	if(p1 > p2) std::swap(p1, p2);

	if(p1 < 0) {
		p1 = p2; //if p1 is negative , use p2
		if(p1 < 0) return false;
	}
	
	intersection = r.getStartPoint() + vec3{r.getDirection() * (float) p1};
	intersectionNormal = glm::normalize(intersection - position); 
	return true;
}

//  