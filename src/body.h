#pragma once
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "vec.h"
#include "mat.h"

class Shape;

class Body{
	public:
	Shape* shape;
	Vec pos;
	Vec velocity;
	Vec force;

	float angular_velocity = 0;
	float torque = 0;
	mat angle;
	
	float moment_of_inertia = 0;

	float static_friction = 0.5f;
	float dynamic_friction = 0.3f;
	float restitution = 1.f;

	
	Body(Shape* shape, Vec pos);

	void draw(sf::RenderWindow* window);
};
