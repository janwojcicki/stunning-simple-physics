#include <iostream>
#include <vector>
#include <cmath>

class Vec{
	public:
	float x, y;
	Vec(float _x, float _y): x(_x), y(_y){}
	Vec():x(0),y(0){}

	float dot(const Vec &other){
		return this->x*other.x + this->y*other.y;
	}
	float len(){
		return sqrt(x*x+y*y);
	}
	Vec operator+(const Vec &other){
		return Vec(this->x+other.x, this->y+other.y);
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

class Point {
	public:
	Vec pos;
	float distance_from_center_of_mass;
	Point(float _x, float _y): pos(Vec(_x, _y)){}
	Point(): pos(Vec()){}
};

class Shape{
	public:
	std::vector<Point> points;
	Vec velocity;
	Vec acceleration;
	Vec center_of_mass;
	float angular_velocity;
	float angular_acceleration;
	float mass;
	Shape(){}

	void init(){
		for(uint i = 0; i < points.size(); i++)	{
			center_of_mass += points[i].pos;
		}
		center_of_mass *= (1.f/points.size());
		for(uint i = 0; i < points.size(); i++)	{
			points[i].distance_from_center_of_mass = 
				((points[i].pos+center_of_mass)*(0.5f)).len();
		}
	}
};

int side_of_line(Vec a, Vec b, Vec p){
	return sign((p.x-a.x)*(b.y-a.y) -(p.y-a.y)*(b.x-a.x));
}

bool checkIntersection(const Shape &a, const Shape &b){
	int next;
	for (uint i = 0; i < a.points.size(); i++){
		next = (i==a.points.size()-1) ? 0 : i+1;
		

	}
	return true;
}

int main(){
	Shape s1, s2;
	s1.points.push_back(Point(0, 0));
	s1.points.push_back(Point(100, 0));
	s1.points.push_back(Point(100, 100));
	s1.points.push_back(Point(0, 100));
	s1.init();
	s2.points.push_back(Point(50, 50));
	s2.points.push_back(Point(100, 0));
	s2.points.push_back(Point(200, 50));
	s2.points.push_back(Point(100, 100));
	s2.init();

	
	return 0;
}
