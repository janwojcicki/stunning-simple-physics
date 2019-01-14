#pragma once
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "vec.h"

struct Shape;
struct Body;

class World{

	std::vector<Body*> bodies;

	Body* add(Shape *shape, Vec pos);
};
