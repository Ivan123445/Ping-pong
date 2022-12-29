// Copyright 2022 Ivan123445

#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>

// мячик
#define BALL_START_POS_X 24
#define BALL_START_POS_Y 6
#define BALL_START_SPD_X 1
#define BALL_START_SPD_Y (-1)

// ракетки
#define RACK_L_START_POS_X 4
#define RACK_L_START_POS_Y 11
#define RACK_R_START_POS_X 76
#define RACK_R_START_POS_Y 11

// скорость игры
#define TIME_ONE_STEP 70000
#define WIN_SCORE 24

int ball_x_coord, ball_y_coord, ball_x_speed, ball_y_speed;
int rack_left_y_coord, rack_right_y_coord;
int score_left, score_right;


void input_field_to_screen();

void move_ball();

int read_symb();

void move_rack(char mov_symb);

void start_data();

void win_screen();

int main() {
    start_data();  //   начальные позиции мяча и ракеток

    initscr();
    nodelay(stdscr, 1);

    while (score_left != WIN_SCORE && score_right != WIN_SCORE) {
        // бесконечный цикл, крутится без остановки пока не достигнем счета в 21
        clear();
        input_field_to_screen();  // отрисовываем счет поле
        refresh();                // отвечает за вывод на экран из printw
        read_symb();              // считываем символы(в read_symb) и перемещаем ракетки
        move_ball();              // перемещаем мяч
        usleep(TIME_ONE_STEP);

    }

    clear();
    win_screen();
    refresh();
    getchar();

    endwin();

    return 0;
}

void win_screen() {
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
                if (score_left == WIN_SCORE) {
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
}

void input_field_to_screen() {
//    выводим счет
    printw("                                      %d | %d \n", score_left, score_right);

//    выводим остальное поле
    for (int y = 0; y < 25; ++y) {
        for (int x = 0; x < 80; ++x) {
            if ((x == 1 || x == 79) && y != 0) {
                printw("|");
            } else {
                if ((y == 0 || y == 24) && x != 0) {
                    printw("_");
                } else {
//          отрисовка левой ракетки
                    if (x == RACK_L_START_POS_X && y >= rack_left_y_coord && y <= rack_left_y_coord + 2)
                        printw("|");
                    else
//          отрисовка правой ракетки
                    if (x == RACK_R_START_POS_X && y >= rack_right_y_coord && y <= rack_right_y_coord + 2)
                        printw("|");
                    else
//          отрисовка мячика
                    if (ball_y_coord == y && ball_x_coord == x)
                        printw("o");
                    else if (x == 40)
                        printw(":");
                    else
                        printw(" ");
                }
            }
        }
        printw("\n");
    }
}

void move_ball() {
//    отражение мячика от ракеток
    if (ball_x_coord == RACK_L_START_POS_X + 1 && ball_y_coord >= rack_left_y_coord &&
        ball_y_coord <= rack_left_y_coord + 3) {
        ball_x_speed *= -1;
    }
    if (ball_x_coord >= RACK_R_START_POS_X - 1 && ball_y_coord >= rack_right_y_coord &&
        ball_y_coord <= rack_right_y_coord + 3) {
        rack_left_y_coord -= 2;
        ball_x_speed *= -1;
    }

//    пропуск мячика левым игроком
    if (ball_x_coord < RACK_L_START_POS_X) {
        score_right += 1;

        start_data();  // ставим в начальное положение
    }
//    пропуск мячика правым игроком
    if (ball_x_coord > RACK_R_START_POS_X) {
        score_left += 1;

        start_data();  // ставим в начальное положение
    }

//    отражение мячика от стен
    if (ball_y_coord == 1 || ball_y_coord == 23) {
        ball_y_speed *= -1;
    }

//    изменение координат мячика
    ball_x_coord += ball_x_speed;
    ball_y_coord += ball_y_speed;
}

int read_symb() {
    switch (getch()) {
        case 'a':
            move_rack('a');
            break;
        case 'z':
            move_rack('z');
            break;
        case 'm':
            move_rack('m');
            break;
        case 'k':
            move_rack('k');
            break;
        default:
            return 1;
    }
    return 0;
}

void move_rack(char mov_symb) {
    if (mov_symb == 'a' && rack_left_y_coord > 1) {
        rack_left_y_coord--;
    }
    if (mov_symb == 'z' && rack_left_y_coord < 21) {
        rack_left_y_coord++;
    }
    if (mov_symb == 'k' && rack_right_y_coord > 1) {
        rack_right_y_coord--;
    }
    if (mov_symb == 'm' && rack_right_y_coord < 21) {
        rack_right_y_coord++;
    }
}

void start_data() {
    ball_x_coord = BALL_START_POS_X;
    ball_y_coord = BALL_START_POS_Y;

    ball_x_speed = BALL_START_SPD_X;
    ball_y_speed = BALL_START_SPD_Y;

    rack_right_y_coord = RACK_R_START_POS_Y;
    rack_left_y_coord = RACK_L_START_POS_Y;
}

