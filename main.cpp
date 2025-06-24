#include <iostream>
#include <SFML/Graphics.hpp>
#include "C_RandomNum/C_RandomNum.h"

int ts = 64; //  宝石宽高
// sf::Vector2i offset(48.f,24.f);     //  调整元素与背景图格子的相对位置，我不打算用背景，故该变量无意义

//  宝石结构体，kind为宝石种类，共七种，索引为从0到6；
//  row与col表示宝石为第几行第几列，对应grid二维数组中的 i 与 j；
//  x，y为宝石实际的坐标位置，根据宝石的位置与宝石宽度计算可得，float类型原因为SFML库中sprite坐标为float类型
//  之所以将row、col与x，y分开来，是因为宝石在变动后，其应该所处的位置与实际位置可能不符，故设置两套坐标
//  match为计算宝石是否连成三个的标记字段
//  alpha为宝石颜色中的透明度，可配合做渐隐效果
struct piece {
    int col, row, kind, match , alpha;
    float x, y;
    piece() {
        match = 0;
        alpha = 255;
    }
} grid[10][10];     //  初始化10*10二维数组，但第一行（索引为0）与第10行（索引为9）留白，仅保留索引1~8

//  swap方法，互换两个宝石的位置
//  仅交换宝石的应该所处的行与列，实际位置由移动方法控制
void swap(piece p1, piece p2) {
    std::swap(p1.col, p2.col);
    std::swap(p1.row, p2.row);

    grid[p1.row][p1.col] = p1;
    grid[p2.row][p2.col] = p2;
}


int main() {
    //  设置游戏主画面
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(880, 680)), "BejeweledPokmon");
    window.setFramerateLimit(60);

    //  设置游戏素材，使用了七只64*64的宝可梦
    sf::Texture texture[7];
    texture[0] = sf::Texture("../resource/bulbasaur.png");
    texture[1] = sf::Texture("../resource/charmander.png");
    texture[2] = sf::Texture("../resource/clefairy.png");
    texture[3] = sf::Texture("../resource/eevee.png");
    texture[4] = sf::Texture("../resource/jigglypuff.png");
    texture[5] = sf::Texture("../resource/pikachu.png");
    texture[6] = sf::Texture("../resource/squirtle.png");

    //  sprite不能声明为数组，比较奇怪，关于SFML中的sprite特性需继续研究
    //  此处如果换为数组，写法上将更为简洁
    sf::Sprite bulbasaur(texture[0]);
    sf::Sprite charmander(texture[1]);
    sf::Sprite clefairy(texture[2]);
    sf::Sprite eevee(texture[3]);
    sf::Sprite jigglypuff(texture[4]);
    sf::Sprite pikachu(texture[5]);
    sf::Sprite squirtle(texture[6]);

    //  初始化grid
    //  一般情况下，宝石的x坐标对应二维数组的列，y坐标对应二维数组的行，与习惯不太相符，需特别注意
    for (int i = 1; i <= 8; i++) {
        for (int j = 1; j <= 8; j++) {
            grid[i][j].kind = RandomNum::range(0, 6);
            grid[i][j].col = j;
            grid[i][j].row = i;
            grid[i][j].x = static_cast<float>(j * ts);
            grid[i][j].y = static_cast<float>(i * ts);
        }
    }

    //  设置变量，共有以下几个功能：
    //  1、判断当前的宝石是否在交换过程中（如在过程中则不可点击）
    //  2、判断当前宝石是否在移动过程中（如在过程中则不可点击）
    //  3、记录鼠标点击的次序及位置，计算出玩家想要交换娜两个宝石，若符合规则，则支持其交换，否则恢复原状
    //      3.1、规则：①仅左右或上下邻近的宝石可以互相交换；②需满足交换后存在三个或以上的同种类型的宝石为一排或一列
    int x0, y0, x, y;
    int click = 0;
    sf::Vector2i pos;
    bool isSwap = false;
    bool isMoving = false;

    //  游戏主循环
    while (window.isOpen()) {
        while (std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            else if (event->is<sf::Event::KeyPressed>()) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) window.close();  //  ESC键关闭游戏
            } else if (event->is<sf::Event::MouseButtonPressed>()) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {  //  鼠标左键点击事件
                    if (!isSwap && !isMoving) click++;      //  表示仅在无当前无交换且无移动时，才可进行有效点击
                    pos = sf::Mouse::getPosition(window);   //  获取鼠标点击的坐标
                }
            }
        }

        //  计算第一次点击的宝石为第几行第几列
        if (click == 1) {
            x0 = pos.x / ts;
            y0 = pos.y / ts;
        }
        //  计算第二次点击的宝石为第几行第几列
        if (click == 2) {
            x = pos.x / ts;
            y = pos.y / ts;
            if (abs(x - x0) + abs(y - y0) == 1) {   //  两次点击的宝石必须为横向或纵向相邻的宝石
                swap(grid[y0][x0], grid[y][x]);     //  交换宝石应该所属的位置
                isSwap = true;                      //  设置正在交换变量为true
                click = 0;                          //  重置点击次数
            } else {
                click = 1;                          //  如第二次点击的宝石与第一次点击的宝石相距超过1，则作废第二次点击
            }
        }

        //  遍历grid，找出匹配的宝石，修改其match属性
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


        isMoving = false;                   //  初始化isMoving
        //  遍历grid，若存在宝石现在的位置（x，y）与宝石应该在的位置（row，col）不符的，坐标以每帧4个像素的速度移动过去
        for (int i = 1; i <= 8; i++) {
            for (int j = 1; j <= 8; j++) {
                auto &p = grid[i][j];
                float dx, dy;
                //  速度，每帧执行4个循环
                //  若不用循环方式实现速度，则需保证速度的乘数为偶数，相比之下循环更容易控制
                for (int k = 0; k < 4; k++) {
                    dx = p.x - static_cast<float>(p.col * ts);
                    dy = p.y - static_cast<float>(p.row * ts);
                    if (dx != 0.f) p.x -= dx / abs(dx);
                    if (dy != 0.f) p.y -= dy / abs(dy);
                }
                //  如果存在当前位置与应处位置不符的情况，则isMoving应为true
                if (dx != 0.f || dy != 0.f) isMoving = true;
            }
        }

        //  宝石消除时淡出的动画效果，以每帧减少10的透明度淡出
        if (!isMoving) {
            for (int i = 1; i <= 8; i++) {
                for (int j = 1; j <= 8; j++) {
                    if (grid[i][j].match != 0) {
                        if (grid[i][j].alpha > 10) {
                            grid[i][j].alpha -= 10;
                            isMoving = true;
                        }
                    }
                }
            }
        }

        //  计算得分，全grid的match属性累加即可
        int score = 0;
        for (int i = 1; i <= 8; i++) {
            for (int j = 1; j <= 8; j++) {
                score += grid[i][j].match;
            }
        }

        //  如果宝石交换后，所有的grid得分为0（即没有凑成可消除的结果），则将先前交换的宝石再交换回去，同时重置isSwap为false
        if (isSwap && !isMoving) {
            if (score == 0) {
                swap(grid[y0][x0], grid[y][x]);
            }
            isSwap = false;
        }

        //  根据宝石的match属性，将match不等于0的宝石消除，同时上方的宝石下落，并在列的最上方生成同等数量的宝石
        //  很偶然的情况下会出现最上方仅两个宝石消除的情况，此bug暂时未排查出
        //  缺少检测逻辑：若全场不存在可交换的宝石时，自动重新生成grid；
        if (!isMoving) {
            for (int i = 8; i > 1; i--) {
                for (int j = 1; j <= 8; j++) {
                    if (grid[i][j].match != 0) {
                        for (int n = i; n >= 1; n--) {
                            if (grid[n][j].match == 0) {
                                swap(grid[n][j], grid[i][j]);
                                break;
                            }
                        }
                    }
                }
            }
            for (int j = 1; j <= 8; j++) {
                for (int i = 8, n = 0; i > 0; i--) {
                    if (grid[i][j].match != 0) {
                        grid[i][j].kind = RandomNum::range(0, 6);
                        grid[i][j].y = static_cast<float>(-ts * n);
                        n += 1;
                        grid[i][j].match = 0;
                        grid[i][j].alpha = 255;
                    }
                }
            }
        }

        //  清理画布
        window.clear(sf::Color::White);
        //  根据宝石的属性显示游戏画面
        for (int i = 1; i <= 8; i++) {
            for (int j = 1; j <= 8; j++) {
                switch (auto piece = grid[i][j]; piece.kind) {
                    case 0:
                        bulbasaur.setPosition(sf::Vector2f(piece.x, piece.y));
                        bulbasaur.setColor(sf::Color(255,255,255,piece.alpha));
                        window.draw(bulbasaur);
                        break;
                    case 1:
                        charmander.setPosition(sf::Vector2f(piece.x, piece.y));
                        charmander.setColor(sf::Color(255,255,255,piece.alpha));
                        window.draw(charmander);
                        break;
                    case 2:
                        clefairy.setPosition(sf::Vector2f(piece.x, piece.y));
                        clefairy.setColor(sf::Color(255,255,255,piece.alpha));
                        window.draw(clefairy);
                        break;
                    case 3:
                        eevee.setPosition(sf::Vector2f(piece.x, piece.y));
                        eevee.setColor(sf::Color(255,255,255,piece.alpha));
                        window.draw(eevee);
                        break;
                    case 4:
                        jigglypuff.setPosition(sf::Vector2f(piece.x, piece.y));
                        jigglypuff.setColor(sf::Color(255,255,255,piece.alpha));
                        window.draw(jigglypuff);
                        break;
                    case 5:
                        pikachu.setPosition(sf::Vector2f(piece.x, piece.y));
                        pikachu.setColor(sf::Color(255,255,255,piece.alpha));
                        window.draw(pikachu);
                        break;
                    case 6:
                        squirtle.setPosition(sf::Vector2f(piece.x, piece.y));
                        squirtle.setColor(sf::Color(255,255,255,piece.alpha));
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
