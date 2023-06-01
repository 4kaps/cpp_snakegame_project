#pragma once
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include "Item.hpp"
#include "Empty.hpp"
#include "Board.hpp"
#include "Drawable.hpp"
#include "Snake.hpp"
#include "Scoreboard.hpp"

class SnakeGame { // 게임의 구성에 대한 클래스다. Board클래스를 이용해 값을 입출력한다.
    Board board;
    bool game_over;
    Growth* growth[3];
    Poison* poison[3];
    int growthCount;
    int poisonCount;
    time_t growthTimer;
    time_t poisonTimer;
    Snake snake;

    Scoreboard scoreboard;
    int score;

    void handleNextPiece(SnakePiece next) {
        if (growth != NULL) {
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
            case ' ': 
            {
                emptyRow = snake.tail().getY();
                emptyCol = snake.tail().getX();
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

    void createGrowth() {
        if (growthCount < 3 && time(nullptr) - growthTimer >= 10) {
            int y, x;
            board.getEmptyCoordinates(y, x);
            growth[growthCount] = new Growth(y, x);
            board.add(*growth[growthCount]);
            growthCount++;
            growthTimer = time(nullptr);
        }
    }

    void createPoison() {
        if (poisonCount < 3 && time(nullptr) - poisonTimer >= 10) {
            int y, x;
            board.getEmptyCoordinates(y, x);
            poison[poisonCount] = new Poison(y, x);
            board.add(*poison[poisonCount]);
            poisonCount++;
            poisonTimer = time(nullptr);
        }
    }

    void eatGrowth() {
        score += 100;
        scoreboard.updateScore(score);
        delete growth[growthCount - 1];
        growth[growthCount - 1] = nullptr;
        growthCount--;
        growthTimer = time(nullptr);
    }

    void eatPoison() {
        score += 50;
        scoreboard.updateScore(score);
        delete poison[poisonCount - 1];
        poison[poisonCount - 1] = nullptr;
        poisonCount--;
        poisonTimer = time(nullptr);
    }

public:
    SnakeGame(int height, int width, int speed = 300) {
        board = Board(height, width, speed);
        int sb_row = board.getStartRow() + height;
        int sb_col = board.getStartCol();
        scoreboard = Scoreboard(width, sb_row, sb_col);
        growthCount = 0;
        poisonCount = 0;
        growthTimer = time(nullptr);
        poisonTimer = time(nullptr);
        initialize();
    }

    ~SnakeGame() {
        delete growth;
        delete poison;
    }

    void initialize() {
        growth = NULL;
        poison = NULL;
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

        if (poison == NULL) {
            createPoison();
        }

        if (growth == NULL) {
            createGrowth();
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

        if (growthCount == 0) {
            createGrowth();
        }

        if (poisonCount == 0) {
            createPoison();
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
