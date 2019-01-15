#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "body.h"
#include "vec.h"
#include <iostream>

class Shape{
	public:
	enum Type{
		Poly	
	};

	Shape (){}
	virtual void init(void) = 0;
	virtual Shape* clone(void) = 0;
	virtual void draw(sf::RenderWindow* window, Vec pos, float angle) = 0;

	float moment_of_inertia = 0;
};

class Polygon : public Shape{
	public:
	std::vector<Vec> points;
	sf::ConvexShape cshape;
	Polygon(){}
	
	Shape* clone(){
		Polygon* p = new Polygon();	
		for (uint i = 0; i < points.size(); i++){
			p->points.push_back(Vec(points[i].x, points[i].y));
		}
		return p;
	}


	void init(){
		Vec com;

		//znajdz srodek ciezkosci i przesun pkty
		//tak zeby srodek ciezkosci = (0, 0) w ukladzie
		for (uint i = 0; i < points.size(); i++){
			com += points[i];
		}
		com = com * (1.f/ (float) points.size());

		//przy okazji dev do rysowania zrob ksztalt
		cshape.setPointCount(points.size());
    	cshape.setFillColor(sf::Color::Green);
		for (uint i = 0; i < points.size(); i++){
			points[i] = points[i] - com;

			cshape.setPoint(i, sf::Vector2f(points[i].x, points[i].y));
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

	void draw(sf::RenderWindow* window, Vec pos, float angle){
		cshape.setPosition(pos.x, pos.y);
		cshape.setRotation(angle);
		window->draw(cshape);
	}
};
