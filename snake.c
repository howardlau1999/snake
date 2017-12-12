#include "snake.h"

/* Initial state. */
char grid[GRID_HEIGHT][GRID_WIDTH] = {0};

int snakeX[SNAKE_MAX_LENGTH] = {1, 2, 3, 4, 5};
int snakeY[SNAKE_MAX_LENGTH] = {1, 1, 1, 1, 1};

int foodX[FOOD_MAX_NUMBER] = {0};
int foodY[FOOD_MAX_NUMBER] = {0};
int foodNumber = 0;
int snakeLength = 5;
int gameStatus = STATUS_NORMAL;

unsigned long beginTime;

/* Initialize the board! */
void initBoard(int width, int height) {
	beginTime = (unsigned) time(0);
	int y, x;
	for (y = 0; y < height; ++y) {
		for (x = 0; x < width; ++x) {
			if (y == 0 || y == height - 1 || x == 0 || x == width - 1)
				grid[y][x] = CHAR_GRID_WALL;
			else
				grid[y][x] = CHAR_GRID_BLANK;
		}
	}
}

/* Clear the screen. */
void clearScreen() {
    system("clear");
}

/* Clear the screen and print out the grid. */
void printGrid() {
    clearScreen();
    int x, y;
    for (y = 0; y < GRID_HEIGHT; ++y) {
        for (x = 0; x < GRID_WIDTH; ++x) {
            putchar(grid[y][x]);
        }
        putchar('\n');
    }
    #ifdef DEBUG
    printf("Snake Length: %d/%d\n", snakeLength, SNAKE_MAX_LENGTH);
    printf("Food Number: %d/%d\n", foodNumber, FOOD_MAX_NUMBER);
    printf("Time: %ld\n", (unsigned)time(0) - beginTime);
    printf("nextMove(): %c\n", nextMove());
    #endif
}

/* Read in only one character and discard the following characters. */
/* If no character is read, return space character. */
char readInput() {
    char ch;
    if (scanf("%c", &ch)) {
        while (getchar() != '\n') continue;
        return ch;
    } else {
        return ' ';
    }
}

/* Randomly place food at a blank place according to probability*/
void placeFood(double food_prob) {
	int x, y;

	/* Too much food! */
	if (foodNumber + 1 > FOOD_MAX_NUMBER) return;
	/* Unlucky! */
	if ((double)rand() / RAND_MAX > food_prob) return;

	do {
		x = (int)(GRID_WIDTH * ((double)rand() / RAND_MAX));
		y = (int)(GRID_HEIGHT * ((double)rand() / RAND_MAX));
	} while (grid[y][x] != CHAR_GRID_BLANK);

	foodNumber++;
	foodX[foodNumber - 1] = x;
	foodY[foodNumber - 1] = y;
    drawFood();
}

/* See if our lovely snake has eaten a food */
int eatFood(int dx, int dy) {
	/* Get future head coordinates */
	int headX = snakeX[snakeLength - 1] + dx;
	int headY = snakeY[snakeLength - 1] + dy;

	if (grid[headY][headX] == CHAR_GRID_FOOD) {
		/* Will it be too long? */
		if (snakeLength + 1 > SNAKE_MAX_LENGTH) return 0;

		/* Eat it! */
		snakeLength++;
		snakeX[snakeLength - 1] = headX;
		snakeY[snakeLength - 1] = headY;

		foodNumber--;
		int i;
		for (i = 0; i < foodNumber; ++i) {
			if (headX == foodX[i] && headY == foodY[i]) {
				foodX[i] = foodX[foodNumber];
				foodY[i] = foodY[foodNumber];
				break;
			}
		}
		return 1;
	}

	return 0;
}

/* Check if out lovely snake can make its move */
int predictMovable(int dx, int dy) {

	/* Get future head coordinates */
	int headX = snakeX[snakeLength - 1] + dx;
	int headY = snakeY[snakeLength - 1] + dy;
	int i;

	/* 1. Will it run out of bounds? */
	if (headX <= 0 || headX >= GRID_WIDTH - 1 || \
		headY <= 0 || headY >= GRID_HEIGHT - 1) {
		return 0;
	}

	/* 2. Will it hit the bricks? */
	if (grid[headY][headX] == CHAR_GRID_BRICK) {
		return 0;
	}

	/* 3. Will it bump into itself? */
	for (i = 0; i < snakeLength; ++i) {
		int bodyX = snakeX[i];
		int bodyY = snakeY[i];

		if (headX == bodyX && headY == bodyY) {
			return 0;
		}
	}

	/* Yes, it can move! */
	return 1;
}

int manhattanDist(int x1, int y1, int x2, int y2) {
	return abs(x1 - x2) + abs(y1 - y2);
}

/* Caculate and make an intelligent move */
char nextMove() {
	/* Find a direction that is near one food */
	int i = 0;
	int headX = snakeX[snakeLength - 1];
	int headY = snakeY[snakeLength - 1];
	int distMin = 9999;
	char direction = 'D';
	for (i = 0; i < foodNumber; ++i) {
		int fX = foodX[i];
		int fY = foodY[i];
		int dist = 0;
		if (predictMovable(UP)) {
			dist = manhattanDist(headX, headY - 1, fX, fY);
			if (dist < distMin) {
				distMin = dist;
				direction = 'W';
			}
		}

		if (predictMovable(DOWN)) {
			dist = manhattanDist(headX, headY + 1, fX, fY);
			if (dist < distMin) {
				distMin = dist;
				direction = 'S';
			}
		}

		if (predictMovable(LEFT)) {
			dist = manhattanDist(headX - 1, headY, fX, fY);
			if (dist < distMin) {
				distMin = dist;
				direction = 'A';
			}
		}

		if (predictMovable(RIGHT)) {
			dist = manhattanDist(headX + 1, headY, fX, fY);
			if (dist < distMin) {
				distMin = dist;
				direction = 'D';
			}
		}
	}

	if (distMin == 9999) {
		if (predictMovable(RIGHT)) direction = 'D';
		if (predictMovable(LEFT)) direction = 'A';
		if (predictMovable(UP)) direction = 'W';
		if (predictMovable(DOWN)) direction = 'S';
	}

	return direction;
}

/* Clear the snake */
void clearSnake() {
	int i;
    for (i = 0; i < snakeLength; ++i) {
        int x = snakeX[i];
        int y = snakeY[i];
        grid[y][x] = CHAR_GRID_BLANK;
    }
}
/* Draw food */
void drawFood() {
	int i;
	for (i = 0; i < foodNumber; ++i) {
		int y = foodY[i];
		int x = foodX[i];
		grid[y][x] = CHAR_GRID_FOOD;
	}
}

/* Draw the snake */
void drawSnake() {
	int i;
    for (i = 0; i < snakeLength; ++i) {
        int x = snakeX[i];
        int y = snakeY[i];
        grid[y][x] = (snakeLength - 1) == i ? CHAR_SNAKE_HEAD : CHAR_SNAKE_BODY;
    }
}

/* Move the snake one step according to dx and dy */
void snakeMove(int dx, int dy) {
	if (!predictMovable(dx, dy)) {
		gameStatus = STATUS_GAME_OVER;
		return;
	}
	
	int i;
    
	clearSnake();

	if (!eatFood(dx, dy)) {
		/* Propagate the movement first */
	    for (i = 0; i < snakeLength - 1; ++i) {
	        snakeX[i] = snakeX[i + 1];
	        snakeY[i] = snakeY[i + 1];
	    }

	    /* Set the new head */
	    snakeX[snakeLength - 1] = snakeX[snakeLength - 1] + dx;
	    snakeY[snakeLength - 1] = snakeY[snakeLength - 1] + dy;
	}
    
    drawSnake();
    
}

int gameOver() {
    return gameStatus == STATUS_GAME_OVER;
}

