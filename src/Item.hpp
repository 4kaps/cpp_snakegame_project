#pragma once
#include <ncurses.h>
#include "Drawable.hpp"

class Growth : public Drawable {
public:
    Growth(int y, int x) {
        this->y = y;
        this->x = x;
        this->icon = '+';
    }

};

class Poison : public Drawable {
public:
    Poison(int y, int x) {
        this->y = y;
        this->x = x;
        this->icon = '-';
    }

};

class Special : public Drawable {
public:
    Special(int y, int x) {
        this->y = y;
        this->x = x;
        this->icon = '?';
    }

};

