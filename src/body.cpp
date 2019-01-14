#include "body.h"
#include "shape.h"

Body::Body(Shape* shape, Vec pos) : shape(shape->clone()), pos(pos){
	shape->init();	
	moment_of_inertia = shape->moment_of_inertia;
}

void Body::draw(sf::RenderWindow* window){
	shape->draw(window, pos, angle);
}
