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
	
	// void addSphere(Sphere _s) {
	// 	s = _s;
	// }

	void addLight() {
		
		std::vector<Triangle> lightTriangles = l.getLightTriangles();
		
		for(auto t : lightTriangles) {
			room.push_back(t);
		}
	}

	ColorDbl calcLight(vec3& intersection, vec3& intersectionNormal) const;
	bool castShadowray(vec3& intersection, vec3& intersectionNormal, vec3& randLightPoint) const;
	bool caustic(vec3& p, vec3& in, Ray& r, int pass = 0) const;
	
private:
	std::vector<Triangle> room;
	std::vector<Sphere> spheres;
	// Sphere s;
	Light l{};
	
};

Triangle Scene::checkTriangleIntersections(Ray &r, vec3& closestTriangle, vec3& intersectionNormal) const{
	// Loop through all triangles and check if we have a hi 
	// For now when no object in the scene, intersection with only wall or floor or roof
	Triangle t{};
	for(auto i : room) {
		vec3 intersection{}, intersectionN{};
		if(!i.rayIntersection(r, intersection, intersectionN)) continue;
		if(glm::distance(r.getStartPoint(), intersection) < glm::distance(r.getStartPoint(), closestTriangle)) {
			t = i;
			closestTriangle = intersection;
			intersectionNormal = intersectionN;
		}	
	}
	return t;
}

Sphere Scene::checkSphereIntersections(Ray &r, vec3& closestSphere, vec3& intersectionNormal) const {
	Sphere _s{};
	for(auto s : spheres) {
		vec3 intersection{}, intersectionN{};
		
		if(!s.rayIntersection(r, intersection, intersectionN)) continue;
		if(glm::distance(r.getStartPoint(), intersection) < glm::distance(r.getStartPoint(), closestSphere)) {
			_s = s;
			closestSphere = intersection;
			intersectionNormal = intersectionN;
		}
	}
	return _s;
}

ColorDbl Scene::calcLight(vec3& intersection, vec3& intersectionNormal) const {
	ColorDbl color = ColorDbl{0.0, 0.0, 0.0};
	double area = 0;
	int counter = 0;

	for(Triangle LT : l.getLightTriangles()) {
		area += LT.calcArea();
		for(int i = 0; i < 3; ++i){

			vec3 randLightPoint = LT.getRandomPoint() ;
			// vec3 randLightPoint = l.getLightCenter();
			vec3 iN = intersectionNormal;
			vec3 it = intersection;
			++counter;
			if(!castShadowray(it, iN, randLightPoint)) continue;
			
			double cosAlpha = glm::dot(intersectionNormal, glm::normalize(randLightPoint - intersection));
			double cosBeta = glm::dot(LT.getNormal(), glm::normalize(intersection - randLightPoint));
			// double angle = glm::dot(glm::normalize(intersectionNormal), glm::normalize(randLightPoint - intersection)) ;
			// double beta = glm::clamp((double) glm::dot(LT.getNormal(), glm::normalize(intersection - randLightPoint)), 0.0, 1.0);
			if(cosAlpha < 0) cosAlpha = 0;
			if(cosBeta < 0) cosBeta = 0;
			double geo = cosBeta * cosAlpha/ glm::pow(glm::distance(intersection, randLightPoint), 2.0);
			color += l.getColor() * geo * 50.0;
		}
	}
	// return color;
	return color * area / (double)counter;
}

bool Scene::castShadowray(vec3& intersection, vec3& intersectionNormal, vec3& randLightPoint) const {

	//SHADOW BIAS
	vec3 startPoint = intersection + EPSILON * intersectionNormal;
	Ray r{intersection, glm::normalize(randLightPoint - intersection)};
	Ray rBias{startPoint, glm::normalize(randLightPoint - startPoint)};

	
	vec3 closestTriangle{1000.0f, 0.0f, 0.0f};
	vec3 closestSphere{1000.0f, 0.0f, 0.0f};
	vec3 iNT{};
	vec3 iNS{};

	Triangle t  = checkTriangleIntersections(r, closestTriangle, iNT);
	Sphere s = checkSphereIntersections(rBias, closestSphere, iNS);
	
	double distToLight = glm::distance(randLightPoint, intersection);
	double distToTriangle = glm::distance(closestTriangle, intersection);
	double distToSphere = glm::distance(closestSphere, startPoint);

	Material M = distToSphere < distToTriangle ? s.getMaterial() : t.getMaterial();
	vec3 point = distToSphere < distToTriangle ? closestSphere : closestTriangle;
	vec3 in = distToSphere < distToTriangle ? iNS : iNT;

	// if(M.isType(TRANSPARENT)) {
	// 	return caustic(point, in, r);
	// }


	if(distToTriangle + EPSILON < distToLight ) return false;
	if(distToSphere + EPSILON < distToLight && glm::abs(distToSphere) > EPSILON) return false;
	return true;
}

bool Scene::caustic(vec3& p, vec3& in, Ray& r, int pass) const {
	if(pass == 6) return false;
	float n1 = 1.0; //AIR
	// float n2 = 1.5; //GLASS
	float n2 = 1.5; //DIAMOND

	double cosIn = glm::clamp(-1.0f, 1.0f, glm::dot(r.getDirection(), in));
	
	if (cosIn < 0) {
		cosIn = -cosIn;
	}
	else {
		std::swap(n1, n2);
		in = -in;
	}
	//SCHLICS alt FRESH (TO FRKN HEAVY)
	double R0 = pow((n1 - n2) / (n1 + n2), 2);
	double R =  R0 + (1 - R0) * pow(1 - cosIn, 5);
	double T = 1 - R; //Transmission
	

	float ratio = n1/n2;
	float refractContr = 1.0 - ratio * ratio * (1.0 - cosIn * cosIn);

	vec3 refractedDir = refractContr < 0.0f ? vec3{} : (float)ratio * r.getDirection() + (float)(ratio * cosIn - sqrt(refractContr)) * in;
	Ray ref{p + EPSILON * refractedDir, refractedDir};


	vec3 closestTriangle{1000.0f, 0.0f, 0.0f};
	vec3 closestSphere{1000.0f, 0.0f, 0.0f};
	vec3 iNT{};
	vec3 iNS{};
	Triangle refHitT = checkTriangleIntersections(ref, closestTriangle, iNT);
	Sphere refHitS = checkSphereIntersections(ref, closestSphere, iNS);

	double distToTriangle = glm::distance(closestTriangle, p);
	double distToSphere = glm::distance(closestSphere, p);

	Material M = distToSphere < distToTriangle ? refHitS.getMaterial() : refHitT.getMaterial();
	vec3 point = distToSphere < distToTriangle ? closestSphere : closestTriangle;
	vec3 intersection = distToSphere < distToTriangle ? iNS : iNT;

	if(M.isType(TRANSPARENT)) {
		return caustic(point, intersection, ref, pass + 1);
	}
	if(M.isType(LIGHTSOURCE)) return true;

	// vec3 newDirection = glm::reflect(r.getDirection(), glm::normalize(intersectionNormalS));
	// Ray newRay{closestSphere + intersectionNormalS * EPSILON, newDirection};
	// ColorDbl reflectedLight = traceRay(s, newRay, pass + 1);

	return false;

}


void Scene::drawRoom() {

	//VERTEXES

	//
	//			p2-----p4
	// 	p1-				 		-p6
	//			p3-----p5
	//
	vec3 p1_up{-3.0f, 0.0f, 5.0f}, p1_down{-3.0f, 0.0f, -5.0f};
	vec3 p2_up{0.0f, 6.0f, 5.0f}, p2_down{0.0f, 6.0f, -5.0f};
	vec3 p3_up{0.0f, -6.0f, 5.0f}, p3_down{0.0f, -6.0f, -5.0f};
	vec3 p4_up{10.0f, 6.0f, 5.0f}, p4_down{10.0f, 6.0f, -5.0f};
	vec3 p5_up{10.0f, -6.0f, 5.0f}, p5_down{10.0f, -6.0f, -5.0f};
	vec3 p6_up{13.0f, 0.0f, 5.0f}, p6_down{13.0f, 0.0f, -5.0f};

	//Colors
	Material WhiteLamb{ColorDbl{1.0f, 1.0f, 1.0f}, LAMBERTIAN};
	Material RedLamb{ColorDbl{1.0f, 0.0f, 0.0f}, LAMBERTIAN};
	Material GreenLamb{ColorDbl{0.0f, 1.0f, 0.0f}, LAMBERTIAN};
	Material BlueLamb{ColorDbl{0.0f, 0.0f, 1.0f}, LAMBERTIAN};
	Material YellowLamb{ColorDbl{1.0f, 1.0f, 0.0f}, LAMBERTIAN};
	Material TealLamb{ColorDbl{0.0f, 1.0f, 1.0f}, LAMBERTIAN};
	Material PurpleLamb{ColorDbl{1.0f, 0.0f, 1.0f}, LAMBERTIAN};
	Material GrayLamb{ColorDbl{0.7f, 0.7f, 0.7f}, LAMBERTIAN};



	Material YellowPerf{ColorDbl{1.0f, 1.0f, 0.0f}, PERFECT_REFL};
	Material RedPerf{ColorDbl{1.0f, 0.0f, 0.0f}, PERFECT_REFL};
	Material TealPerf{ColorDbl{0.0f, 0.5f, 0.5f}, PERFECT_REFL};

	Material WhiteTransp{ColorDbl{1.0f, 1.0f, 1.0f}, TRANSPARENT};
	Material RedTransp{ColorDbl{1.0f, 0.0f, 0.0f}, TRANSPARENT};
	//ROOF
	//((-3, 0, 5))
	//(0, 6 , 5)

	
	//(0, -6, 5)
	room.push_back(Triangle{p1_up, p2_up, p3_up, GrayLamb}); //Normal Negativ z
	// std::cout << glm::cross(p1_up-p2_up, p3_up-p2_ up).z;
	//(0, -6, 5)
	//(0, 6, 5)
	//(10, 6, 5)
	room.push_back(Triangle{p2_up, p5_up, p3_up, GrayLamb});	//Normal Negativ z
	
	//(0, -6, 5)
	//(10, 6, 5)
	//(10, -6, 5)
	room.push_back(Triangle{p2_up, p4_up, p5_up, GrayLamb}); //Normal negativ z

	//(10, 6, 5)
	//(13, 0, 5)
	//(10, -6, 5)
	room.push_back(Triangle{p5_up, p4_up, p6_up, GrayLamb}); //Normal negativ z

	//FLOOR
	//((-3, 0, -5))
	//(0, -6 , -5)
	//(0, 6, -5)
	room.push_back(Triangle{p1_down, p3_down, p2_down, WhiteLamb}); //Normal positiv z
	
	//(0, -6, -5)
	//(10, 6, -5)
	//(0, 6, -5)
	room.push_back(Triangle{p2_down, p3_down, p5_down, WhiteLamb}); //Normal positiv z

	//(0, -6, -5)
	//(10, -6, -5)
	//(10, 6, -5)
	room.push_back(Triangle{p2_down, p5_down, p4_down, WhiteLamb}); //Normal pos z

	//(10, -6, -5)
	//(13, 0, -5)
	//(10, 6 , -5)
	room.push_back(Triangle{p4_down, p5_down, p6_down, WhiteLamb}); //Normal pos z

	//POS Y  NEG x 1
	//(-3, 0, 5)
	//(0, -6, -5)
	//(-3, 0, -5)
	room.push_back(Triangle{p1_up, p1_down, p2_down, YellowLamb});

	//POS Y  NEG X 2
	//(-3, 0, 5)
	//(0, -6, 5)
	//(0, -6, -5)
	room.push_back(Triangle{p1_up, p2_down, p2_up, YellowLamb});


	//Negativ x NEG y 1
	//(-3, 0, 5)
	//(-3, 0, -5)
	//(0, 6, -5)
	room.push_back(Triangle{p1_up, p3_down, p1_down, GreenLamb});

	//Negativ x NEG y 2
	//(-3, 0, 5)
	//(0, 6, -5)
	//(0, 6, 5)
	room.push_back(Triangle{p1_up, p3_up, p3_down, GreenLamb});

	//Långsida POS Y
	//(10, 6, 5)
	//(0, 6, -5)
	//(0, 6, 5)
	room.push_back(Triangle{p4_up, p2_up, p2_down, RedLamb});

	//(10, -6, 5)
	//(10, -6, -5)
	//(0, -6, -5)
	room.push_back(Triangle{p4_up, p2_down, p4_down, RedLamb});

	//Långsida neg Y
	//(10, -6, 5)
	//(0, -6, 5)
	//(0, -6, -5)
	room.push_back(Triangle{p5_up, p3_down, p3_up,GreenLamb});

	//(10, 6, 5)
	//(0, 6, -5)
	//(10, 6, -5)
	room.push_back(Triangle{p5_up, p5_down, p3_down, GreenLamb});

	//Pos x pos y 1
	//(13, 0, 5)
	//(13, 0, -5)
	//(10, 6, -5)
	room.push_back(Triangle{p6_up, p4_down, p6_down, WhiteLamb});

	//Pos x pos y 2
	//(13, 0, 5) 
	//(10, -6, -5)
	//(10, -6, 5)
	room.push_back(Triangle{p6_up, p4_up, p4_down, WhiteLamb});

	//pos x neg y 1
	//(13, 0, 5)
	//(10, 6, -5)
	//(13, 0, -5)
	room.push_back(Triangle{p6_up, p6_down, p5_down, YellowPerf});

	//pos x pos y 2
	//(13, 0, 5)
	//(10, 6, 5)
	//(10, 6, -5)
	room.push_back(Triangle{p6_up, p5_down, p5_up, YellowPerf});

	//Add object Tetrhedron
	vec3 tBotFront{7, -3, 0};
	vec3 tBotRight{8, -5, 0};
	vec3 tBotLeft{8 , -1, 0};
	vec3 tTop{8, -3, 2.5};

	// Bottom triangle
	//(10, 2, -3)
	//(11, 1, -3)
	//(11, 3, -3)
	room.push_back(Triangle{tBotFront, tBotLeft, tBotRight, GrayLamb});

	//Left
	//(10, 2, -3)
	//(11, 2, -2)
	//(11, 1, -3)
	room.push_back(Triangle{tBotFront, tTop, tBotLeft, GrayLamb});
	//
	//RIGHT
	//(10, 2, -3)
	//(11, 3, -3)
	//(11, 2, -2)
	room.push_back(Triangle{tBotFront, tBotRight, tTop, GrayLamb});


	//BACK
	//(11, 3, -3)
	//(11, 1, -3)
	//(11, 2, -2)s
	room.push_back(Triangle{tBotRight, tBotLeft, tTop, GrayLamb});

		//Rätblock
	// vec3 rp1{9, 2, -4.5};
	// vec3 rp2{10, 1, -4.5};
	// vec3 rp3{9, 2, 1};
	// vec3 rp4{10, 1, 1};
	// vec3 rp5{10, 3, 1};
	// vec3 rp6{11, 2, 1};
	// vec3 rp7{10, 3, -4.5};
	// vec3 rp8{11, 2, -4.5};

	// //fram
	// room.push_back(Triangle{rp1, rp4, rp3, TealPerf});
	// room.push_back(Triangle{rp1, rp2, rp4, TealPerf});

	// //vänster
	// room.push_back(Triangle{rp7, rp1, rp3, TealPerf});
	// room.push_back(Triangle{rp7, rp3, rp5, TealPerf});

	// //höger
	// room.push_back(Triangle{rp2, rp6, rp4, TealPerf});
	// room.push_back(Triangle{rp2, rp8, rp6, TealPerf});

	// //top
	// room.push_back(Triangle{rp3, rp6, rp5, TealPerf});
	// room.push_back(Triangle{rp3, rp4, rp6, TealPerf});

	// addSphere(Sphere{vec3{7.0f, -1.5f, 0.0f}, 1.0f, YellowPerf});
	spheres.push_back(Sphere{vec3{6.0f, -2.5f, -2.5f}, 1.5f, WhiteLamb});
	spheres.push_back(Sphere{vec3{6.0f, 3.0f, -2.5f}, 1.5f, RedTransp});
	// spheres.push_back(Sphere{vec3{7.0f, 3.0f, 2.0f}, 1.5f, RedTransp});
	spheres.push_back(Sphere{vec3{9.0f, 3.0f, 3.0f}, 1.5f, YellowPerf});
	// addSphere(Sphere{vec3{5.0f, -1.0f, 0.0f}, 1.5f, WhiteTransp});
	addLight();
}