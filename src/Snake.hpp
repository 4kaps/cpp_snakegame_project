#pragma once
#include <ncurses.h>
#include "Drawable.hpp"
#include <queue>

enum Direction {
    up = -1, 
    down = 1, 
    left = -2, 
    right = 2
};

class SnakePiece : public Drawable {
public:
    SnakePiece() {
        this->x = this->y = 0;
        this->icon = '#';
    }

    SnakePiece(int y, int x) {
        this->x = x;
        this->y = y;
        this->icon = '#';
    }
};

class Snake {
    Direction cur_direction;

public:
    std::queue<SnakePiece> prev_pieces;

    Snake() {
        cur_direction = down;
    }

    void addPiece(SnakePiece piece) {
        prev_pieces.push(piece);
    }

    void removePiece() {
        prev_pieces.pop();
    }

    SnakePiece tail() {
        return prev_pieces.front(); // queue 제일 앞 원소를 반환
    }

    SnakePiece head() {
        return prev_pieces.back(); // queue 제일 뒤 원소를 반환
    }

    Direction getDirection() {
        return cur_direction;
    }

    void setDirection(Direction d) {
        if (cur_direction + d != 0)
            cur_direction = d;
    }

    void clock(Direction d) {
        if (cur_direction == up)
            cur_direction == right;
        if (cur_direction == right)
            cur_direction == down;
        if (cur_direction == down)
            cur_direction == left;
        if (cur_direction == left)
            cur_direction == up;
    }
    void reverseClock(Direction d) {
        if (cur_direction == up)
            cur_direction == left;
        if (cur_direction == left)
            cur_direction == down;
        if (cur_direction == down)
            cur_direction == right;
        if (cur_direction == right)
            cur_direction == up;
    }

    SnakePiece nextHead() {
        int row = head().getY();
        int col = head().getX();
    
        switch (cur_direction) {
        case down:
            row++;
            break;
        case up:
            row--;
            break;
        case left:
            col--;
            break;
        case right:
            col++;
            break;
        }
        return SnakePiece(row, col);
    }

    SnakePiece gateNext(int y, int x) {
        switch (cur_direction) {
        case down:
            y++;
            break;
        case up:
            y--;
            break;
        case left:
            x--;
            break;
        case right:
            x++;
            break;
        }
        return SnakePiece(y, x);
    }


};