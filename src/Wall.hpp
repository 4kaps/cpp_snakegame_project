#pragma once
#include <ncurses.h>
#include "Drawable.hpp"

class Wall : public Drawable {
public:

    Wall(int y, int x) {
        this->y = y;
        this->x = x;
        this->icon = 'w';
    }
};

class immuneWall : public Drawable {
public:

    immuneWall(int y, int x) {
        this->y = y;
        this->x = x;
        this->icon = 'W';
    }
};
