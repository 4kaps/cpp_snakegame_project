#pragma once
#include <ncurses.h>

class Scoreboard {
    WINDOW *score_win;
    WINDOW *time_win;
    WINDOW *speed_win;
    WINDOW *length_win;
    WINDOW *growth_win;
    WINDOW *poison_win;
    WINDOW *out_line1;

    WINDOW *mission_box_name;
    WINDOW *mission_length;
    WINDOW *mission_growth;
    WINDOW *mission_poison;
    WINDOW *out_line2;
public:
    Scoreboard() {}

    Scoreboard(int width, int y, int x) {
        score_win = newwin(1, width, y - 17, x + 38);
        time_win = newwin(1, width, y - 16, x + 38);
        speed_win = newwin(1, width, y - 15, x + 38);
        length_win = newwin(1, width, y - 14, x + 38);
        growth_win = newwin(1, width, y - 13, x + 38);
        poison_win = newwin(1, width, y - 12, x + 38);
        out_line1 = newwin(8, width + 2, y - 18, x + 37);
        
        mission_box_name = newwin(1, width, y - 6, x + 38);
        mission_length = newwin(1, width, y - 5, x + 38);
        mission_growth = newwin(1, width, y - 4, x + 38);
        mission_poison = newwin(1, width, y - 3, x + 38);
        out_line2 = newwin(6, width + 2, y - 7, x + 37);
    }

    void initialize(int initial_score) {
        clear();
        box(out_line1, 0, 0);
        box(out_line2, 0, 0);
        mvwprintw(score_win, 0, 0, "Score :");
        updateScore(initial_score);
        mvwprintw(time_win, 0, 0, "Distance :"); 
        mvwprintw(speed_win, 0, 0, "Speed :"); 
        mvwprintw(length_win, 0, 0, "Length :"); 
        mvwprintw(growth_win, 0, 0, "+ :"); 
        mvwprintw(poison_win, 0, 0, "- :"); 

        mvwprintw(mission_box_name, 0, 0, "< Mission >"); 
        mvwprintw(mission_length, 0, 0, "Length : 10");
        mvwprintw(mission_growth, 0, 0, "+ : 5"); 
        mvwprintw(mission_poison, 0, 0, "- : 2"); 
        refresh();
    }

    void updateScore(int score) {
        mvwprintw(score_win, 0, score_win->_maxx - 20, "%11llu", score);
    }

    void updateTime(int time) { 
        mvwprintw(time_win, 0, time_win->_maxx - 20, "%11llu", time);
    }

    void updateSpeed(double speed){
        mvwprintw(speed_win, 0, speed_win->_maxx - 20, "%11.2lf", speed);
    }

    void updateLength(int length) { 
        mvwprintw(length_win, 0, length_win->_maxx - 20, "%11llu", length);
    }

    void updateGrowth(int growth) { 
        mvwprintw(growth_win, 0, growth_win->_maxx - 20, "%11llu", growth);
    }

    void updatePoison(int poison) { 
        mvwprintw(poison_win, 0, poison_win->_maxx - 20, "%11llu", poison);
    }

    void updateMissionLength(char isComplete1){
        mvwprintw(mission_length, 0, mission_length->_maxx - 20, "(%c)", isComplete1);
    }

    void updateMissionGrowth(char isComplete2){
        mvwprintw(mission_growth, 0, mission_growth->_maxx - 20, "(%c)", isComplete2);
    }

    void updateMissionPoison(char isComplete3){
        mvwprintw(mission_poison, 0, mission_poison->_maxx - 20, "(%c)", isComplete3);
    }

    void clear() {
        wclear(out_line1);
        wclear(out_line2);
        wclear(score_win);
        wclear(time_win);
        wclear(speed_win);
        wclear(length_win);
        wclear(growth_win);
        wclear(poison_win);

        wclear(mission_box_name);
        wclear(mission_growth);
        wclear(mission_length);
        wclear(mission_poison);
    }

    void refresh() {
        wrefresh(out_line1);
        wrefresh(out_line2);
        wrefresh(score_win);
        wrefresh(time_win);
        wrefresh(speed_win);
        wrefresh(length_win);
        wrefresh(growth_win);
        wrefresh(poison_win);

        wrefresh(mission_box_name);
        wrefresh(mission_growth);
        wrefresh(mission_length);
        wrefresh(mission_poison);
    }
};
