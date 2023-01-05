#include "classes.h"

Game::Game(int win_score, int field_x, int field_y, int rack_width)
    : ball(field_x/2, field_y/2),
      rackets(rack_width, field_y, field_y/2-rack_width/2, RACK_X_OFFSET, field_x - RACK_X_OFFSET)
{
    this->win_score = win_score;
    this->field_x = field_x;
    this->field_y = field_y;

    score_left = score_right = 0;

    initscr();
    nodelay(curscr, true);
}

int Game::render() {
    clear();

    //    выводим счет
    printw("                                      %d | %d \n", score_left, score_right);

//    выводим остальное поле
    for (int y = 0; y < field_y; ++y) {
        for (int x = 0; x < field_x; ++x) {
            if ((x == 1 || x == field_x - 1) && y != 0) {
                printw("|");
            } else {
                if ((y == 0 || y == field_y - 1) && x != 0) {
                    printw("_");
                } else {
//          отрисовка левой ракетки
                    if (x == RACK_X_OFFSET && y >= rackets.getLeftYCoord() + 1
                        && y < rackets.getLeftYCoord() + rackets.getWidth() + 1)
                        printw("|");
                    else
//          отрисовка правой ракетки
                    if (x == field_x - RACK_X_OFFSET && y >= rackets.getRightYCoord() + 1
                        && y < rackets.getRightYCoord() + rackets.getWidth() + 1)
                        printw("|");
                    else
//          отрисовка мячика
                    if (ball.getYCoord() == y && ball.getXCoord() == x)
                        printw("o");
                    else if (x == field_x/2)
                        printw(":");
                    else
                        printw(" ");
                }
            }
        }
        printw("\n");
    }

    refresh();
    return 0;
}

int Game::next_step() {
    ball.move();

    //    отражение мячика от ракеток
    if (ball.getXCoord() == rackets.getLeftXCoord() + 1 && ball.getYCoord() >= rackets.getLeftYCoord() &&
        ball.getYCoord() <= rackets.getLeftYCoord() + rackets.getWidth()) {
        ball.reflect_x();
    }
    if (ball.getXCoord() >= rackets.getRightXCoord() - 1 && ball.getYCoord() >= rackets.getRightYCoord() &&
        ball.getYCoord() <= rackets.getRightYCoord() + rackets.getWidth()) {
        ball.reflect_x();
    }

//    пропуск мячика левым игроком
    if (ball.getXCoord() < rackets.getLeftXCoord()) {
        score_right += 1;

        init();  // ставим в начальное положение
    }
//    пропуск мячика правым игроком
    if (ball.getXCoord() > rackets.getRightXCoord()) {
        score_left += 1;

        init();  // ставим в начальное положение
    }

//    отражение мячика от стен
    if (ball.getYCoord() == 1 || ball.getYCoord() == field_y - 2) {
        ball.reflect_y();
    }

    return 0;
}

int Game::read_symb() {
    switch (getchar()) {
        case 'a':
            rackets.move(-1, 0);
            break;
        case 'z':
            rackets.move(1, 0);
            break;
        case 'm':
            rackets.move(0, 1);
            break;
        case 'k':
            rackets.move(0, -1);
            break;
        default:
            return 1;
    }
    return 0;
}

int Game::init() {
    ball.init(field_x/2, field_y/2);
    rackets.init(field_y/2-2);

    return 0;
}

int Game::win_screen() {
    clear();
    //  рисуем новое поле
    for (int y = 0; y < 25; ++y) {
        for (int x = 0; x < 80; ++x) {
            if ((x == 0 || x == 79) && y != 24 && y != 0) {
                printw("|");
                continue;
            }
//          Углы
            if (y == 0 && x == 0) {
                printw(" ");
                continue;
            }
            if (y == 24 && x == 0) {
                printw(" ");
                continue;
            }
            if (y == 0 && x == 79) {
                printw(" ");
                continue;
            }
            if (y == 24 && x == 79) {
                printw(" ");
                continue;
            }
//          Поздравление
            if (y == 9 && x == 32) {
                printw("CONGRATULATIONS!");
                x += 15;
                continue;
            }
//          Выводим, кто победитель
            if (y == 12 && x == 23) {
                if (score_left == win_score) {
                    printw("Player left, you are the winner!");
                    x += 31;
                    continue;
                } else {
                    printw("Player right, you are the winner!");
                    x += 32;
                    continue;
                }
            }
//          Выводим счет
            if (y == 15 && x == 37) {
                printw("%d : %d", score_left, score_right);
                x += 4;
                continue;
            } else {
                printw(" ");
            }
        }
        printw("\n");
    }

    refresh();

    getchar();
    return 0;
}

int Rackets::move(int left_offset, int right_offset) {
    left_y_coord += left_offset;
    right_y_coord += right_offset;

    left_y_coord < 0 ? left_y_coord = 0 : 0;
    right_y_coord < 0 ? right_y_coord = 0 : 0;

    left_y_coord + width + 1 >= field_y ? left_y_coord = field_y - width - 2 : 0;
    right_y_coord + width + 1 >= field_y ? right_y_coord = field_y - width - 2 : 0;
    return 0;
}
