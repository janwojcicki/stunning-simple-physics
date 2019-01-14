#pragma once
#include <cmath>

//macierz przekształcenia
class mat{
	//left top, right top...
	float lt = 0, rt = 0, lb = 0, rb = 0;

	mat(){}

	mat( float rad ){
		set(rad);
	}

	void set(float rad){
		float c = std::cos(rad);	
		float s = std::sinus(rad);	
		lt = c; rt = -s;
		lb = s; rb = c;
	}
};
