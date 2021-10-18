#include "vector_utils.hpp"
#include "world_cell.hpp"

#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

int main(int, char**)
{
    WorldCell wc;
    wc.shaffle();

    std::vector<sf::Vector2f> points;

    std::cout << "Hello, world!\n";
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    while (window.isOpen()) {
        const auto t1 = std::chrono::steady_clock::now();
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i pi = { event.mouseButton.x, event.mouseButton.y };
                auto p = window.mapPixelToCoords(pi);
                sf::Vector2f inter;
                if (wc.rayCast(wc.getPlayerPosition(), p, inter)) {
                    points.push_back(inter);
                    wc._rope.plus.clear();
                    wc._rope.points.clear();
                    wc._rope.points.push_back(inter, p);
                }
            }
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        // window.draw(...);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            wc.moveLeft();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            wc.moveRight();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            wc.moveUp();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            wc.moveDown();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            wc.dec();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            wc.inc();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            wc._rope.plus.clear();
            wc._rope.points.clear();
        }

        wc.draw(window);
        for (const auto& p : points) {
            sf::CircleShape c;
            c.setPosition(p);
            c.setRadius(5);
            c.setPointCount(5);
            c.setOrigin(c.getRadius(), c.getRadius());
            c.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
            window.draw(c);
        }

        wc.update();
        // end the current frame
        window.display();

        const auto t2 = std::chrono::steady_clock::now();
        constexpr auto T = std::chrono::microseconds(1'000'000) / 60;
        const auto dt = T - (t2 - t1);
        if (dt > std::chrono::seconds(0)) {
            std::this_thread::sleep_for(dt);
        }
    }
}
