#pragma once
#include <cmath>

class Material {
public:
	Material(const ColorDbl& c = ColorDbl{}, const int& t = 0) {
		colorDbl = c;
		type = t;
	}
	
	ColorDbl reflect() const;

	bool isType(const int& t) const { return t == type;}

	ColorDbl getColor() const {return colorDbl;}
	int getType() const {return type;}

private:
	ColorDbl colorDbl;
	int type;

	ColorDbl lambertianReflection() const;
	ColorDbl perfectReflection() const;
};

ColorDbl Material::reflect() const {
	switch(type) {
		case LAMBERTIAN: return lambertianReflection();

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