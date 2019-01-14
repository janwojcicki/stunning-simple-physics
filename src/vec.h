#pragma once
#include <cmath>

class Vec{
	public:
	float x, y;
	Vec(float _x, float _y): x(_x), y(_y){}
	Vec():x(0),y(0){}

	float dot(const Vec &other){
		return this->x*other.x + this->y*other.y;
	}
	float cross_len(const Vec &b){
   		return (abs(x*b.y - y*b.x));
	}
	float len(){
		return sqrt(x*x+y*y);
	}
	Vec rotate(float phi){
		phi = -phi;
		return Vec(cos(phi)*x - sin(phi)*y, sin(phi)*x + cos(phi)*y);
	}
	void rotate_in_place(float phi){
		phi = -phi;
		float nx, ny;
		nx = cos(phi)*x - sin(phi)*y;
		ny = sin(phi)*x + cos(phi)*y;
		x = nx;
		y = ny;
	}
	Vec operator+(const Vec &other){
		return Vec(this->x+other.x, this->y+other.y);
	}
	Vec operator-(const Vec &other){
		return Vec(this->x-other.x, this->y-other.y);
	}
	Vec operator*(const float &other){
		return Vec(this->x*other, this->y*other);
	}
	void operator+=(const Vec &other){
		this->x+=other.x;
		this->y+=other.y;
	}
	void operator*=(const float &a){
		this->x*=a;
		this->y*=a;
	}
};
