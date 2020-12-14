#pragma once
#include <cmath>
#include "typeDefinitions.hpp"
#include "utils.hpp"

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