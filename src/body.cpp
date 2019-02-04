#include "body.h"
#include "shape.h"

Body::Body(Shape* _shape, Vec _pos) : shape(_shape->clone()), pos(_pos){
	shape->init();	
	I = shape->moment_of_inertia;
	inv_I = 1/I;
}

void Body::draw(sf::RenderWindow* window){
	shape->draw(window, pos, angle);
}
