#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 24
#define BRICK_ROWS 5
#define BRICK_COLS 18
#define BRICK_WIDTH 3

char map[SCREEN_HEIGHT][SCREEN_WIDTH];
int bricks[BRICK_ROWS][BRICK_COLS];

int paddleX, paddleY, paddleSize = 10;
int ballX, ballY, ballDX, ballDY;
int score = 0, remainingBricks = BRICK_ROWS * BRICK_COLS;
int isPaused = 0;

void gotoxy(int x, int y) {
    COORD c = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void setup() {
    srand(time(NULL));
    paddleX = SCREEN_WIDTH / 2 - paddleSize / 2;
    paddleY = SCREEN_HEIGHT - 2;

    ballX = SCREEN_WIDTH / 2;
    ballY = SCREEN_HEIGHT - 3;
    ballDX = (rand() % 2 == 0) ? -1 : 1;
    ballDY = -1;

    for(int i=0;i<BRICK_ROWS;i++)
        for(int j=0;j<BRICK_COLS;j++)
            bricks[i][j] = 1;
}

void clearMap() {
    for(int i=0;i<SCREEN_HEIGHT;i++)
        for(int j=0;j<SCREEN_WIDTH;j++)
            map[i][j] = ' ';
}

void drawBorders() {
    for(int i=0;i<SCREEN_WIDTH;i++)
        map[0][i] = map[SCREEN_HEIGHT-1][i] = '#';
    for(int i=0;i<SCREEN_HEIGHT;i++)
        map[i][0] = map[i][SCREEN_WIDTH-1] = '#';
}

void drawPaddle() {
    for(int i=0;i<paddleSize;i++)
        map[paddleY][paddleX + i] = '=';
}

void drawBall() {
    map[ballY][ballX] = 'O';
}

void drawBricks() {
    int startX = 2, startY = 2;
    for(int i=0;i<BRICK_ROWS;i++) {
        for(int j=0;j<BRICK_COLS;j++) {
            if(!bricks[i][j]) continue;
            for(int k=0;k<BRICK_WIDTH;k++)
                map[startY + i][startX + j*(BRICK_WIDTH+1) + k] = '=';
        }
    }
}

void moveBall() {
    int nx = ballX + ballDX;
    int ny = ballY + ballDY;

    if(nx <= 1 || nx >= SCREEN_WIDTH-2) ballDX *= -1;
    if(ny <= 1) ballDY *= -1;

    if(ny == paddleY-1 && nx >= paddleX && nx <= paddleX+paddleSize) ballDY *= -1;

    int brickY = ny-2;
    int brickX = (nx-5)/(BRICK_WIDTH+1);
    if(brickY >= 0 && brickY < BRICK_ROWS && brickX >= 0 && brickX < BRICK_COLS && bricks[brickY][brickX]) {
        bricks[brickY][brickX] = 0;
        ballDY *= -1;
        score++;
        remainingBricks--;
    }

    if(ny >= SCREEN_HEIGHT-1) {
        system("cls");
        printf("Game Over! Score: %d\n", score);
        exit(0);
    }

    if(remainingBricks == 0) {
        system("cls");
        printf("You Win! Score: %d\n", score);
        exit(0);
    }

    ballX += ballDX;
    ballY += ballDY;
}

void handleInput() {
    if(!_kbhit()) return;
    char c = _getch();
    if(c=='a' && paddleX>1) paddleX -= 2;
    if(c=='d' && paddleX+paddleSize<SCREEN_WIDTH-1) paddleX += 2;
    if(c=='p') isPaused = !isPaused;
}

int main() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ci = {1,FALSE};
    SetConsoleCursorInfo(hConsole, &ci);

    setup();

    while(1) {
        clearMap();
        drawBorders();
        drawBricks();
        drawPaddle();
        drawBall();

        for(int i=0;i<SCREEN_HEIGHT;i++) {
            gotoxy(0,i);
            for(int j=0;j<SCREEN_WIDTH;j++)
                putchar(map[i][j]);
        }

        gotoxy(2,1);
        printf("Score: %d", score);

        handleInput();
        if(!isPaused) moveBall();

        Sleep(70);
    }
}

