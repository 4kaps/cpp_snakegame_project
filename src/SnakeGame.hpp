#pragma once
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include "Item.hpp"
#include "Wall.hpp"
#include "Empty.hpp"
#include "Board.hpp"
#include "Drawable.hpp"
#include "Snake.hpp"
#include "Scoreboard.hpp"
#include "Gate.hpp"

class SnakeGame
{ // 게임의 구성에 대한 클래스다. Board클래스를 이용해 값을 입출력한다.
    Board board;
    bool game_over;
    Growth *growth;
    Poison *poison;
    Special *special;
    Wall *wall;
    Gate *g1;
    Gate *g2;
    immuneWall *immunewall;
    Snake snake;
    time_t specialTimer;
    time_t growthTimer;
    time_t poisonTimer;

    Scoreboard scoreboard;
    int score;
    int elapsed = 0;

    void handleNextPiece(SnakePiece next)
    {
        if (growth != NULL)
        {
            switch (board.getCharAt(next.getY(), next.getX()))
            {
                int emptyRow, emptyCol;

            case '+':
                eatGrowth();
                break;
            case '-':
                eatPoison();
                emptyRow = snake.tail().getY();
                emptyCol = snake.tail().getX();
                board.add(Empty(emptyRow, emptyCol));
                snake.removePiece();
                emptyRow = snake.tail().getY();
                emptyCol = snake.tail().getX();
                board.add(Empty(emptyRow, emptyCol));
                snake.removePiece();
                break;
            case '?':
                eatSpecial();
                emptyRow = snake.tail().getY();
                emptyCol = snake.tail().getX();
                board.add(Empty(emptyRow, emptyCol));
                snake.removePiece();
                break;
            case ' ':
                emptyRow = snake.tail().getY();
                emptyCol = snake.tail().getX();
                board.add(Empty(emptyRow, emptyCol));
                snake.removePiece();
                break;
            case 'w':
                game_over = true;
                break;
            case 'G': // G에 닿게 되면 (5, 5)로 나오게 구현한 상태
                emptyRow = snake.tail().getY();
                emptyCol = snake.tail().getX();
                board.add(Empty(emptyRow, emptyCol));
                snake.removePiece();
                break;
            default:
                game_over = true;
                break;
            }
        }
        if (board.getCharAt(next.getY(), next.getX()) == 'G') {
            board.add(SnakePiece(5, 5));
            snake.addPiece(SnakePiece(5, 5));
        }
        else {
            board.add(next);
            snake.addPiece(next);
        }
    }

    void deleteGrowth(){
        board.add(Empty(growth->getY(), growth->getX()));
        delete growth;
        createGrowth();
    }

    void deletePoison(){
        board.add(Empty(poison->getY(), poison->getX()));
        delete poison;
        createPoison();
    }

    void deleteSpecial(){
        board.add(Empty(special->getY(), special->getX()));
        delete special;
        createSpecial();
    }

    void createGrowth()
    {
        int y, x;
        board.getEmptyCoordinates(y, x);
        growth = new Growth(y, x);
        board.add(*growth);
        time(&growthTimer);
    }

    void createPoison()
    {
        int y, x;
        board.getEmptyCoordinates(y, x);
        poison = new Poison(y, x);
        board.add(*poison);
        time(&poisonTimer);
    }

    void createSpecial()
    {
        int y, x;
        board.getEmptyCoordinates(y, x);
        special = new Special(y, x);
        board.add(*special);
        time(&specialTimer);
    }

    void eatGrowth()
    {
        delete growth;
        growth = NULL;
        score += 100;
        scoreboard.updateScore(score);
    }

    void eatPoison()
    {
        delete poison;
        poison = NULL;
        score += 50;
        scoreboard.updateScore(score);
    }

    void eatSpecial()
    {
        delete special;
        special = NULL;
        int old_timeout = board.getTimeout();
        board.timeout /= 1.5;
        scoreboard.updateScore(score);
    }

    void createGate()
    {
        int gate1Choice;
        int gate2Choice;
        srand(static_cast<unsigned int>(time(0))); // 시간을 기반으로 srand()을 초기화
        gate1Choice = rand() % 106; // 0부터 105까지의 난수 생성
        do {
            gate2Choice = rand() % 106; // 두 번째 랜덤 숫자
        } while (gate1Choice == gate2Choice); // 두 번째 숫자가 첫 번째 숫자와 같을 경우 반복
        
        g1 = new Gate(gate1Choice);
        g2 = new Gate(gate2Choice);

        board.add(*g1);
        board.add(*g2);

        }

public:
    SnakeGame(int height, int width, int speed = 300)
    {
        board = Board(height, width, speed);
        int sb_row = board.getStartRow() + height;
        int sb_col = board.getStartCol();
        scoreboard = Scoreboard(width, sb_row, sb_col);
        initialize();
    }

    ~SnakeGame()
    {
        delete growth;
        delete poison;
        delete special;
    }

    void initialize()
    {
        growth = NULL;
        poison = NULL;
        special = NULL;
        board.initialize();
        score = 0;
        scoreboard.initialize(score);
        game_over = false;
        srand(time(NULL));
        snake.setDirection(down);

        handleNextPiece(SnakePiece(1, 1));
        handleNextPiece(snake.nextHead());
        handleNextPiece(snake.nextHead());
        snake.setDirection(right);
        handleNextPiece(snake.nextHead());

        if (special == NULL)
        {
            createSpecial();
        }

        if (poison == NULL)
        {
            createPoison();
        }

        if (growth == NULL)
        {
            createGrowth();
        }

        // wall 추가
        for (int i = 0; i <= 17; i++)
        {
            for (int j = 0; j <= 35; j++)
            {
                // 끝의 모서리 4부분 -> immuneWall
                if (i == 0 && (j == 0 || j == 35) || i == 17 && (j == 0 || j == 35)) {
                    immunewall = new immuneWall(i,j);
                    board.add(*immunewall);
                }
                // 변이 아닌 가운데부분 continue
                else if (i > 0 && i < 17 && j > 0 && j < 35)
                    continue;
                else {
                wall = new Wall(i, j);
                board.add(*wall);
                }
            }
        }

        for (int j = 10; j <= 25; j++)
        {
            wall = new Wall(6, j);
            board.add(*wall);
            wall = new Wall(12, j);
            board.add(*wall);
        }

        createGate();
    }

    void processInput()
    {
        chtype input = board.getInput();
        int old_timeout = board.getTimeout();
        switch (input)
        {
        case KEY_UP:
        case 'w':
            if(snake.getDirection() == down){
                game_over = true;
                break;
            }else{
                snake.setDirection(up);
                break; 
            }
        case KEY_DOWN:
        case 's':
            if(snake.getDirection() == up){
                game_over = true;
                break;
            }else{
                snake.setDirection(down);
                break; 
            }
        case KEY_RIGHT:
        case 'd':
            if(snake.getDirection() == left){
                game_over = true;
                break;
            }else{
                snake.setDirection(right);
                break; 
            }
        case KEY_LEFT:
        case 'a':
            if(snake.getDirection() == right){
                game_over = true;
                break;
            }else{
                snake.setDirection(left);
                break; 
            }
        case 'p':
            board.setTimeout(-1);
            while (board.getInput() != 'p')
                ;
            board.setTimeout(old_timeout);
            break;
        case 'n':
            deleteGrowth();
            break;
        default:
            break;
        }
    }

    void updateState()
    {
        handleNextPiece(snake.nextHead());
        time_t currentTime;
        time(&currentTime);

        if (growth != NULL && difftime(currentTime, growthTimer) >= 10) {
        deleteGrowth();
        }

        if (poison != NULL && difftime(currentTime, poisonTimer) >= 13) {
        deletePoison();
        }

        if (special != NULL && difftime(currentTime, specialTimer) >= 15) {
        deleteSpecial();
        }
        if (special == NULL)
        {
            createSpecial();
        }

        if (growth == NULL)
        {
            createGrowth();
        }

        if (poison == NULL)
        {
            createPoison();
        }

        if(snake.prev_pieces.size() < 3){
            game_over = true;
        }
        scoreboard.updateTime(elapsed++);
    }

    void redraw()
    {
        board.refresh();
        scoreboard.refresh();
    }

    bool isOver()
    {
        return game_over;
    }

    int getScore()
    {
        return score;
    }
};
