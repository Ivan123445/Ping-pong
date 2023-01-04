#include <cstdio>
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
#define RACK_WIGHT         3

// скорость игры
#define TIME_ONE_STEP 70000
#define WIN_SCORE 24

class Ball {
    int x_coord, y_coord;
    int x_speed, y_speed;

public:
    Ball() = delete;

    Ball(int x_coord, int y_coord)
        :x_coord(x_coord), y_coord(y_coord)
    {
        x_speed = 1;
        y_speed = 1;
    }

    void init(int x, int y) { x_coord = x; y_coord = y; }
    void move() { x_coord += x_speed; y_coord += y_speed; }

    void reflect_y() { y_speed *= -1; }
    void reflect_x() { x_speed *= -1; }

    [[nodiscard]] int getXCoord() const { return x_coord; }
    [[nodiscard]] int getYCoord() const { return y_coord; }
};

class Rackets {
    int width;
    int left_y_coord, right_y_coord;

public:
    Rackets() = delete;

    Rackets(int width, int left_y_coord, int right_y_coord)
        :width(width), left_y_coord(left_y_coord), right_y_coord(right_y_coord)
    {}

    void init(int left_y, int right_y) {left_y_coord = left_y; right_y_coord = right_y; }
    int move(int left_offset, int right_offset);

    [[nodiscard]] int getLeftYCoord() const { return left_y_coord; }
    [[nodiscard]] int getRightYCoord() const { return right_y_coord; }
    [[nodiscard]] int getWidth() const {return width; }
};

class Game {
    Ball ball;
    Rackets rackets;
    int rack_l_offset, rack_r_offset;
    int score_left, score_right;

public:
    Game() = delete;
    Game(int ball_x, int ball_y, int rack_width, int rack_l_coord, int rack_r_coord);

    int render();
    int next_step();
    int read_symb();
    int init();
    bool win() { return !(score_left < WIN_SCORE && score_right < WIN_SCORE); }
};

int main() {
    Game game(BALL_START_POS_X, BALL_START_POS_Y, RACK_WIGHT,
                    RACK_L_START_POS_Y, RACK_R_START_POS_Y); //   начальные позиции мяча и ракеток

    initscr();
    nodelay(stdscr, true);

    while (!game.win()) {
        // бесконечный цикл, крутится без остановки пока не достигнем счета в 24
        clear();
        game.render();
        refresh();

        game.read_symb();

        game.next_step();

        usleep(TIME_ONE_STEP);
    }

    clear();
// TODO WinScreen
    refresh();
    getchar();

    endwin();

    return 0;
}

Game::Game(int ball_x, int ball_y, int rack_width, int rack_l_coord, int rack_r_coord)
    : ball(ball_x, ball_y), rackets(rack_width, rack_l_coord, rack_r_coord)
{
    score_left = 0;
    score_right = 0;
}

int Game::render() {
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
                    if (x == RACK_L_START_POS_X && y >= rackets.getLeftYCoord()
                                            && y <= rackets.getLeftYCoord() + rackets.getWidth())
                        printw("|");
                    else
//          отрисовка правой ракетки
                    if (x == RACK_R_START_POS_X && y >= rackets.getRightYCoord()
                                            && y <= rackets.getRightYCoord() + rackets.getWidth())
                        printw("|");
                    else
//          отрисовка мячика
                    if (ball.getYCoord() == y && ball.getXCoord() == x)
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
    return 0;
}

int Game::next_step() {
    rackets.move(rack_l_offset, rack_r_offset);
    rack_l_offset = 0;
    rack_r_offset = 0;
    ball.move();

    //    отражение мячика от ракеток
    if (ball.getXCoord() == RACK_L_START_POS_X + 1 && ball.getYCoord() >= rackets.getLeftYCoord() &&
            ball.getYCoord() <= rackets.getLeftYCoord() + rackets.getWidth()) {
        ball.reflect_x();
    }
    if (ball.getXCoord() >= RACK_R_START_POS_X - 1 && ball.getYCoord() >= rackets.getRightYCoord() &&
            ball.getYCoord() <= rackets.getRightYCoord() + rackets.getWidth()) {
//        rack_left_y_coord -= 2;
        ball.reflect_x();
    }

//    пропуск мячика левым игроком
    if (ball.getXCoord() < RACK_L_START_POS_X) {
        score_right += 1;

        init();  // ставим в начальное положение
    }
//    пропуск мячика правым игроком
    if (ball.getXCoord() > RACK_R_START_POS_X) {
        score_left += 1;

        init();  // ставим в начальное положение
    }

//    отражение мячика от стен
    if (ball.getYCoord() == 1 || ball.getYCoord() == 23) {
        ball.reflect_y();
    }

    return 0;
}

int Game::read_symb() {
    switch (getch()) {
        case 'a':
            rack_l_offset += 1;
            break;
        case 'z':
            rack_l_offset -= 1;
            break;
        case 'm':
            rack_r_offset += 1;
            break;
        case 'k':
            rack_r_offset -= 1;
            break;
        default:
            return 1;
    }
    return 0;
}

int Game::init() {
    ball.init(BALL_START_POS_X, BALL_START_POS_Y);
    rackets.init(RACK_L_START_POS_Y, RACK_R_START_POS_Y);

    return 0;
}

int Rackets::move(int left_offset, int right_offset) {
    left_y_coord += left_offset;
    right_y_coord += right_offset;

    left_y_coord < 0 ? left_y_coord = 0 : 0;
    right_y_coord < 0 ? right_y_coord = 0 : 0;
    return 0;
}
