#pragma once
#include <ncurses.h>
#include "Drawable.hpp"

class Item : public Drawable {
public:
    Item(int y, int x) {
        this->y = y;
        this->x = x;
        this->icon = '+';
    }

};