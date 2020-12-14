#include "../include/Material.hpp"

ColorDbl Material::reflect() const {
	switch(type) {
		case LAMBERTIAN: return lambertianReflection();

		case OREN_NAYAR: return lambertianReflection();

		case PERFECT_REFL: return perfectReflection();

		case TRANSPARENT: return perfectReflection();

		default: return lambertianReflection(); 
	}
}



ColorDbl Material::lambertianReflection() const {
	return colorDbl * 0.8 / M_PI;
}


ColorDbl Material::perfectReflection() const {
	return colorDbl;
}