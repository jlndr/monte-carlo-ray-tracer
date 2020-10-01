#pragma once
#include "glm/glm.hpp"
#include "Ray.hpp"

class Sphere {
public:
	Sphere(glm::vec3 _position = glm::vec3{}, double _radius = 0, glm::vec3 _color = glm::vec3{})
	:	position{_position},
		radius{_radius},
		color{_color}{};

	Sphere(const Sphere& _s) {
		position = _s.position;
		radius = _s.radius;
		color = _s.color;
	}

	Sphere operator=(const Sphere& _s) { 
		position = _s.position;	
		radius = _s.radius;
		color = _s.color;
		return *this;
	}

	bool rayIntersection(Ray& r, glm::vec3& intersection, glm::vec3& intersectionNormal) const;
	glm::vec3 getColor(){return color;}

private:
	glm::vec3 position;
	double radius;
	glm::vec3 color{0.0f, 0.0f, 0.0f};
};

bool Sphere::rayIntersection(Ray& r, glm::vec3& intersection, glm::vec3& intersectionNormal) const {
	double p1, p2; // intersection points

	// glm::vec3 L = position - r.getStartPoint();
	// double Lproj = glm::dot(L, r.getDirection());

	// double d2 = glm::dot(L, L) - glm::pow(Lproj, 2);
	// if(d2 > glm::pow(radius, 2)) return false;

	// double rProj = glm::sqrt(glm::pow(radius,2) - d2);
	// p1 = Lproj - rProj;
	// p2 = Lproj + rProj;
	glm::vec3 L = r.getStartPoint() - position; 
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
	
	intersection = r.getStartPoint() + glm::vec3{r.getDirection().x * p1, r.getDirection().y * p1, r.getDirection().z * p1};
	intersectionNormal = glm::normalize(intersection - position); 
	return true;
}
// bool Sphere::rayIntersection(Ray& ray, glm::vec3 &intersection) const{
//     //Ray offset from sphere center
//    glm::vec3 normalizedRayDirection = ray.getDirection();
//    glm::vec3 rayOrigin = ray.getStartPoint();
//    glm::vec3 sphereCenter = position;
//    float sphereRadius = radius;
		
//    float b = glm::dot((2.0f*normalizedRayDirection), (rayOrigin-sphereCenter));
//    float ac = glm::dot(rayOrigin-sphereCenter, rayOrigin-sphereCenter)-glm::pow(sphereRadius, 2);
//    float d1 = -b / 2.0f;
//    float d2 = d1;
//    float bsqrt = glm::pow(d1, 2) - ac;
//    d1 += bsqrt;
//    d2 -= bsqrt;
// 	 const double EPSILON = 0.00000000000000001;
//    if(bsqrt < EPSILON)
//        return false;

//     bsqrt = glm::sqrt(bsqrt);
//    if(d1 <= 0 && d2 <= 0){
//        return false;
//    }
//    else if(d2 < d1) {
//        intersection = rayOrigin + d2*normalizedRayDirection;
//        return true;
//    }
//    else if(d1 < d2) {
//        intersection = rayOrigin + d1*normalizedRayDirection;
//        return true;
//    }
//    else {
//        intersection = rayOrigin + d2*normalizedRayDirection;
//        return true;
//    }

//    //This will never happen.
//    return false;
// }