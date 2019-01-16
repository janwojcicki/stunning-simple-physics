#pragma once
#include <cmath>
#include <iostream>
#include "vec.h"

//macierz przekształcenia
class mat{
	public:
	//left top, right top...
	float lt = 1, rt = 0, lb = 0, rb = 1;

	mat(){}

	mat( float rad ){
		set(rad);
	}
	mat(float a, float b, float c, float d):lt(a),rt(b),lb(c),rb(d){}

	void set(float rad){
		float c = std::cos(rad);	
		float s = std::sin(rad);
		lt = c; rt = -s;
		lb = s; rb = c;
	}

	mat transpose(){
		return mat(lt, lb, rt, rb);	
	}

	const Vec operator*(const Vec& other){
		return Vec(lt*other.x+rt*other.y, lb*other.x + rb*other.y);
	}

	float angle(){
		return atan2(lb, lt);
	}
};
