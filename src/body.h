#pragma once
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "vec.h"

struct Shape;

class Body{
	public:
	Shape* shape;
	Vec pos;
	

	Body(Shape* shape, Vec pos) ;
};
