#include "../include/Camera.hpp"

ColorDbl Camera::traceRay(const Scene& s, Ray& r, int pass) {
	vec3 closestTriangle{1000.0, 0.0, 0.0};
	vec3 closestSphere{1000.0, 0.0, 0.0};
	
	vec3 intersectionNormalT{};
	vec3 intersectionNormalS{};
	Triangle hit = s.checkTriangleIntersections(r, closestTriangle, intersectionNormalT);
	Sphere sp = s.checkSphereIntersections(r, closestSphere, intersectionNormalS);

	if(glm::distance(r.getStartPoint(), closestTriangle) < glm::distance(r.getStartPoint(), closestSphere)) {
		Material m = hit.getMaterial();
		if(m.isType(LIGHTSOURCE)) return m.getColor();
		else if(m.isType(PERFECT_REFL) || m.isType(TRANSPARENT)) {
			if(pass == MAX_PASSES) return m.reflect();
			vec3 newDirection = glm::reflect(r.getDirection(), glm::normalize(hit.getNormal()));
			Ray newRay{closestTriangle, newDirection};
				return traceRay(s, newRay, 0);
		}
	}
	else { //sphere stuff
		Material m = sp.getMaterial();
		if(m.isType(LIGHTSOURCE)) return m.getColor();
		else if(m.isType(PERFECT_REFL)) {
			if(pass == MAX_PASSES) return m.reflect();
			vec3 newDirection = glm::reflect(r.getDirection(), glm::normalize(intersectionNormalS));
			Ray newRay{closestSphere + intersectionNormalS * EPSILON, newDirection};
				return traceRay(s, newRay, 0);
		}
		else if(m.isType(TRANSPARENT)) {

			if(pass == MAX_PASSES) return m.reflect();

			float n1 = 1.0; //AIR
			float n2 = 1.5; //GLASSs
			// float n2 = 1.745; // // Flint Glass  
			vec3 n = intersectionNormalS;
			double cosIn = glm::clamp(-1.0f, 1.0f, glm::dot(r.getDirection(), n));
			
			if (cosIn < 0) {
				cosIn = -cosIn;
			}
			else {
				std::swap(n1, n2);
				n = -n;
			}
			//SCHLICS
			double R0 = pow((n1 - n2) / (n1 + n2), 2);
			double R =  R0 + (1 - R0) * pow(1 - cosIn, 5);
			double T = 1 - R; //Transmission
			

			float ratio = n1/n2;
			float refractContr = 1.0 - ratio * ratio * (1.0 - cosIn * cosIn);

			vec3 refractedDir = refractContr < 0.0f ? vec3{} : (float)ratio * r.getDirection() + (float)(ratio * cosIn - sqrt(refractContr)) * n;
			Ray ref{closestSphere + EPSILON * refractedDir, refractedDir};
			ColorDbl refractedLight = traceRay(s, ref, pass + 1);

			vec3 newDirection = glm::reflect(r.getDirection(), glm::normalize(intersectionNormalS));
			Ray newRay{closestSphere + intersectionNormalS * EPSILON, newDirection};
			ColorDbl reflectedLight = traceRay(s, newRay, pass + 1);

			return refractedLight * T + reflectedLight * R;
			// std::cout << R << "\n";
			// return refractedLight * T;
			// return reflectedLight * R;
			
		}
		// return sp.getMaterial().getColor();
	}

	vec3 intersection = glm::distance(closestSphere, r.getStartPoint()) < glm::distance(closestTriangle, r.getStartPoint())  ? closestSphere : closestTriangle;
	vec3 intersectionNormal = glm::distance(closestSphere, r.getStartPoint()) < glm::distance(closestTriangle, r.getStartPoint())  ? intersectionNormalS : intersectionNormalT;
	// bool sphereHit = closestSphere.x < closestTriangle.x ? true : false;

	ColorDbl directLight{};
	Material m = glm::distance(closestSphere, r.getStartPoint()) < glm::distance(closestTriangle, r.getStartPoint())  ? sp.getMaterial() : hit.getMaterial();
	directLight = m.reflect();



	ColorDbl light = s.calcLight(intersection, intersectionNormal, -r.getDirection(), m.getType());

	directLight *= light;
	//Direct light done

	//Compute indirect

	// Determine number of ray from intersection point. Hemisphere samples
	// const int indirectSamples = 3;
	ColorDbl indirect{};

	int counter = 0;

	do {
		indirect += sampleIndirectRay(s, intersection, intersectionNormal);
		++counter;
	} while (uniformRand() < 0.8 && counter < 20);
	// for(int i = 0; i < indirectSamples; ++i) {
	// }
	indirect *= (1.0 / (double)counter * M_PI);
	
	
	ColorDbl color =  directLight + indirect * 0.0375; //+ asdasd
	// ColorDbl color = directLight;
	return color; // + indirect
}

ColorDbl Camera::sampleIndirectRay(const Scene& s, const vec3& point, const vec3& normal) {
	ColorDbl color{0.0, 0.0, 0.0};
	
	float inclAngle = (float)acos(sqrt(uniformRand()));
	float azimAngle = (float)2 * M_PI * uniformRand();
	
	vec3 randDir = normal;

	vec3 helper = normal + vec3{1, 1, 1};
	vec3 tangent = glm::normalize(glm::cross(normal, helper));
	
	randDir = glm::normalize(glm::rotate(
		randDir,
		inclAngle,
		tangent
	));

	randDir = glm::normalize(glm::rotate(
		randDir,
		azimAngle,
		normal
	));

	Ray reflectedRay{point, randDir};
	Ray reflectedRayBias{point + EPSILON * randDir, randDir};
	//Check intersections
	//color from intersection :)
	vec3 closestTriangle{1000.0, 0.0, 0.0};
	vec3 closestSphere{1000.0, 0.0, 0.0};
	
	vec3 intersectionNormalT{};
	vec3 intersectionNormalS{};
	Triangle hit = s.checkTriangleIntersections(reflectedRay, closestTriangle, intersectionNormalT);
	Sphere sp = s.checkSphereIntersections(reflectedRayBias, closestSphere, intersectionNormalS);

	Material m = glm::distance(closestSphere, reflectedRay.getStartPoint()) < glm::distance(closestTriangle, reflectedRay.getStartPoint())  ? sp.getMaterial() : hit.getMaterial();
	// vec3 p =	glm::distance(closestSphere, reflectedRay.getStartPoint()) < glm::distance(closestTriangle, reflectedRay.getStartPoint()) ? closestSphere : closestTriangle;
	// vec3 in = glm::distance(closestSphere, reflectedRay.getStartPoint()) < glm::distance(closestTriangle, reflectedRay.getStartPoint()) ? intersectionNormalS : intersectionNormalT;

	// double cosAlpha = glm::dot(in, -normal);

	if(m.isType(LIGHTSOURCE)) return m.getColor();
	else if(m.isType(PERFECT_REFL) || m.isType(TRANSPARENT)) return color;

	color = m.reflect();

	return color;

}


void Camera::createPixels() {

	for (int i = 0; i < HEIGHT; ++i) {
		for (int j = 0; j < WIDTH; ++j) {
			pixels[j + i * HEIGHT] = Pixel{topLeft + vec3{0.0f, -0.0025 * j, -0.0025 * i}};
		}
	}
}

void Camera::render(const Scene& s) {
	const int SUBPIXELS = 2; //In each direction

	const float subSideLength = Pixel::getSideLength() / SUBPIXELS; 
	std::cout << "\nRENDER\n";

		// Parallellize the for loop with openMP.
	#pragma omp parallel for collapse(2) num_threads(4)
	for (int i = 0; i < HEIGHT; ++i) 
	{
		for (int j = 0; j < WIDTH; ++j) {
			ColorDbl color{0.0, 0.0, 0.0};
			
			// Loop over subpixels
			for(int k = 0; k < SUBPIXELS; ++k) {
				for(int m = 0; m < SUBPIXELS; ++m) {
					
					//Samples per pixel
					for(int n = 0; n < spp; ++n) {
						double y = randMinMax(k * subSideLength, subSideLength + k * subSideLength);
						double z = randMinMax(m * subSideLength, subSideLength + m * subSideLength);
						vec3 point{0.0, y, z};
						Ray r{CameraPos, glm::normalize(pixels[j + i * HEIGHT].getPosition() - point - CameraPos)};
						color += traceRay(s, r, 0);
					}

				}
			}

			pixels[j + i * HEIGHT].setColor(color);
			if(color.x > maxValue) maxValue = color.x;
			if(color.y > maxValue) maxValue = color.y;
			if(color.z > maxValue) maxValue = color.z;
		
		}

	}
	std::cout << "\nPIXELS DONE \n";

	createImage();
}

void Camera::createImage() {

	//Open file
	FILE *f = fopen("image.ppm", "wb"); //b = binary mode
	fprintf(f, "P6\n %i %i 255\n", WIDTH, HEIGHT);
	std::cout << "\nFile open\n";

	for (int i = 0; i < HEIGHT; ++i) {
		for (int j = 0; j < WIDTH; ++j) {
			vec3 color = pixels[j + i * HEIGHT].getColor();

			//Normera till 255
			fputc((int) (sqrt((color.x/maxValue)) * 255.0), f);
			fputc((int) (sqrt((color.y/maxValue)) * 255.0), f);
			fputc((int) (sqrt((color.z/maxValue)) * 255.0), f);
		}
	}
	fclose(f);
	std::cout << "File closed\n\n";
}