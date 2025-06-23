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

    int x0, y0, x, y;
    int click = 0;
    sf::Vector2i pos;
    bool isSwap = false;
    bool isMoving = false;

    while (window.isOpen()) {
        while (std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            else if (event->is<sf::Event::KeyPressed>()) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) window.close();
            } else if (event->is<sf::Event::MouseButtonPressed>()) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                    if (!isSwap && !isMoving) click++;
                    pos = sf::Mouse::getPosition(window);
                }
            }
        }

        if (click == 1) {
            x0 = pos.x / ts;
            y0 = pos.y / ts;
        }
        if (click == 2) {
            x = pos.x / ts;
            y = pos.y / ts;
            if (abs(x - x0) + abs(y - y0) == 1) {
                swap(grid[y0][x0], grid[y][x]);
                isSwap = true;
                click = 0;
            } else {
                std::cout << x << " " << y << std::endl;
                std::cout << x0 << " " << y0 << std::endl;
                click = 1;
            }
        }

        for (int i = 1; i <= 8; i++) {
            for (int j = 1; j <= 8; j++) {
                if (grid[i][j].kind == grid[i + 1][j].kind && grid[i][j].kind == grid[i - 1][j].kind) {
                    grid[i - 1][j].match++;
                    grid[i][j].match++;
                    grid[i + 1][j].match++;
                }
                if (grid[i][j].kind == grid[i][j + 1].kind && grid[i][j].kind == grid[i][j - 1].kind) {
                    grid[i][j - 1].match++;
                    grid[i][j].match++;
                    grid[i][j + 1].match++;
                }
            }
        }


        isMoving = false;
        for (int i = 1; i <= 8; i++) {
            for (int j = 1; j <= 8; j++) {
                auto &p = grid[i][j];
                float dx, dy;
                for (int k = 0; k < 2; k++) {
                    dx = p.x - static_cast<float>(p.col * ts);
                    dy = p.y - static_cast<float>(p.row * ts);
                    if (dx != 0.f) p.x -= dx / abs(dx);
                    if (dy != 0.f) p.y -= dy / abs(dy);
                }
                if (dx != 0.f || dy != 0.f) isMoving = true;
            }
        }

        int score = 0;
        for (int i = 1; i <= 8; i++) {
            for (int j = 1; j <= 8; j++) {
                score += grid[i][j].match;
            }
        }

        if (isSwap && !isMoving) {
            if (score == 0){
                swap(grid[y0][x0], grid[y][x]);
                isSwap = false;
            }
        }

        if (!isMoving) {
            // for (int i = 8; i > 1; i--) {
            //     for (int j = 1; j <= 8; j++) {
            //         if (grid[i][j].match != 0) {
            //             for (int n = i; n >= 1; n--) {
            //                 if (grid[n][j].match == 0) {
            //                     swap(grid[n][j], grid[i][j]);
            //                     break;
            //                 }
            //             }
            //         }
            //     }
            // }
            // for (int j = 1; j <= 8; j++) {
            //     for (int i = 8, n = 0; i > 0; i--) {
            //         if (grid[i][j].match != 0) {
            //             grid[i][j].kind = RandomNum::range(0, 6);
            //             grid[i][j].y = static_cast<float>(ts * n);
            //             n += 1;
            //             grid[i][j].match = 0;
            //         }
            //     }
            // }
            for (int i = 8; i > 0; i--) {
                for (int j = 1; j <= 8; j++) {
                    if (grid[i][j].match != 0) {
                        std::cout << grid[i][j].row << " "<< grid[i][j].col << std::endl;
                    }
                }
            }
        }

        window.clear(sf::Color::White);
        for (int i = 1; i <= 8; i++) {
            for (int j = 1; j <= 8; j++) {
                switch (auto piece = grid[i][j]; piece.kind) {
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
