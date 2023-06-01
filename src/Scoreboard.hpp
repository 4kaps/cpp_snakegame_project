#pragma once
#include<ncurses.h>

class Scoreboard {
    WINDOW *score_win;
public:
    Scoreboard() {
        
    }

    Scoreboard(int width, int y, int x) {
        score_win = newwin(1, width, y, x);
    }

    void initialize(int initial_score) {
        clear();
        mvwprintw(score_win, 0, 0, "Score:");
        updateScore(initial_score);
        refresh();
    }

    void updateScore(int score) { // 튜토리얼12 07:36
        mvwprintw(score_win, 0, score_win->_maxx-10, "%11llu", score);
    }

    void clear() {
        wclear(score_win);
    }

    void refresh() {
        wrefresh(score_win);
    }
};