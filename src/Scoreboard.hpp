#pragma once
#include <ncurses.h>

class Scoreboard {
    WINDOW *score_win;
    WINDOW *time_win;
    WINDOW *speed_win;
    WINDOW *out_line;
public:
    Scoreboard() {}

    Scoreboard(int width, int y, int x) {
        score_win = newwin(1, width, y - 13, x + 38);
        time_win = newwin(1, width, y - 12, x + 38);
        speed_win = newwin(1, width, y - 11, x + 38);
        out_line = newwin(5, width + 3, y - 14, x + 37);
    }

    void initialize(int initial_score) {
        clear();
        box(out_line, 0, 0);
        mvwprintw(score_win, 0, 0, "Score:");
        updateScore(initial_score);
        mvwprintw(time_win, 0, 0, "Distance:"); 
        mvwprintw(speed_win, 0, 0, "Speed:"); 
        refresh();
    }

    void updateScore(int score) {
        mvwprintw(score_win, 0, score_win->_maxx - 10, "%11llu", score);
    }

    void updateTime(int time) { 
        mvwprintw(time_win, 0, time_win->_maxx - 10, "%11llu", time);
    }

    void updateSpeed(double speed){
        mvwprintw(speed_win, 0, speed_win->_maxx - 10, "%11.2lf", speed);
    }

    void clear() {
        wclear(out_line);
        wclear(score_win);
        wclear(time_win);
        wclear(speed_win);
    }

    void refresh() {
        wrefresh(out_line);
        wrefresh(score_win);
        wrefresh(time_win);
        wrefresh(speed_win);
    }
};
