#include <iostream>
#include <ncurses.h>
#include "src/SnakeGame.hpp"
#include "src/Board.hpp"
#include "src/Drawable.hpp"

#define BOARD_DIM 18
#define BOARD_ROWS BOARD_DIM
#define BOARD_COLS BOARD_DIM * 2


int main(int argc, char **argv)
{
    initscr();
    refresh();
    noecho();
    curs_set(0);

    SnakeGame game(BOARD_ROWS, BOARD_COLS);

    while(!game.isOver()) {
        game.processInput();

        game.updateState();

        game.redraw();
    } 

    endwin();

    std::cout << "you scored " << game.getScore() << " poins!" << std::endl;

    return 0;
}