#include <unistd.h>
#include <thread>

#include "classes.h"

// ширина ракеток
#define RACK_WIGHT 4

// поле
#define FIELD_X 80
#define FIELD_Y 25

// скорость игры
#define TIME_ONE_STEP 70000
#define TIME_ONE_FRAME 70000
#define WIN_SCORE 24


int main() {
    Game game(WIN_SCORE, FIELD_X, FIELD_Y, RACK_WIGHT);

    initscr();
    nodelay(curscr, true);

    std::thread th_render([&]{
        while (!game.win()) {
            game.render();
            usleep(TIME_ONE_FRAME);
        }
    });

    std::thread th_read_symb([&]{
        while (!game.win()) game.read_symb();
    });

    std::thread th_next_step([&]{
        while (!game.win()) {
            game.next_step();
            usleep(TIME_ONE_STEP);
        }
    });


    th_render.join();
    th_read_symb.join();
    th_next_step.join();

    game.win_screen();
    getchar();

    endwin();

    return 0;
}
