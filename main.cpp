#include <iostream>
#include <SFML/Graphics.hpp>
#include "C_RandomNum/C_RandomNum.h"

int ts = 64; //  宝石宽高
// sf::Vector2i offset(48.f,24.f);     //  调整元素与背景图格子的相对位置，我不打算用背景，故该变量无意义

struct piece {
    int col, row, kind, match;
    float x, y;
    piece() { match = 0; }
} grid[10][10];

void swap(piece p1, piece p2) {
    std::swap(p1.col, p2.col);
    std::swap(p1.row, p2.row);

    grid[p1.row][p1.col] = p1;
    grid[p2.row][p2.col] = p2;
}


int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(880, 680)), "BejeweledPokmon");
    window.setFramerateLimit(60);

    sf::Texture texture[7];
    texture[0] = sf::Texture("../resource/bulbasaur.png");
    texture[1] = sf::Texture("../resource/charmander.png");
    texture[2] = sf::Texture("../resource/clefairy.png");
    texture[3] = sf::Texture("../resource/eevee.png");
    texture[4] = sf::Texture("../resource/jigglypuff.png");
    texture[5] = sf::Texture("../resource/pikachu.png");
    texture[6] = sf::Texture("../resource/squirtle.png");


    sf::Sprite bulbasaur(texture[0]);
    sf::Sprite charmander(texture[1]);
    sf::Sprite clefairy(texture[2]);
    sf::Sprite eevee(texture[3]);
    sf::Sprite jigglypuff(texture[4]);
    sf::Sprite pikachu(texture[5]);
    sf::Sprite squirtle(texture[6]);

    for (int i = 1; i <= 8; i++) {
        for (int j = 1; j <= 8; j++) {
            grid[i][j].kind = RandomNum::range(0, 6);
            grid[i][j].col = j;
            grid[i][j].row = i;
            grid[i][j].x = static_cast<float>(j * ts);
            grid[i][j].y = static_cast<float>(i * ts);
        }
    }

    while (window.isOpen()) {
        while (std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            else if (event->is<sf::Event::KeyPressed>()) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) window.close();
            }
        }
        window.clear(sf::Color::White);
        for (int i = 1; i <= 8; i++) {
            for (int j = 1; j <= 8; j++) {
                auto piece = grid[i][j];
                switch (piece.kind) {
                    case 0:
                        bulbasaur.setPosition(sf::Vector2f(piece.x, piece.y));
                        window.draw(bulbasaur);
                        break;
                    case 1:
                        charmander.setPosition(sf::Vector2f(piece.x, piece.y));
                        window.draw(charmander);
                        break;
                    case 2:
                        clefairy.setPosition(sf::Vector2f(piece.x, piece.y));
                        window.draw(clefairy);
                        break;
                    case 3:
                        eevee.setPosition(sf::Vector2f(piece.x, piece.y));
                        window.draw(eevee);
                        break;
                    case 4:
                        jigglypuff.setPosition(sf::Vector2f(piece.x, piece.y));
                        window.draw(jigglypuff);
                        break;
                    case 5:
                        pikachu.setPosition(sf::Vector2f(piece.x, piece.y));
                        window.draw(pikachu);
                        break;
                    case 6:
                        squirtle.setPosition(sf::Vector2f(piece.x, piece.y));
                        window.draw(squirtle);
                        break;
                    default: ;
                }
            }
        }
        window.display();
    }


    return 0;
}
