#pragma once
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "vec.h"
#include "body.h"
#include <iostream>
#include "contact.h"


class World{
	public:
	std::vector<Body*> bodies;
	Vec gravity;
	int iterations = 10;
	
	Body* add(Shape *shape, Vec pos){
		Body* b = new Body(shape, pos);
		bodies.push_back(b);
		return b;
	}

	void draw(sf::RenderWindow* window){
		for (uint i = 0; i < bodies.size(); i++){
			bodies[i]->draw(window);
		}	
	}

	void integrateForces(Body *b, float _dt){
		b->velocity += (b->force * (1.f/b->mass) + gravity) * (_dt*0.5f);
		b->angular_velocity += (b->torque * (1.f/b->moment_of_inertia)) * (_dt*0.5f);
	}
	void integrate_velocity( Body *b, float dt )
	{
		std::cout << "VELO "<<gravity.x << " " << gravity.y << '\n';
		b->pos += b->velocity * dt;
		b->angle.set(b->angle.angle() + b->angular_velocity * dt);
		integrateForces( b, dt );
	}

	void step(float _dt){
		Body* A, * B;	
		std::vector<Contact> contacts;
		for(uint i = 0; i < bodies.size(); i++){
			A = bodies[i];
			std::cout << A->pos.x << " " << A->pos.y << '\n';
			for (uint j = i+1; j<bodies.size(); j++){
				B = bodies[j];
				//if((A->pos-B->pos).len() <= A->shape->furthest_point + B->shape->furthest_point){
					//hmm podejrzane, może się przecinają
					
					contacts.push_back(Contact(A, B, gravity));
					contacts[contacts.size()-1].solve();
				//}
			}
		}
		for(uint i = 0; i < bodies.size(); i++){
			integrateForces(bodies[i], _dt);
		}
		for(uint i = 0; i < contacts.size(); i++){
			contacts[i].init(_dt);	
		}
		for(int j = 0; j < iterations; j++){
			for(uint i = 0; i < contacts.size(); i++){
				contacts[i].apply_impulse();	
			}	
		}

	  	for(uint i = 0; i < bodies.size( ); i++)
			integrate_velocity( bodies[i], _dt );
	
	  	// Correct positions
	  	/*for(uint i = 0; i < contacts.size( ); ++i)
			contacts[i].positional_correction();
	
	  	// Clear all forces
	  	for(uint i = 0; i < bodies.size( ); ++i)
	  	{
			Body *b = bodies[i];
			b->force = Vec( 0, 0 );
			b->torque = 0;
	  	}*/
	}
};
