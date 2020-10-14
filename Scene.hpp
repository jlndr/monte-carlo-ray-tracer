#pragma once
#include <iostream>
#include <array>
#include <vector>

#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include "typeDefinitions.hpp"
#include "Triangle.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "Light.hpp"
#include "Material.hpp"


class Scene {

public:
	Scene(){};
	void drawRoom();
	Triangle checkTriangleIntersections (Ray &ray, vec3& closestTriangle, vec3& intersectionNormal) const;
	Sphere checkSphereIntersections(Ray &ray, vec3& closestSphere, vec3& intersectionNormal) const;
	
	void addSphere(Sphere _s) {
		s = _s;
	}

	void addLight() {
		
		std::vector<Triangle> lightTriangles = l.getLightTriangles();
		
		for(auto t : lightTriangles) {
			room.push_back(t);
		}
	}

	ColorDbl calcLight(vec3& intersection, vec3& intersectionNormal) const;
	bool castShadowray(vec3& intersection, vec3& intersectionNormal, vec3& randLightPoint) const;
	
private:
	std::vector<Triangle> room;
	Sphere s;
	Light l{};
	
};

Triangle Scene::checkTriangleIntersections(Ray &r, vec3& closestTriangle, vec3& intersectionNormal) const{
	// Loop through all triangles and check if we have a hi 
	// For now when no object in the scene, intersection with only wall or floor or roof
	Triangle t{};
	for(auto i : room) {
		vec3 intersection{};
		if(!i.rayIntersection(r, intersection, intersectionNormal)) continue;
		if(glm::distance(r.getStartPoint(), intersection) < glm::distance(r.getStartPoint(), closestTriangle)) {
			t = i;
			closestTriangle = intersection;
		}	
	}
	return t;
}

Sphere Scene::checkSphereIntersections(Ray &r, vec3& closestSphere, vec3& intersectionNormal) const {
	Sphere _s{};
	vec3 intersection{};
	if(s.rayIntersection(r, intersection, intersectionNormal)) {
		_s = s;
		closestSphere = intersection;
	}
	
	return _s;
}

ColorDbl Scene::calcLight(vec3& intersection, vec3& intersectionNormal) const {
	ColorDbl color = ColorDbl{0.0, 0.0, 0.0};
	double area = 0;
	int counter = 0;
	double angle = 0.0;
	for(Triangle LT : l.getLightTriangles()) {
		area += LT.calcArea();
		for(int i = 0; i < 3; ++i){

			// vec3 randLightPoint = LT.getRandomPoint();
			vec3 randLightPoint = l.getLightCenter();
			++counter;
			
			if(!castShadowray(intersection, intersectionNormal, randLightPoint)) {
				// color.x /= 2;
				// color.y /= 2;
				// color.z /= 2;
				continue;
			}
			angle = glm::dot(-glm::normalize(intersectionNormal), glm::normalize(randLightPoint - intersection)) ;
			double beta = glm::clamp((double) glm::dot(LT.getNormal(), -glm::normalize(randLightPoint - intersection)), 0.0, 1.0);
			// glm::pow(glm::distance(intersection, randLightPoint), 2.0)
			double geo = glm::clamp(angle, 0.0, 1.0)  / glm::pow(glm::distance(intersection, randLightPoint), 2.0);
			color += l.getColor() * 40.0 * geo; 
		}
	}

	return color * area / (double)counter;
}

bool Scene::castShadowray(vec3& intersection, vec3& intersectionNormal, vec3& randLightPoint) const {

	// if(intersection.z + EPSILON > randLightPoint.z) return false;

	//SHADOW BIAS
	vec3 startPoint = intersection + EPSILON * intersectionNormal;
	Ray r{intersection, glm::normalize(randLightPoint - intersection)};
	Ray rBias{startPoint, glm::normalize(randLightPoint - startPoint)};

	
	vec3 closestTriangle{1000.0f, 0.0f, 0.0f};
	vec3 closestSphere{1000.0f, 0.0f, 0.0f};

	Triangle hit = checkTriangleIntersections(r, closestTriangle, intersectionNormal);
	Sphere sp = checkSphereIntersections(rBias, closestSphere, intersectionNormal);
	
	double distToLight = glm::distance(randLightPoint, intersection);
	double distToTriangle = glm::distance(closestTriangle, intersection);
	double distToSphere = glm::distance(closestSphere, startPoint);


	if(distToTriangle + EPSILON < distToLight ) return false;
	if(distToSphere + EPSILON < distToLight && glm::abs(distToSphere) > EPSILON) return false;
	return true;
	
}

void Scene::drawRoom() {

	//VERTEXES

	//
	//			p3-----p5
	// 	p1-				 		-p6
	//			p2-----p4
	//
	vec3 p1_up{-3.0f, 0.0f, 5.0f}, p1_down{-3.0f, 0.0f, -5.0f};
	vec3 p2_up{0.0f, -6.0f, 5.0f}, p2_down{0.0f, -6.0f, -5.0f};
	vec3 p3_up{0.0f, 6.0f, 5.0f}, p3_down{0.0f, 6.0f, -5.0f};
	vec3 p4_up{10.0f, -6.0f, 5.0f}, p4_down{10.0f, -6.0f, -5.0f};
	vec3 p5_up{10.0f, 6.0f, 5.0f}, p5_down{10.0f, 6.0f, -5.0f};
	vec3 p6_up{13.0f, 0.0f, 5.0f}, p6_down{13.0f, 0.0f, -5.0f};

	//Colors
	Material WhiteLamb{ColorDbl{1.0f, 1.0f, 1.0f}, LAMBERTIAN};
	Material RedLamb{ColorDbl{1.0f, 0.0f, 0.0f}, LAMBERTIAN};
	Material GreenLamb{ColorDbl{0.0f, 1.0f, 0.0f}, LAMBERTIAN};
	Material BlueLamb{ColorDbl{0.0f, 0.0f, 1.0f}, LAMBERTIAN};
	Material YellowLamb{ColorDbl{1.0f, 1.0f, 0.0f}, LAMBERTIAN};
	Material TealLamb{ColorDbl{0.0f, 0.5f, 0.5f}, PERFECT_REFL};
	Material PurpleLamb{ColorDbl{1.0f, 0.0f, 1.0f}, LAMBERTIAN};
	Material GrayLamb{ColorDbl{0.7f, 0.7f, 0.7f}, LAMBERTIAN};
	Material Teal{ColorDbl{0.0f, 0.5f, 0.5f}, LAMBERTIAN};


	Material YellowPerf{ColorDbl{1.0f, 1.0f, 0.0f}, PERFECT_REFL};
	Material RedPerf{ColorDbl{1.0f, 0.0f, 0.0f}, PERFECT_REFL};
	//ROOF
	//((-3, 0, 5))
	//(0, 6 , 5)
	//(0, -6, 5)
	room.push_back(Triangle{p1_up, p3_up, p2_up, GrayLamb});

	//(0, -6, 5)
	//(0, 6, 5)
	//(10, 6, 5)
	room.push_back(Triangle{p2_up, p3_up, p5_up, GrayLamb});	
	
	//(0, -6, 5)
	//(10, 6, 5)
	//(10, -6, 5)
	room.push_back(Triangle{p2_up, p5_up, p4_up, GrayLamb});

	//(10, 6, 5)
	//(13, 0, 5)
	//(10, -6, 5)		
	room.push_back(Triangle{p5_up, p6_up, p4_up, GrayLamb});

	//FLOOR
	//((-3, 0, -5))
	//(0, -6 , -5)
	//(0, 6, -5)
	room.push_back(Triangle{p1_down, p2_down, p3_down, WhiteLamb});
	
	//(0, -6, -5)
	//(10, 6, -5)
	//(0, 6, -5)
	room.push_back(Triangle{p2_down, p5_down, p3_down, WhiteLamb});

	//(0, -6, -5)
	//(10, -6, -5)
	//(10, 6, -5)
	room.push_back(Triangle{p2_down, p4_down, p5_down, WhiteLamb});

	//(10, -6, -5)
	//(13, 0, -5)
	//(10, 6 , -5)
	room.push_back(Triangle{p4_down, p6_down, p5_down, WhiteLamb});

	//Negativ xy 1
	//(-3, 0, 5)
	//(0, -6, -5)
	//(-3, 0, -5)
	room.push_back(Triangle{p1_up, p2_down, p1_down, RedLamb});

	//Negativ xy 2
	//(-3, 0, 5)
	//(0, -6, 5)
	//(0, -6, -5)
	room.push_back(Triangle{p1_up, p2_up, p2_down, RedLamb});


	//Negativ x pos y 1
	//(-3, 0, 5)
	//(-3, 0, -5)
	//(0, 6, -5)
	room.push_back(Triangle{p1_up, p1_down, p3_down, GreenLamb});

	//Negativ x pos y 2
	//(-3, 0, 5)
	//(0, 6, -5)
	//(0, 6, 5)
	room.push_back(Triangle{p1_up, p3_down, p3_up, GreenLamb});

	//Långsida neg Y
	//(10, -6, 5)
	//(0, -6, -5)
	//(0, -6, 5)
	room.push_back(Triangle{p4_up, p2_down, p2_up, BlueLamb});

	//(10, -6, 5)
	//(10, -6, -5)
	//(0, -6, -5)
	room.push_back(Triangle{p4_up, p4_down, p2_down, BlueLamb});

	//Långsida neg Y
	//(10, 6, 5)
	//(0, 6, 5)
	//(0, 6, -5)
	room.push_back(Triangle{p5_up, p3_up, p3_down, YellowLamb});

	//(10, 6, 5)
	//(0, 6, -5)
	//(10, 6, -5)
	room.push_back(Triangle{p5_up, p3_down, p5_down, YellowLamb});

	//Pos x neg y 1
	//(13, 0, 5)
	//(13, 0, -5)
	//(10, -6, -5)
	room.push_back(Triangle{p6_up, p6_down, p4_down, PurpleLamb});

	//Pos x neg y 2
	//(13, 0, 5)
	//(10, -6, -5)
	//(10, -6, 5)
	room.push_back(Triangle{p6_up, p4_down, p4_up, PurpleLamb});

	//pos x pos y 1
	//(13, 0, 5)
	//(10, 6, -5)
	//(13, 0, -5)
	room.push_back(Triangle{p6_up, p5_down, p6_down, TealLamb});

	//pos x pos y 2
	//(13, 0, 5)
	//(10, 6, 5)
	//(10, 6, -5)
	room.push_back(Triangle{p6_up, p5_up, p5_down, TealLamb});

	//Add object Tetrhedron
	vec3 tBotFront{5, 2, -4.5};
	vec3 tBotRight{7, 4, -4.5};
	vec3 tBotLeft{7, 0, -4.5};
	vec3 tTop{7, 2, -2.0};

	// Bottom triangle
	//(10, 2, -3)
	//(11, 1, -3)
	//(11, 3, -3)
	room.push_back(Triangle{tBotFront, tBotLeft, tBotRight, Teal});

	//Left
	//(10, 2, -3)
	//(11, 2, -2)
	//(11, 1, -3)
	room.push_back(Triangle{tBotFront, tTop, tBotLeft, Teal});
	
	//RIGHT
	//(10, 2, -3)
	//(11, 3, -3)
	//(11, 2, -2)
	room.push_back(Triangle{tBotFront, tBotRight, tTop, Teal});
	//BACK
	//(11, 3, -3)
	//(11, 1, -3)
	//(11, 2, -2)s
	room.push_back(Triangle{tBotRight, tBotLeft, tTop, Teal});

	// addSphere(Sphere{vec3{7.0f, -1.5f, 0.0f}, 1.0f, YellowPerf});
	addSphere(Sphere{vec3{5.0f, -3.5f, -1.0f}, 1.0f, RedLamb});
	addLight();
}