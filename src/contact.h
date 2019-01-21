#include "body.h"
#include "shape.h"
#include <assert.h>
#include "consts.h"

class Contact{
	public:
	Body* A, *B;
	Vec normal;
	float penetration;
	Vec contacts[2];
	uint contact_count = 0;

	float avg_rest;
	float s_friction;
	float d_friction;
	Vec gravity;



	Contact(Body* _a, Body* _b, Vec _gravity) : A(_a), B(_b), gravity(_gravity){}


	//seperating axis theorem
	//zwraca najmniejszą głebokość zderzenia
	//w przypadku braku kolizji zwróci dodatnie gówno pewnie
	float sat(Body* a, Body* b, uint* f){
		float best_dis = -500000.f;

		Polygon* aa = (Polygon*) a->shape;
		Polygon* bb = (Polygon*) b->shape;

		for(uint i = 0; i < aa->normals.size(); i++){
			Vec n = aa->normals[i];
			Vec nw = a->angle * n;

			n = b->angle.transpose() * nw;

			Vec extr = bb->get_extreme_point( -n );

			Vec v1 = a->angle * aa->points[i];
			v1 += a->pos;
			v1 += -b->pos;
			v1 = b->angle.transpose() * v1;

			if(n.dot(extr - v1) > best_dis){
				best_dis = n.dot(extr-v1);
				*f = i;
			}

			//Vec v =	(b->angle * bb->points[i] + a->pos);
		}
		return best_dis;
	}


	void find_incident_face( Vec *v, Body *RefPoly, Body *IncPoly, uint referenceIndex )
	{
		Vec referenceNormal = RefPoly->shape->normals[referenceIndex];

		referenceNormal = RefPoly->angle * referenceNormal; 
		referenceNormal = IncPoly->angle.transpose( ) * referenceNormal; 


		int incidentFace = 0;
		float minDot = 5000000;
		for(uint i = 0; i < IncPoly->shape->normals.size(); ++i)
		{
			float dot = referenceNormal.dot( IncPoly->shape->normals[i] );
			if(dot < minDot)
			{
				minDot = dot;
				incidentFace = i;
			}
		}

		// Assign face vertices for incidentFace
		v[0] = IncPoly->angle * ((Polygon*)IncPoly->shape)->points[incidentFace] + IncPoly->pos;
		incidentFace = incidentFace + 1 >= (int)IncPoly->shape->normals.size() ? 0 : incidentFace + 1;
		v[1] = IncPoly->angle * ((Polygon*)IncPoly->shape)->points[incidentFace] + IncPoly->pos;
	}

	int Clip( Vec n, float c, Vec *face )
	{
		uint sp = 0;
		Vec out[2] = {
			face[0],
			face[1]
		};

		// Retrieve distances from each endpoint to the line
		// d = ax + by - c
		float d1 = n.dot(face[0]) - c;
		float d2 = n.dot(face[1]) - c;

		// If negative (behind plane) clip
		if(d1 <= 0.0f) out[sp++] = face[0];
		if(d2 <= 0.0f) out[sp++] = face[1];

		// If the points are on different sides of the plane
		if(d1 * d2 < 0.0f) // less than to ignore -0.0f
		{
			// Push interesection point
			float alpha = d1 / (d1 - d2);
			out[sp] = face[0] + (face[1] - face[0]) * alpha;
			++sp;
		}

		// Assign our new converted values
		face[0] = out[0];
		face[1] = out[1];

		assert( sp != 3 );

		return sp;
	}
inline bool BiasGreaterThan( float a, float b )
{
  const float k_biasRelative = 0.95f;
  const float k_biasAbsolute = 0.01f;
  return a >= b * k_biasRelative + a * k_biasAbsolute;
}

	bool solve(){
		contact_count = 0;
		uint faceA,faceB;
		float deepA = sat(A, B, &faceA);
		if(deepA >= 0.f) //nie ma kolizji
			return false;
		float deepB = sat(B, A, &faceB);
		if(deepB >= 0.f) //nie ma kolizji
			return false;


		uint referenceIndex;
		bool flip; 

		Body *RefPoly; 
		Body *IncPoly; 

		// Determine which shape contains reference face
		if(BiasGreaterThan(deepA, deepB)){
			RefPoly = A;
			IncPoly = B;
			referenceIndex = faceA;
			flip = false;
		} else {
			RefPoly = B;
			IncPoly = A;
			referenceIndex = faceB;
			flip = true;
		}

		Vec incidentFace[2];
		find_incident_face( incidentFace, RefPoly, IncPoly, referenceIndex );


		Vec v1 = ((Polygon*) RefPoly->shape)->points[referenceIndex];
		referenceIndex = referenceIndex + 1 == RefPoly->shape->normals.size() ? 0 : referenceIndex + 1;
		Vec v2 = ((Polygon*) RefPoly->shape)->points[referenceIndex];

		v1 = RefPoly->angle * v1 + RefPoly->pos;
		v2 = RefPoly->angle * v2 + RefPoly->pos;

		Vec sidePlaneNormal = (v2 - v1);
		sidePlaneNormal = sidePlaneNormal.normalize();

		Vec refFaceNormal( sidePlaneNormal.y, -sidePlaneNormal.x );

		float refC = refFaceNormal.dot(v1);
		float negSide = -sidePlaneNormal.dot(v1);
		float posSide =  sidePlaneNormal.dot(v2);

		if(Clip( -sidePlaneNormal, negSide, incidentFace ) < 2)
			return false; 

		if(Clip(  sidePlaneNormal, posSide, incidentFace ) < 2)
			return false;

		// Flip
		normal = flip ? -refFaceNormal : refFaceNormal;

		// Keep points behind reference face
		uint cp = 0; // clipped points behind reference face
		float separation = refFaceNormal.dot( incidentFace[0] ) - refC;
		if(separation <= 0.0f)
		{
			contacts[cp] = incidentFace[0];
			penetration = -separation;
			++cp;
		}
		else
			penetration = 0;

		separation = refFaceNormal.dot( incidentFace[1] ) - refC;
		if(separation <= 0.0f)
		{
			contacts[cp] = incidentFace[1];

			penetration += -separation;
			++cp;

			// Average penetration
			penetration /= (float)cp;
		}

		contact_count = cp;
		return true;
	}

	void init(float _dt){
		avg_rest = std::min(A->restitution, B->restitution);

		s_friction = std::sqrt(A->static_friction * B->static_friction);
		d_friction = std::sqrt(A->dynamic_friction * B->dynamic_friction);

		for(uint i = 0; i < contact_count; ++i){
			Vec rra = contacts[i] - A->pos;
			Vec rrb = contacts[i] - B->pos;

			Vec rrv = B->velocity + rrb.cross(-B->angular_velocity) -
				A->velocity - rra.cross(-A->angular_velocity);

			if(rrv.len()*rrv.len() < (gravity * _dt).len()*(gravity * _dt).len() + EPSILON){
				//avg_rest = 0.0f;	
			}
		}
	}

	void apply_impulse(){
		
		for(uint i = 0; i < contact_count; i++){
			Vec rra = contacts[i] - A->pos;
			Vec rrb = contacts[i] - B->pos;


			Vec rrv = B->velocity + rrb.cross(-B->angular_velocity) -
				A->velocity - rra.cross(-A->angular_velocity);

			float contact_vel = rrv.dot(normal);

			if(contact_vel > 0)
				return;

			float ra_cross_n = rra.cross(normal);
			float rb_cross_n = rrb.cross(normal);

			float inv_sum = A->iM + B->iM + ra_cross_n*ra_cross_n * A->iI + rb_cross_n*rb_cross_n * B->iI;

			
			float j = (-1.f + avg_rest) * contact_vel;
			j /= inv_sum;
			j /= (float)contact_count;

			std::cout << "J "<< j << std::endl;
			Vec impulse = normal *j;
			A->ApplyImpulse(-impulse, rra);
			B->ApplyImpulse(impulse, rrb);

			rrv = B->velocity + rrb.cross(-B->angular_velocity) -
				A->velocity - rra.cross(-A->angular_velocity);

			Vec t = rrv - (normal * rrv.dot(normal));
			t = t.normalize();

			float jt = -rrv.dot(t);
			jt /= inv_sum;
			jt /= (float) contact_count;

			if (almost_equal(jt, 0.0f)){
				return;
			}

			Vec tangent_imp;
			if(std::abs(jt) < j * s_friction)
				tangent_imp = t*jt;
			else
				tangent_imp = t*(-j)*d_friction;

			A->ApplyImpulse(-tangent_imp, rra);
			B->ApplyImpulse(tangent_imp, rrb);
		}
	}
	void positional_correction( void )
	{
		const float k_slop = 0.05f; 
		const float percent = 0.4f; 
		Vec correction = normal * (std::max( penetration - k_slop, 0.0f ) / (A->iM + B->iM)) * percent;
		A->pos += -correction * A->iM;
		B->pos+= correction * B->iM;
	}
};
