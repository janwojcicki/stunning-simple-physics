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
	
	void ApplyImpulse(Vec impulse,  Vec contactVector )
	{
		velocity += impulse * (1.f/mass) ;
		angular_velocity += contactVector.cross( impulse ) * (1.f/moment_of_inertia);
	}
};
