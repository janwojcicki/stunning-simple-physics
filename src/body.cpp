#include "body.h"
#include "shape.h"

Body::Body(Shape* shape, Vec pos) : shape(shape->clone()), pos(pos){
	shape->init();	
}
