#include <SFML/Graphics.hpp>
#include <optional> 
#include "SFML\System.hpp"
#include "SFML\Window.hpp"
#include "SFML\Graphics.hpp"
#include <iostream>

int main() {
    //init game
    float GridSizeF = 100.f;
    unsigned gridSizeU = static_cast<unsigned>(GridSizeF); 


    // SFML 3 Syntax: VideoMode mit {}f
    sf::RenderWindow window(sf::VideoMode({1200, 800}), "Chess - SFML 3");

    //init game elements
    sf::RectangleShape shape(sf::Vector2f(GridSizeF,GridSizeF));

    while (window.isOpen()) {
        // Die neue pollEvent Syntax
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        //RENDER
        window.clear(sf::Color(45, 45, 45));

        //Grid
        window.draw(shape);



        window.display(); //show window
    }
    return 0;
}