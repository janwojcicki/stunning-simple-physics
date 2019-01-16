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
	enum Type{
		Poly	
	};

	Shape (){}
	virtual void init(void) = 0;
	virtual Shape* clone(void) = 0;
	virtual void draw(sf::RenderWindow* window, Vec pos, mat angle) = 0;

	float moment_of_inertia = 0;
	float furthest_point = 0;

	std::vector<Vec> normals; //proste normalne do boków
};

class Polygon : public Shape{
	public:
	std::vector<Vec> points;
	sf::ConvexShape cshape;
	Polygon(){}
	
	Shape* clone(){
		Polygon* p = new Polygon();	
		std::cout << points.size() << " "<<normals.size();
		for (uint i = 0; i < points.size(); i++){
			p->points.push_back(Vec(points[i].x, points[i].y));
		}
		for (uint i = 0; i < normals.size(); i++){
			p->normals.push_back(Vec(normals[i].x, normals[i].y));
		}
		return p;
	}


	void init(){
		Vec com;
		std::cout << "INIT";

		//znajdz srodek ciezkosci i przesun pkty
		//tak zeby srodek ciezkosci = (0, 0) w ukladzie
		for (uint i = 0; i < points.size(); i++){
			com += points[i];
		}
		com = com * (1.f/ (float) points.size());

		//przy okazji dev do rysowania zrob ksztalt
		//i najdalszy pkt od srodka
		cshape.setPointCount(points.size());
    	cshape.setFillColor(sf::Color::Green);
		for (uint i = 0; i < points.size(); i++){
			points[i] = points[i] - com;
			
			furthest_point = std::max(furthest_point, points[i].len());

			cshape.setPoint(i, sf::Vector2f(points[i].x, points[i].y));
		}

		//policz wersory prostych normalnych do boków
		int ni;
		for(uint i = 0; i < points.size(); i++){
			ni = i == points.size()-1 ? 0 : i+1;
			Vec bok = points[ni] - points[i];
			normals.push_back(Vec(bok.y, -bok.x).normalize());
		}

		//policz moment bezwladnosci
		float sum1=0;
		float sum2=0;
		for (uint n=0;n<points.size();++n)  { 
		   sum1 += points[n+1].cross_len(points[n])* 
				   (points[n+1].dot(points[n+1]) + points[n+1].dot(points[n]) + points[n].dot(points[n]));
		   sum2 += points[n+1].cross_len(points[n]);
		}

		moment_of_inertia = (sum1/sum2/6);
		
	}
	inline float RadToDeg(float Rad)
	{
		return Rad / M_PI * 180.f;
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
