#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MSG_GAME_OVER "GAME OVER!\n"

#define STATUS_NORMAL 0
#define STATUS_GAME_OVER 1

#define GRID_WIDTH 25
#define GRID_HEIGHT 12
#define SNAKE_MAX_LENGTH ((GRID_WIDTH-2)*(GRID_HEIGHT-2))

#define CHAR_GRID_WALL '*'
#define CHAR_GRID_BRICK '*'
#define CHAR_GRID_FOOD '$'
#define CHAR_GRID_BLANK ' '

#define CHAR_SNAKE_BODY 'X'
#define CHAR_SNAKE_HEAD 'H'

#define UP 0,-1
#define DOWN 0,1
#define LEFT -1,0
#define RIGHT 1,0

#define FOOD_PROBABILITY 0.1
#define FOOD_MAX_NUMBER (SNAKE_MAX_LENGTH / 6)
#define DEBUG 1

void initBoard(int width, int height);
int predictMovable(int dx, int dy);
void snakeMove(int dx, int dy);
void placeFood(double food_prob);
int eatFood(int dx, int dy);
void clearScreen(void);
void clearSnake(void);
void drawSnake(void);
void printGrid(void);
char readInput(void);
int gameOver(void);
char nextMove(void);
int manhattonDistance(int x1, int y1, int x2, int y2);