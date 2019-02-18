#include <cmath>
#include <iostream>
#include "vec.h"
const float EPSILON = 0.0001f;
inline bool almost_equal( float a, float b )
{
  return std::abs( a - b ) <= EPSILON;
}

void fuck(float f, std::string s){
	std::cout << s << " " << f << std::endl;
}
void fuck(Vec v, std::string s){
	std::cout << s << " " << v.x << " " << v.y << std::endl;
}
