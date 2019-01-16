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
	
	Body* add(Shape *shape, Vec pos){
		std::cout << "draw";
		Body* b = new Body(shape, pos);
		bodies.push_back(b);
		return b;
	}

	void draw(sf::RenderWindow* window){
		for (uint i = 0; i < bodies.size(); i++){
			bodies[i]->draw(window);
		}	
	}

	void step(float _dt){
		Body* A, * B;	
		std::vector<Contact> contacts;
		for(uint i = 0; i < bodies.size(); i++){
			A = bodies[i];
			for (uint j = i+1; j<bodies.size(); j++){
				B = bodies[j];
				//if((A->pos-B->pos).len() <= A->shape->furthest_point + B->shape->furthest_point){
					//hmm podejrzane, może się przecinają
					
					contacts.push_back(Contact(A, B));
				//}
			}
		}
		for(uint i = 0; i < contacts.size(); i++){
			contacts[i].solve();
			//std::cout << b1 << '\n' << b2 << '\n';
			//std::cout << (b1 < 0.f && b2 < 0.f ? "coll" : "not");
		}
	}
};
