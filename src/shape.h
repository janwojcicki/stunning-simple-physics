#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "body.h"
#include "vec.h"
#include "mat.h"
#include <iostream>
#include <cmath>


class Shape{
	public:
		inline float RadToDeg(float Rad)
		{
			return Rad / M_PI * 180.f;
		}
		int type = 0;

		Shape (){}
		virtual void init(void) = 0;
		virtual Shape* clone(void) = 0;
		virtual void draw(sf::RenderWindow* window, Vec pos, mat angle) = 0;

		float moment_of_inertia = 0;
		float furthest_point = 0;

		std::vector<Vec> normals; //proste normalne do boków
};

class Circle : public Shape{
	public:
		float radius;
		sf::CircleShape cshape;
		Shape* clone(){
			Circle* c = new Circle(radius);
			return c;
		}
		Circle(float _rad)  : radius(_rad){
			type = 1;
		}

		void init(){
			cshape.setRadius(radius);
			cshape.setFillColor(sf::Color::Green);
			furthest_point = radius;
			moment_of_inertia = 0.5f * radius * radius;
		}
		void draw(sf::RenderWindow* window, Vec pos, mat angle){
			cshape.setPosition(pos.x-radius, pos.y-radius);
			//cshape.setRotation(RadToDeg(angle.angle()));
			window->draw(cshape);
		}
};

class Polygon : public Shape{
	public:
		std::vector<Vec> points;
		sf::ConvexShape cshape;
		Polygon(){
			type = 2;
		}

		Shape* clone(){
			Polygon* p = new Polygon();	
			for (uint i = 0; i < points.size(); i++){
				p->points.push_back(Vec(points[i].x, points[i].y));
			}
			for (uint i = 0; i < normals.size(); i++){
				p->normals.push_back(Vec(normals[i].x, normals[i].y));
			}
			return p;
		}

		static bool sort_points_anticlockwise(Vec a, Vec b){
			Vec center(0, 0);
			if (a.x - center.x >= 0 && b.x - center.x < 0)
				return false;
			if (a.x - center.x < 0 && b.x - center.x >= 0)
				return true;
			if (a.x - center.x == 0 && b.x - center.x == 0) {
				if (a.y - center.y >= 0 || b.y - center.y >= 0)
					return !(a.y > b.y);
				return !(b.y > a.y);
			}

			// compute the cross product of vectors (center -> a) x (center -> b)
			int det = (a.x - center.x) * (b.y - center.y) - (b.x - center.x) * (a.y - center.y);
			if (det < 0)
				return false;
			if (det > 0)
				return true;

			// points a and b are on the same line from the center
			// check which point is closer to the center
			int d1 = (a.x - center.x) * (a.x - center.x) + (a.y - center.y) * (a.y - center.y);
			int d2 = (b.x - center.x) * (b.x - center.x) + (b.y - center.y) * (b.y - center.y);
			return !(d1 > d2);
		}


		void init(){
			Vec com;

			//znajdz srodek ciezkosci i przesun pkty
			//tak zeby srodek ciezkosci = (0, 0) w ukladzie
			for (uint i = 0; i < points.size(); i++){
				com += points[i];
			}
			com = com * (1.f/ (float) points.size());

			//i najdalszy pkt od srodka
			for (uint i = 0; i < points.size(); i++){
				points[i] = points[i] - com;

				furthest_point = std::max(furthest_point, points[i].len());
			}

			std::sort(points.begin(), points.end(), sort_points_anticlockwise);

			//przy okazji dev do rysowania zrob ksztalt
			cshape.setPointCount(points.size());
			cshape.setFillColor(sf::Color::Green);
			//policz wersory prostych normalnych do boków
			int ni;
			for(uint i = 0; i < points.size(); i++){
				ni = i == points.size()-1 ? 0 : i+1;
				Vec bok = points[ni] - points[i];
				normals.push_back(Vec(bok.y, -bok.x).normalize());
				cshape.setPoint(i, sf::Vector2f(points[i].x, points[i].y));
			}

			//policz moment bezwladnosci
			float sum1=0;
			float sum2=0;
			for (uint n=0;n<points.size();++n)  { 
				ni = n == points.size()-1 ? 0 : n+1;
				sum1 += points[ni].cross_len(points[n])* 
					(points[ni].dot(points[ni]) + points[ni].dot(points[n]) + points[n].dot(points[n]));
				sum2 += points[ni].cross_len(points[n]);
			}

			moment_of_inertia = (sum1/sum2/6);

		}

		void draw(sf::RenderWindow* window, Vec pos, mat angle){
			cshape.setPosition(pos.x, pos.y);
			cshape.setRotation(RadToDeg(angle.angle()));
			window->draw(cshape);
		}

		Vec get_extreme_point( const Vec& dir ){
			float extreme_dis = -500000;
			Vec extreme_vertex;

			for(uint i = 0; i < points.size(); i++){
				Vec v = points[i];
				float dis = v.dot(dir);

				if(extreme_dis < dis){
					extreme_dis = dis;
					extreme_vertex = v;
				}
			}
			return extreme_vertex;
		}
};
