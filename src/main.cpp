#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include "vec.h"
#include "shape.h"
#include "world.h"
#include "body.h"
#include <ctime>

int main()
{
	srand(time(0));
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML");

	std::vector<Vec> P = {Vec(-20, -10), Vec(20, -20), Vec(20, 20), Vec(-20, 20)};
	std::vector<Vec> P2 = { Vec(10, -20),Vec(-50, -20), Vec(20, 20), Vec(-60, 20)};
	Polygon p;
	p.points = P;
	Polygon p2;
	p2.points = P2;
	Circle c1(15.f);
	World w;
	Body* b = w.add(&p, Vec(50, 50));
	b->angle.set(3.14f/6.f);
	Body* b2 = w.add(&p, Vec(100, 100));
	b2->stat = true;
	b2->inv_mass = 0;
	b2->inv_I = 0;
	Body* b3 = w.add(&c1, Vec(100, -20));
	//b3->angle.set(-0.3f);
	b->velocity = Vec(0, -2.f);

	sf::Clock clock;
	float accm = 0;
    while (window.isOpen())
    {
		sf::Time dt = clock.restart();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

			if (event.type == sf::Event::MouseButtonPressed){
				std::vector<Vec> points;

				std::vector<float> angs;
				int n_of_points = (float)(rand()%4)+3.f;
				std::cout << n_of_points;
				float rand_size = rand()%20+10;
				for (int i = 0; i < n_of_points; i++){
					//points.push_back(Vec())
					angs.push_back((float)rand()/(float)RAND_MAX * 2.f*3.14f);
				}
				std::sort(angs.begin(), angs.end());
				for (float a : angs){
					points.push_back(Vec(std::cos(a)*rand_size, std::sin(a)*rand_size));
					points.back().pretty("point");
				}
				Polygon px;
				px.points = points;
				Body* bx = w.add(&px, Vec(event.mouseButton.x, event.mouseButton.y));
			}
        }
		//accm += dt.asSeconds();
		//if(accm>3){
			w.step(dt.asSeconds());
			accm = 0;
		//}

        window.clear();
		w.draw(&window);
        window.display();
    }

    return 0;
}
