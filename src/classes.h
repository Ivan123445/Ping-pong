#ifndef PING_PONG_CLASSES_H
#define PING_PONG_CLASSES_H

#include <ncurses.h>

#define RACK_X_OFFSET 4

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
    int field_y;
    int left_y_coord, right_y_coord;
    int left_x_coord, right_x_coord;

public:
    Rackets() = delete;

    Rackets(int width, int field_y, int y_coord, int left_x, int right_x)
            :width(width), field_y(field_y), left_y_coord(y_coord), right_y_coord(y_coord), left_x_coord(left_x), right_x_coord(right_x)
    {}

    void init(int y_coord) {left_y_coord = right_y_coord = y_coord; }
    int move(int left_offset, int right_offset);

    [[nodiscard]] int getLeftYCoord() const { return left_y_coord; }
    [[nodiscard]] int getRightYCoord() const { return right_y_coord; }
    [[nodiscard]] int getLeftXCoord() const { return left_x_coord; }
    [[nodiscard]] int getRightXCoord() const { return right_x_coord; }
    [[nodiscard]] int getWidth() const {return width; }
};

class Game {
    int field_x, field_y;
    Ball ball;
    Rackets rackets;
    int rack_l_offset, rack_r_offset;
    int win_score;
    int score_left, score_right;

public:
    Game() = delete;

    Game(int win_score, int field_x, int field_y, int rack_width);

    int init();
    int render();
    int read_symb();
    int next_step();
    int win_screen();

    [[nodiscard]] bool win() const { return !(score_left < win_score && score_right < win_score); }
};

#endif //PING_PONG_CLASSES_H
