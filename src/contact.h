#include "body.h"
#include "shape.h"

class Contact{
	public:
	Body* A, *B;

	int contact_count = 0;

	Contact(Body* _a, Body* _b) : A(_a), B(_b){}


	//seperating axis theorem
	//zwraca najmniejszą głebokość zderzenia
	//w przypadku braku kolizji zwróci dodatnie gówno pewnie
	float sat(Body* a, Body* b, uint* f){
		float best_dis = -500000.f;

		Polygon* aa = (Polygon*) a->shape;
		Polygon* bb = (Polygon*) b->shape;

		for(uint i = 0; i < aa->normals.size(); i++){
			Vec norm = a->angle * (-aa->normals[i]);

			norm = b->angle.transpose() * norm;

			Vec extr = bb->get_extreme_point( -norm );

			Vec v1 = a->angle * aa->points[i];
			v1 += a->pos;
			v1 += -b->pos;
			v1 = b->angle.transpose() * v1;

			if(norm.dot(extr - v1) > best_dis){
				best_dis = norm.dot(extr-v1);
				*f = i;
			}

			//Vec v =	(b->angle * bb->points[i] + a->pos);
		}
		return best_dis;
	}

	void solve(){
		uint faceA,faceB;
		float deepA = sat(A, B, &faceA);
		if(deepA >= 0.f) //nie ma kolizji
			return;
		float deepB = sat(B, A, &faceB);
		if(deepB >= 0.f) //nie ma kolizji
			return;
	}
};
