#pragma once
#include <ncurses.h>

class Scoreboard {
    WINDOW *score_win;
    WINDOW *time_win;
public:
    Scoreboard() {}

    Scoreboard(int width, int y, int x) {
        score_win = newwin(1, width, y, x);
        time_win = newwin(1, width, y + 1, x); 
    }

    void initialize(int initial_score) {
        clear();
        mvwprintw(score_win, 0, 0, "Score:");
        updateScore(initial_score);
        mvwprintw(time_win, 0, 0, "Distance:"); 
        refresh();
    }

    void updateScore(int score) {
        mvwprintw(score_win, 0, score_win->_maxx - 10, "%11llu", score);
    }

    void updateTime(int time) { 
        mvwprintw(time_win, 0, time_win->_maxx - 10, "%11llu", time);
    }

    void clear() {
        wclear(score_win);
        wclear(time_win);
    }

    void refresh() {
        wrefresh(score_win);
        wrefresh(time_win);
    }
};
