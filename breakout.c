#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define screenWidth 80
#define screenHeight 24

int remainingBricks = (3 * (screenWidth - 2)) - 72;
int score = 0, paddleSize = 10, isPaused = 0; // 0 means not paused, 1 means paused

void gotoxy(int x, int y)
{
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

int map[screenHeight][screenWidth];
int bricks[5][screenWidth];

typedef struct
{
    int x;
    int y;
    int speed;
} Paddle;

typedef struct
{
    int x;
    int y;
    int speedX;
    int speedY;
} Ball;

Paddle paddle;
Ball ball;

void setup(char difficulty)
{
    srand(time(NULL));

    paddle.x = screenWidth / 2 - 5;
    paddle.y = screenHeight - 1;
    paddle.speed = 3;

    ball.x = screenWidth / 2;
    ball.y = screenHeight - 2;
    ball.speedX = -3;
    ball.speedY = -3;

    if (difficulty == 48) //48 is ASCII value of '0'
    {
        // Easy level
        ball.speedX = -2;
        ball.speedY = -2;
    }
    else if (difficulty == 49)//49 is ASCII value of '1'
    { // Hard level
        ball.speedX = -3;
        ball.speedY = -3;
    }
    // Set a random initial direction for the ball
    int randomDirection = rand() % 3;

    if (randomDirection < 2)
    {
        ball.speedX = -1;
        ball.speedY = -1;
    }
    else
    {
        ball.speedX = 1;
        ball.speedY = -1;
    }

    // Initialize bricks
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < screenWidth; j++)
        {
            bricks[i][j] = 219;
        }
    }
}
void displayPauseMessage()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = {(screenWidth - 34) / 2, 1};
    DWORD written;

    // Construct the message based on the isPaused flag
    const char *message;
    if (isPaused)
    {
        message = "Game Paused! Press 'P' to resume";
    }
    else
    {
        message = "Press 'P' to pause the Game";
    }

    // Write the message to the console buffer
    WriteConsoleOutputCharacter(hConsole, message, strlen(message), position, &written);
}
void drawPaddle()
{
    for (int i = 0; i < paddleSize; i++)
    {
        map[paddle.y][paddle.x + i] = 219; // Draw paddle with ASCII character 219
    }
}

void movePaddle(char direction)
{
    if (!isPaused)
    {
        if (direction == 'a' && paddle.x - paddle.speed > 0)
        {
            paddle.x -= paddle.speed;
        }
        else if (direction == 'd' && paddle.x + paddle.speed < screenWidth - paddleSize + 1)
        {
            paddle.x += paddle.speed;
        }
    }
}

void drawScore()
{
    gotoxy(2, 1);
    printf("Score: %d ", score);
}

void drawBall()
{
    map[ball.y][ball.x] = 'o'; // Draw ball with 'o' character
}

void moveBall()
{
    // Calculate the new position of the ball
    int newBallX = ball.x + ball.speedX ;
    int newBallY = ball.y + ball.speedY ;

    // Check for collisions with walls
    if (newBallX >= screenWidth - 1 || newBallX <= 0)
    {
        ball.speedX = -ball.speedX; // Reverse horizontal direction upon hitting walls
    }

    if (newBallY <= 0)
    {
        ball.speedY = -ball.speedY; // Reverse vertical direction upon hitting top wall
    }

    if (newBallY >= screenHeight - 2 && ball.x >= paddle.x && ball.x <= paddle.x + paddleSize)
    {
        ball.speedY = -ball.speedY; // Reverse vertical direction upon hitting paddle
    }

    if (newBallY >= screenHeight - 1 && (ball.x != paddle.x)) // if the ball does not hit the paddle
    {

        // Game over
        gotoxy((screenWidth - 9) / 2, screenHeight / 2);
        printf("Game Over!\n ");

        gotoxy((screenWidth - 18) / 2, (screenHeight / 2) + 1);
        printf("Your Score is : %d \n", score);
        gotoxy((screenWidth - 34) / 2, (screenHeight / 2) + 2);
        Sleep(1000);
        printf("Press any key to exit the game.\n");

        exit(0);
    }

    // if the ball hits the brick
    if (map[newBallY][newBallX] == 219) // Brick
    {

        map[newBallY][newBallX] = ' ';
        bricks[(newBallY - 3)][newBallX] = ' ';
        ball.speedY = -ball.speedY;
        score++;
        drawScore();
        remainingBricks--;
    }
    else if (map[newBallY][newBallX - 1] == 219) // Space
    {
        map[newBallY][newBallX - 1] = ' ';
        bricks[(newBallY - 3)][newBallX - 1] = ' ';
        ball.speedY = -ball.speedY;
        score++;
        drawScore();
        remainingBricks--;
    }
    else if (map[newBallY][newBallX + 1] == 219) // Space
    {
        map[newBallY][newBallX + 1] = ' ';
        bricks[(newBallY - 3)][newBallX + 1] = ' ';
        ball.speedY = -ball.speedY;
        score++;
        drawScore();
        remainingBricks--;
    }

    if (remainingBricks == 0)
    {
        // Display "Game Won" message
        gotoxy((screenWidth - 9) / 2, screenHeight / 2);
        printf("Game Won!");

        gotoxy((screenWidth - 18) / 2, (screenHeight / 2) + 1);
        printf("Your Score is : %d \n", score);

        gotoxy((screenWidth - 34) / 2, (screenHeight / 2) + 2);
        Sleep(1000);
        printf("Press any key to exit the game.\n");

        exit(0);
    }

    // Update the position of the ball
    ball.x = newBallX;
    ball.y = newBallY;
}

void drawBricks()
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < screenWidth - 2; j++)
        {
            if (bricks[i][j] == 219)
            {
                // Add spaces between bricks
                if (j % 10 == 0 || j % 10 == 1 || j % 10 == 2)
                {
                    continue;
                }

                if (i == 1 || i == 3)
                {
                    map[i + 3][j] = ' '; // add horizpntal space with space character
                }
                else
                {
                    map[i + 3][j] = 219; // Draw bricks with ASCII character 219
                }
            }
        }
    }
}

void drawMap()
{
    // Display pause/resume message
    gotoxy((screenWidth - 18) / 2, 2);
    if (isPaused)
    {
        printf("Game Paused. Press 'P' to resume.");
    }
    else
    {
        printf("Press 'P' to pause");
    }

    for (int i = 0; i < screenHeight; i++)
    {
        for (int j = 0; j < screenWidth; j++)
        {
            gotoxy(j + 1, i + 1);
            printf("%c", map[i][j]);
        }
    }
}

void handleInput()
{
    if (_kbhit())
    {
        char key = _getch();
        if (key == 'a' || key == 'A')
        {
            movePaddle('a');
        }
        else if (key == 'd' || key == 'D')
        {
            movePaddle('d');
        }
        else if (key == 'p' || key == 'P')
        {

            isPaused = !isPaused;
        }
    }
}

void updateMap()
{
    for (int i = 0; i < screenHeight; i++)
    {
        for (int j = 0; j < screenWidth; j++)
        {
            if (i == 0 || i == screenHeight - 1 || j == 0 || j == screenWidth - 1)
            {
                map[i][j] = '#'; //  boundary walls
            }
            else
            {
                map[i][j] = ' '; // Clear the map
            }
        }
    }

    drawPaddle();
    drawBall();
    drawBricks();
    drawScore();
}

void displayInstructions()
{
    printf("\n\n\n\n\t\t\t\t\t");
    printf("Welcome to the Brick Breaker Game!\n");
    printf("\n\t\t\t\t\t\t");
    printf("Instructions:\n");
    printf("\n\t\t\t\t");
    printf("- Use 'A' and 'D' to move the paddle left and right.\n");
    printf("\n\t\t\t\t");
    printf("- Press 'P' to pause and resume the game.\n");
    printf("\n\t\t\t\t");
    printf("- Try to bounce the ball off the paddle to break the bricks and score points.\n");
    printf("\n\t\t\t\t");
    printf("- Don't let the ball reach the bottom of the screen!\n\n");
}

int main()
{
    char difficulty;
    displayInstructions();
    printf("\n\t\t\t\t\t\t");
    printf("Select difficulty level:\n");
    printf("\n\t\t\t\t\t\t");
    printf("0 - Easy\n");
    printf("\t\t\t\t\t\t");
    printf("1 - Hard\n");
    printf("\t\t\t\t\t\t");

    scanf(" %c", &difficulty);

    if (difficulty != 48 && difficulty != 49)
    {
        printf("Invalid difficulty level. Exiting...\n");
        return 1;
    }
    setup(difficulty);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(hConsole, &csbiInfo);

    SMALL_RECT windowSize = {0, 0, screenWidth, screenHeight};
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

    CHAR_INFO buffer[screenWidth * screenHeight];
    COORD bufferSize = {screenWidth, screenHeight};
    COORD characterPos = {0, 0};
    SMALL_RECT writeArea = {0, 0, screenWidth - 1, screenHeight - 1};

    // Set up a timer to control the framerate
    LARGE_INTEGER frequency;
    LARGE_INTEGER prevTime, currTime;
    double elapsed;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&prevTime);

    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;       // Set cursor size to 1
    cursorInfo.bVisible = FALSE; // Set cursor visibility to false
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    while (1)
    {
        displayPauseMessage();

        // Calculate elapsed time since the previous frame
        QueryPerformanceCounter(&currTime);
        elapsed = (double)(currTime.QuadPart - prevTime.QuadPart) / frequency.QuadPart;

        if (elapsed >= 0.0666) // Update at approximately 60 frames per second
        {
            updateMap();

            // Copy the map to the buffer
            for (int i = 0; i < screenHeight; i++)
            {
                for (int j = 0; j < screenWidth; j++)
                {
                    buffer[(i * screenWidth) + j].Char.AsciiChar = map[i][j];
                    buffer[(i * screenWidth) + j].Attributes = csbiInfo.wAttributes;
                }
            }

            // Write the buffer to the screen
            WriteConsoleOutputA(hConsole, buffer, bufferSize, characterPos, &writeArea);

            drawScore();

            // Handle input and move ball
            handleInput();
            if (!isPaused)
            {
                moveBall(difficulty);
                // Check for score and increase paddle size
                if (score == 15)
                {
                    paddleSize = 11;
                }
                else if (score == 30)
                {
                    paddleSize = 12;
                }
                else if (score == 45)
                {
                    paddleSize = 13;
                }
                else if (score == 60)
                {
                    paddleSize = 14;
                }
                else if (score == 80)
                {
                    paddleSize = 15;
                }
            }


        }
        Sleep(150);
    }


    return 0;
}

