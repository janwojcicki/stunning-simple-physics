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
	float mass = 10.f;
	float inv_mass = 0.1f;
	Vec velocity;
	Vec force;
	bool stat = false;

	float angular_velocity = 0;
	float torque = 0;
	mat angle;

	float I = 0; // moment bezwladnosci
	float inv_I = 0;

	float static_friction = 0.1f;
	float dynamic_friction = 0.1f;
	float restitution = 0.000001f;


	Body(Shape* shape, Vec pos);

	void draw(sf::RenderWindow* window);
	
	void ApplyImpulse(Vec impulse,  Vec contactVector )
	{

		velocity += impulse * inv_mass ;
		angular_velocity += contactVector.cross( impulse ) * inv_I;
	}
};
