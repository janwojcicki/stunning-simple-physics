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

	std::vector<Vec> P = {Vec(-100, -100), Vec(-100, 100), Vec(100, 100), Vec(100, -100)};
	Polygon p;
	p.points = P;

	World w;
	w.add(&p, Vec(50, 50));

	std::cout << p.moment_of_inertia;
	sf::Clock clock;
    while (window.isOpen())
    {
		sf::Time dt = clock.restart();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
		w.draw(&window);
        window.display();
    }

    return 0;
}
