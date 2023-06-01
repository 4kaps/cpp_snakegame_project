#pragma once
#include <ncurses.h>
#include "Drawable.hpp"

class Poison : public Drawable {
public:
    Poison(int y, int x) {
        this->y = y;
        this->x = x;
        this->icon = '-';
    }
};