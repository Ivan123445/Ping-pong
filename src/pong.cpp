#include <cstdio>
#include <unistd.h>
#include <ncurses.h>

#include "classes.h"

// ширина ракеток
#define RACK_WIGHT 4

// поле
#define FIELD_X 80
#define FIELD_Y 25

// скорость игры
#define TIME_ONE_STEP 70000
#define WIN_SCORE 24


int main() {
    Game game(WIN_SCORE, FIELD_X, FIELD_Y, RACK_WIGHT);

    initscr();
    nodelay(stdscr, true);

    while (!game.win()) {
        clear();
        game.render();
        refresh();

        game.read_symb();
        game.next_step();

        usleep(TIME_ONE_STEP);
    }

    clear();
    game.win_screen();
    refresh();
    getchar();

    endwin();

    return 0;
}
