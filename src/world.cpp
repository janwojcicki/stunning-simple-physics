#include "body.h"
#include "shape.h"
#include "world.h"

Body* World::add(Shape *shape, Vec pos){
	Body* b = new Body(shape, pos);

	

	bodies.push_back(b);
	return b;
}
