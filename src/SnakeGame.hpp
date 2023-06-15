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
#include <cstdlib> // rand(), srand()을 사용하기 위해 필요한 헤더
#include <ctime> // srand()을 초기화하기 위해 필요한 헤더


class SnakeGame
{ // 게임의 구성에 대한 클래스다. Board클래스를 이용해 값을 입출력한다.
    Board board;
    bool game_over;
   
//////////////////////////////////////////////////////////////// {17,34}는 [105][1]

    Growth *growth;
    Poison *poison;
    Special *special;
    Wall *wall;
    immuneWall *immunewall;
    Snake snake;
    

    Scoreboard scoreboard;
    int score;

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

            default:
                game_over = true;
                break;
            }
        }
        board.add(next);
        snake.addPiece(next);
    }

    void createGrowth()
    {
        int y, x;
        board.getEmptyCoordinates(y, x);
        growth = new Growth(y, x);
        board.add(*growth);
    }

    void createPoison()
    {
        int y, x;
        board.getEmptyCoordinates(y, x);
        poison = new Poison(y, x);
        board.add(*poison);
    }

    void createSpecial()
    {
        int y, x;
        board.getEmptyCoordinates(y, x);
        special = new Special(y, x);
        board.add(*special);
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
        
        Gate g1(gate1Choice), g2(gate2Choice);

        board.add(*g1);
        board.add(*g2);

        }
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
    
        void createGate();
        
    
    
    }

    void processInput()
    {
        chtype input = board.getInput();
        int old_timeout = board.getTimeout();
        switch (input)
        {
        case KEY_UP:
        case 'w':
            snake.setDirection(up);
            break;
        case KEY_DOWN:
        case 's':
            snake.setDirection(down);
            break;
        case KEY_RIGHT:
        case 'd':
            snake.setDirection(right);
            break;
        case KEY_LEFT:
        case 'a':
            snake.setDirection(left);
            break;
        case 'p':
            board.setTimeout(-1);
            while (board.getInput() != 'p')
                ;
            board.setTimeout(old_timeout);
            break;
        default:
            break;
        }
    }

    void updateState()
    {
        handleNextPiece(snake.nextHead());
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
