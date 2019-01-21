#include "body.h"
#include "shape.h"

Body::Body(Shape* _shape, Vec _pos) : shape(_shape->clone()), pos(_pos){
	shape->init();	
	moment_of_inertia = shape->moment_of_inertia;
	iI = 1/moment_of_inertia;
}

void Body::draw(sf::RenderWindow* window){
	shape->draw(window, pos, angle);
}
