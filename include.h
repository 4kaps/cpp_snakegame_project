#ifndef INCLUDE_H
#define INCLUDE_H
#include <iostream>
#include <conio.h>

class Game{
public: 
    Game();
    void Run();
private:
    bool gameOver;
    int x, y, fruit, poison, score;
    const int width;
    const int length;
};

#endif