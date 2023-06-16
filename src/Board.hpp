#pragma once // 헤더파일이 중복돼서 빌드되는걸 막는다. 컴파일동안 단 한번만 처리하므로 더 빠른 컴파일이 가능해진다.
#include <ncurses.h>
#include "Drawable.hpp"
#include "Time.hpp"
class SnakeGame;
class Board { // 윈도우를 다루는 클래스다. 초기화부터 해서 값 입/출력, 윈도우 변수를 가진다.
public:

    Board() {
        construct(0, 0, 300); 
    }

    Board(int height, int width, int speed) {
        construct(height, width, speed);
    }

    void initialize() { // 윈도우를 안을 지우고 테두리를 다시 그려준 뒤 버퍼를 출력까지 한다.
        clear();
        refresh();
    }

    void addBorder() { // 윈도우 테두리를 그린다.
        box(board_win, 0, 0);
    }

    void add(Drawable drawable) {
        addAt(drawable.getY(), drawable.getX(), drawable.getIcon());
    }

    void addAt(int y, int x, chtype ch) { // 특정 위치에 문자열을 출력한다.
        mvwaddch(board_win, y, x, ch);
    }

    chtype getInput() { // 윈도우에 키를 입력받는다. 입력받은 값은 chtype형으로 리턴된다.
        time_t time_last_input = Time::milliseconds();
        chtype input = wgetch(board_win);
        chtype new_input = ERR;

        setTimeout(0);
        while (time_last_input + timeout >= Time::milliseconds()) {
            new_input = wgetch(board_win);
        }
        setTimeout(timeout);

        if (new_input != ERR)
            input = new_input;

        return input;
    }

    void getEmptyCoordinates(int &y, int &x) { // 윈도우의 랜덤한 장소 중 ' '인곳을 찾아서 y, x값을 전달한다.
        while((mvwinch(board_win, y = rand() % height, x = rand() % width)) != ' ')
            ;
    }

    chtype getCharAt(int y, int x) {
        return mvwinch(board_win, y, x);
    }

    void clear() { // 윈도우 안을 지우고 테두리를 다시 그려준다. clear()함수를 오버라이딩 한다.
        wclear(board_win);
        addBorder();
    }

    void refresh() { // 윈도우 버퍼 쓰인 데이터를 출력 해준다.
        wrefresh(board_win);
    }
    
    void setTimeout(int timeout) {
        wtimeout(board_win, timeout); 
    }

    int getStartRow() {
        return start_row;
    }

    int getStartCol() {
        return start_col;
    }

    int getTimeout() {
        return timeout;
    }
    int timeout;
private:
    WINDOW* board_win;
    int height, width, start_row, start_col;
    void construct(int height, int width, int speed) { // 입력받은 높이, 너비값으로 윈도우 생성
        int xMax, yMax;
        getmaxyx(stdscr, yMax, xMax); // 현재 내가 연 터미널의 세로길이와 가로길이를 1, 3번째 인수에 저장함.
        this->height = height;
        this->width = width;
        start_row = yMax / 2 - height / 2;
        start_col = xMax / 2 - width / 2;

        timeout = speed / 2;

        board_win = newwin(height, width, start_row, start_col); // 새로운 윈도우 생성, 크기는 1, 2번째 인수
        setTimeout(speed);
        keypad(board_win, true); // 방향키 입력을 가능하게 함
    }
};
