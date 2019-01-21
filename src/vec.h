#pragma once
#include <cmath>
#include <iostream>

class Vec{
	public:
	float x, y;
	Vec(float _x, float _y): x(_x), y(_y){}
	Vec():x(0),y(0){}

	float dot(const Vec &other){
		return this->x*other.x + this->y*other.y;
	}
	void pretty(std::string s){
		std::cout << s << " " << x << " " << y << '\n'	;
	}
	float cross_len(const Vec &b){
   		return (abs(x*b.y - y*b.x));
	}
	float cross(const Vec &b){
   		return ((x*b.y - y*b.x));
	}
	Vec cross(float a){
		return Vec(a*y, -a*x);
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
	Vec normalize(){
		float len = this->len();
		return Vec(x/len, y/len);
	}
	Vec operator+(Vec other){
		return Vec(this->x+other.x, this->y+other.y);
	}
	Vec operator-(const Vec &other){
		return Vec(this->x-other.x, this->y-other.y);
	}
	Vec operator-(){
		return Vec(-x,  -y);
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
