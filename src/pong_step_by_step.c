#include <stdio.h>

// мячик
#define BALL_START_POS_X 40
#define BALL_START_POS_Y 6
#define BALL_START_SPD_X -1
#define BALL_START_SPD_Y -1

// ракетки
#define RACK_L_START_POS_X 4
#define RACK_L_START_POS_Y 11
#define RACK_R_START_POS_X 76
#define RACK_R_START_POS_Y 11

// настройки
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

    while (score_right != WIN_SCORE && score_left != WIN_SCORE) {
        printf("\033[0d\033[2J");

        input_field_to_screen();  //        отрисовываем счет поле

        if (read_symb() == 0) {  //        считываем и если символы
                                //         корректные перемещаем ракеток(в read_symb) и мяч
            move_ball();
        }
    }

    win_screen();

    return 0;
}

void win_screen() {
//  рисуем новое поле
    for (int y = 0; y < 25; ++y) {
        for (int x = 0; x < 80; ++x) {
            if ((x == 0 || x == 79) && y != 24 && y != 0) {
                printf("║");
                continue;
            }
            if ((y == 0 || y == 24) && x != 79 && x != 0) {
                printf("=");
                continue;
            }
//          Углы
            if (y == 0 && x == 0) {
                printf("╔");
                continue;
            }
            if (y == 24 && x == 0) {
                printf("╚");
                continue;
            }
            if (y == 0 && x == 79) {
                printf("╗");
                continue;
            }
            if (y == 24 && x == 79) {
                printf("╝");
                continue;
            }
//          Поздравление
            if (y == 9 && x == 32) {
                printf("\033[0;35mCONGRATULATIONS!\033[0;37m");
                x += 15;
                continue;
            }
//          Выводим, кто победитель
            if (y == 12 && x == 23) {
                if (score_left == WIN_SCORE) {
                    printf("\033[0;36mPlayer left, you are the winner!\033[0;37m");
                    x += 31;
                    continue;
                } else {
                    printf("\033[0;36mPlayer right, you are the winner!\033[0;37m");
                    x += 32;
                    continue;
                }
            }
//          Выводим счет
            if (y == 15 && x == 37) {
                    printf("\033[0;31m%d : %d\033[0;37m", score_left, score_right);
                    x += 4;
                    continue;
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void input_field_to_screen() {
//    выводим счет
    printf("                                      %d | %d \n", score_left, score_right);

//    выводим остальное поле
    for (int y = 0; y < 25; ++y) {
        for (int x = 0; x < 80; ++x) {
            if ((x == 1 || x == 79) && y != 0) {
                printf("|");
            } else {
                if ((y == 0 || y == 24) && x != 0) {
                    printf("_");
                } else {
//          отрисовка левой ракетки
                    if (x == RACK_L_START_POS_X && y >= rack_left_y_coord && y <= rack_left_y_coord + 2)
                        printf("▐");
                    else
//          отрисовка правой ракетки
                    if (x == RACK_R_START_POS_X && y >= rack_right_y_coord && y <= rack_right_y_coord + 2)
                        printf("▐");
                    else
//          отрисовка мячика
                    if (ball_y_coord == y && ball_x_coord == x)
                        printf("o");
                    else if (x == 40)
                        printf("|");
                    else
                        printf(" ");
                }}
        }
        printf("\n");
    }
}

void move_ball() {
//    отражение мячика от ракеток
    if (ball_x_coord == RACK_L_START_POS_X+1 &&
            ball_y_coord >= rack_left_y_coord && ball_y_coord <= rack_left_y_coord + 3) {
        ball_x_speed *= -1;
    }
    if (ball_x_coord == RACK_R_START_POS_X-1 &&
            ball_y_coord >= rack_right_y_coord && ball_y_coord <= rack_right_y_coord + 3) {
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
     if (ball_y_coord == 1 || ball_y_coord == 24) {
        ball_y_speed *= -1;
    }

//    изменение координат мячика
    ball_x_coord += ball_x_speed;
    ball_y_coord += ball_y_speed;
}

int read_symb() {
    char symb_one, symb_two;

//      читаем и проверяем, что передано 2 символа
    if (scanf("%c%c", &symb_one, &symb_two) != 2 || getchar() != '\n') {
        char trash = getchar();
        for (; trash != '\n'; trash = getchar()) {}
        return 1;
    }

//    проверки для 1 игрока
    if (symb_one == 'a' || symb_one == 'z'
        || symb_one == 'k' || symb_one == 'm' || symb_one == ' ') {
        move_rack(symb_one);
    } else {
        char trash = getchar();
        for (; trash != '\n'; trash = getchar()) {}
        return 1;
    }
//    проверки для 2 игрока
    if (symb_two == 'a' || symb_two == 'z'
        || symb_two == 'k' || symb_two == 'm' || symb_two == ' ') {
        move_rack(symb_two);
    } else {
        char trash = getchar();
        for (; trash != '\n'; trash = getchar()) {}
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
