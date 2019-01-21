#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include "vec.h"
#include "shape.h"
#include "world.h"
#include "body.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML");

	std::vector<Vec> P = {Vec(-20, -20), Vec(20, -20), Vec(20, 20), Vec(-60, 20)};
	std::vector<Vec> P2 = {Vec(-50, -20), Vec(10, -20), Vec(20, 20), Vec(-60, 20)};
	Polygon p;
	p.points = P;
	Polygon p2;
	p2.points = P2;
	World w;
	Body* b = w.add(&p, Vec(50, 50));
	b->angle.set(3.14f/6.f);
	Body* b2 = w.add(&p, Vec(100, 100));
	b2->stat = true;
	b2->iM = 0;
	b2->iI = 0;
	Body* b3 = w.add(&p2, Vec(80, -20));
	b3->angle.set(-0.3f);
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
