#pragma once
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include "Apple.hpp"
#include "Poison.hpp"
#include "Empty.hpp"
#include "Board.hpp"
#include "Drawable.hpp"
#include "Snake.hpp"
#include "Scoreboard.hpp"

class SnakeGame { // 게임의 구성에 대한 클래스다. Board클래스를 이용해 값을 입출력한다.
    Board board;
    bool game_over;
    Apple* apple;
    Snake snake;
    Poison* poison;

    Scoreboard scoreboard;
    int score;

    void handleNextPiece(SnakePiece next) {
        if (apple != NULL) {
            switch (board.getCharAt(next.getY(), next.getX()));
            {
            case '+':
            {
                eatApple();
                break;
            }
            case '-':
            {
                eatPoison();
                break;
            }
            case ' ': 
            {
                int emptyRow = snake.tail().getY();
                int emptyCol = snake.tail().getX();
                board.add(Empty(emptyRow, emptyCol));
                snake.removePiece();
                break;
            }
            default:
                game_over = true;
                break;

            }
        }
        board.add(next);
        snake.addPiece(next);
    }

    void createApple() {
        int y, x;
        board.getEmptyCoordinates(y, x);
        apple = new Apple(y, x);
        board.add(*apple);
    }
    
    void createPoison() {
    }

    void eatApple() {
        delete apple;
        apple = NULL;
        score += 100;
        scoreboard.updateScore(score);
    }
    

public:
    SnakeGame(int height, int width, int speed = 300) {
        board = Board(height, width, speed);
        int sb_row = board.getStartRow() + height;
        int sb_col = board.getStartCol();
        scoreboard = Scoreboard(width, sb_row, sb_col);
        initialize();
    }

    ~SnakeGame() {
        delete apple;
        delete poison;
    }

    void initialize() {
        apple = NULL;
        posion = NULL;
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

        if (item == NULL) {
            createItem();
        }
    }

    void processInput() {
        chtype input = board.getInput();
        int old_timeout = board.getTimeout();
        switch (input) {
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
            while (board.getInput() != 'p');
            board.setTimeout(old_timeout);
            break;
        default:
            break;
        }
    }

    void updateState() {
        handleNextPiece(snake.nextHead());
        
        if (item == NULL) {
            createItem();
        }
    }

    void redraw() {
        board.refresh();
        scoreboard.refresh();
    }

    bool isOver() {
        return game_over;
    }

    int getScore() {
        return score;
    }
};
