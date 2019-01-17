#include <cmath>
const float EPSILON = 0.0001f;
inline bool almost_equal( float a, float b )
{
  return std::abs( a - b ) <= EPSILON;
}
