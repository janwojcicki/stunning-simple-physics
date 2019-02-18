#include "body.h"
#include "shape.h"
#include <assert.h>
#include "consts.h"
#include <cfloat>


class Contact{
	public:
		Body* AA, *BB;
		Vec normal;
		float penetration;
		Vec contacts[2];
		uint contact_count = 0;

		float avg_rest;
		float s_friction;
		float d_friction;
		Vec gravity;

		bool tmp = false;



		Contact(Body* _a, Body* _b, Vec _gravity) : AA(_a), BB(_b), gravity(_gravity){}


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

		bool solve_poly_to_poly(Body* A, Body* B){
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

				penetration /= (float)cp;
			}

			contact_count = cp;
			return true;
		}
		bool solve_circle_to_poly(Body* A, Body* B){
			Circle* aa = (Circle*) A->shape;
			Polygon* bb = (Polygon*) B->shape;

			contact_count = 0;

			Vec center = A->pos;
			center = B->angle.transpose() * (center - B->pos);
			float seperation  = -FLT_MAX;
			uint normalface = 0;
			for(uint i = 0; i < bb->points.size(); ++i){
				float s = bb->normals[i].dot( (center - bb->points[i]) );

				if (s > aa->radius) {
					return false;	
				}
				if( s > seperation){
					seperation = s;
					normalface = i;
				}
			}

			Vec v1 = bb->points[normalface];
			int ni = (uint)normalface + 1 < bb->points.size() ? normalface + 1 : 0;
			Vec v2 = bb->points[ni];

			if (seperation < EPSILON){
				contact_count = 1;
				normal = -(B->angle * bb->normals[normalface]);
				contacts[0] = normal*aa->radius + A->pos;
				if(!tmp && std::isnan(contacts[0].x)){
					normal.pretty("normal");
					fuck(aa->radius, "radius");
					tmp = true;
				}
				penetration = aa->radius;
				return true;
			}

			float dot1 = (center - v1).dot(v2-v1);
			float dot2 = (center - v2).dot(v1-v2);
			penetration = aa->radius - seperation;

			if(dot1 <= 0.0f){
				if((center-v1).len() > aa->radius)	
					return false;

				contact_count = 1;
				Vec n = B->angle* (v1 - center);
				normal = n.normalize();
				n = normal;
				v1 = B->angle * v1 + B->pos;
				contacts[0] = v1;
				if(std::isnan(contacts[0].x)){
					std::cout << "2";
				}
			}
			else if(dot2 <= 0.0f){
				if((center-v2).len() > aa->radius)	
					return false;

				contact_count = 1;
				Vec n = v2 - center;
				v2 = B->angle * v2 + B->pos;
				contacts[0] = v2;
				n = B->angle * n;
				n = n.normalize();
				normal = n;
				if(std::isnan(contacts[0].x)){
					std::cout << "3";
				}
			} else{
				Vec n = bb->normals[normalface];
				if ((center - v1).dot(n) > aa->radius)
					return false;

				n = B->angle * n;
				normal = -n;
				contacts[0] = normal * aa->radius + A->pos;
				contact_count = 1;
				if(std::isnan(contacts[0].x)){
					std::cout << "4";
				}
			}
			return true;
		}
		bool solve_circle_to_circle(Body* A, Body* B){
			Circle* aa = (Circle*) A->shape;
			Circle* bb = (Circle*) B->shape;

			Vec n = B->pos - A->pos;

			float dist = n.len();
			float sum_radius = aa->radius + bb->radius;
			if(dist >= sum_radius){
				contact_count = 0;
				return false;
			}

			contact_count = 1;

			if(dist == 0.0f){
				penetration = aa->radius;
				normal = Vec(1, 0);
				contacts[0] = A->pos;
			}else{
				penetration = sum_radius - dist;
				normal = n * (1.f/dist);
				contacts[0] = normal * aa->radius + A->pos;
			}

			return true;
		}


		bool solve(){
			
			if(AA->shape->type == 2	&& BB->shape->type ==2){
				return solve_poly_to_poly(AA, BB);
			}else if(AA->shape->type == 1	&& BB->shape->type ==1){
				return solve_circle_to_circle(AA, BB);
			}else if(AA->shape->type == 1	&& BB->shape->type ==2){
				return solve_circle_to_poly(AA, BB);
			}else if(AA->shape->type == 2	&& BB->shape->type ==1){
				bool x = solve_circle_to_poly(BB, AA);
				normal = -normal;
				return x;
			}
			return true;
		}

		void init(float _dt){
			avg_rest = std::min(AA->restitution, BB->restitution);

			s_friction = std::sqrt(AA->static_friction * BB->static_friction);
			d_friction = std::sqrt(AA->dynamic_friction * BB->dynamic_friction);

			for(uint i = 0; i < contact_count; ++i){
				Vec rra = contacts[i] - AA->pos;
				Vec rrb = contacts[i] - BB->pos;

				Vec rrv = BB->velocity + rrb.cross(-BB->angular_velocity) -
					AA->velocity - rra.cross(-AA->angular_velocity);

				if(rrv.len()*rrv.len() < (gravity * _dt).len()*(gravity * _dt).len() + EPSILON){
					//avg_rest = 0.0f;	
				}
			}
		}

		void apply_impulse(){

			for(uint i = 0; i < contact_count; i++){
				Vec rra = contacts[i] - AA->pos;
				Vec rrb = contacts[i] - BB->pos;


				Vec rrv = BB->velocity + rrb.cross(-BB->angular_velocity) -
				AA->velocity - rra.cross(-AA->angular_velocity);

				float contact_vel = rrv.dot(normal);

				if(contact_vel > 0)
					return;

				float ra_cross_n = rra.cross(normal);
				float rb_cross_n = rrb.cross(normal);

				float inv_sum = AA->inv_mass + BB->inv_mass + ra_cross_n*ra_cross_n * AA->inv_I + rb_cross_n*rb_cross_n * BB->inv_I;


				float j = (-1.f + avg_rest) * contact_vel;
				j /= inv_sum;
				j /= (float)contact_count;

				Vec impulse = normal *j;
				AA->ApplyImpulse(-impulse, rra);
				BB->ApplyImpulse(impulse, rrb);

				rrv = BB->velocity + rrb.cross(-BB->angular_velocity) -
					AA->velocity - rra.cross(-AA->angular_velocity);

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

				AA->ApplyImpulse(-tangent_imp, rra);
				BB->ApplyImpulse(tangent_imp, rrb);
			}
		}
		void positional_correction( void )
		{
			const float k_slop = 0.05f; 
			const float percent = 0.4f; 
			Vec correction = normal * (std::max( penetration - k_slop, 0.0f ) / (AA->inv_mass + BB->inv_mass)) * percent;
			AA->pos += -correction * AA->inv_mass;
			BB->pos+= correction * BB->inv_mass;
		}
};
