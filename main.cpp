#include <iostream>
#include <SFML/Graphics.hpp>
#include "C_RandomNum/C_RandomNum.h"

int ts = 54;
sf::Vector2i offset(48,24);

struct piece {
    int x,y,col,row,kind,match;
    piece() {match=0;}
} grid[10][10];

int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(740,480)),"BejeweledPokmon");
    window.setFramerateLimit(60);

    sf::Texture bulbasaur,charmander,clefairy,eevee,jigglypuff,pikachu,squirtle;
    bulbasaur.loadFromFile("../resource/bulbasaur.png");
    charmander.loadFromFile("../resource/charmander.png");
    clefairy.loadFromFile("../resource/clefairy.png");
    eevee.loadFromFile("../resource/eevee.png");
    jigglypuff.loadFromFile("../resource/jiggle.png");
    pikachu.loadFromFile("../resource/pikachu.png");
    squirtle.loadFromFile("../resource/squirtle.png");

    sf::Sprite bulbasaur_s(bulbasaur),charmander_s(charmander),clefairy_s(clefairy),eevees_s(eevee),jigglypuff_s(jigglypuff),pikachu_s(pikachu),squirtle_s(squirtle);

    while (window.isOpen()) {
        while (std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            else if (event->is<sf::Event::KeyPressed>()) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) window.close();
            }
        }
        window.clear(sf::Color::White);
        window.draw(bulbasaur_s);
        window.draw(charmander_s);
        window.draw(clefairy_s);
        window.draw(eevees_s);
        window.draw(jigglypuff_s);
        window.draw(pikachu_s);
        window.draw(squirtle_s);
        window.display();
    }



    return 0;

}