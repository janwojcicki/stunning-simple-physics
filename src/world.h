#pragma once
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "vec.h"
#include "body.h"
#include <iostream>


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
};
