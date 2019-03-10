//
//  snake.c
//  IC-Snake
//
//  Created by @devmaximilian on 30/12 2018
//  Copyright © 2018 @devmaximilian. All rights reserved.
//

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

// Keep track of game status
int gameover;

// Map dimensions
const int width = 40;
const int height = 20;

// Position
int x, y;

// Fruit position
int fx, fy;

// Player score
int score = 0;

// Tail
int *tx = 0;
int *ty = 0;

// Tail length
int size = 0;

// Direction
enum direction
{
    NONE,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Snake Direction
enum direction moving;

// Drop fruit at random
void dropFruit()
{
    fx = rand() % width;
    fy = rand() % height;
}

/**
 * Get raw input working
 * Source: https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html
 */
struct termios orig_termios;

// Disable raw input
void disableRawMode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

// Enable raw input
void enableRawMode()
{
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

// Everything needed before game starts
void setup()
{
    // Run game
    gameover = 0;

    // Set initial direction
    moving = NONE;

    // Set initial position
    x = width / 2;
    y = height / 2;

    // Randomly place a fruit
    dropFruit();

    // Set score
    score = 0;

    // Enable getting raw input
    enableRawMode();
}

// Visualize game
void draw()
{
    system("clear");

    // Display score
    printf("Score: %d\r\n", score);

    for (int i = 0; i < width + 2; i++)
    {
        printf("#");
    }
    printf("\r\n");

    for (int a = 0; a < height; a++)
    {
        for (int b = 0; b < width; b++)
        {
            if (b == 0)
            {
                printf("#");
            }

            if (a == y && b == x)
            {
                printf("s");
            }
            else if (a == fy && b == fx)
            {
                printf("f");
            }
            else
            {
                printf(" ");
            }

            if (b == width - 1)
            {
                printf("#");
            }
        }
        printf("\r\n");
    }

    for (int i = 0; i < width + 2; i++)
    {
        printf("#");
    }
    printf("\r\n");
}

// Interpret input
void input()
{
    char c = '\0';
    if (read(STDIN_FILENO, &c, 1) == -1)
    {
    }
    if (iscntrl(c))
    {
        // No key pressed
    }
    else
    {
        switch (c)
        {
        case 'a':
            moving = LEFT;
            break;
        case 'd':
            moving = RIGHT;
            break;
        case 'w':
            moving = UP;
            break;
        case 's':
            moving = DOWN;
            break;
        case 'q':
            gameover = 1;
            break;
        }
    }
}

// Decrease tail array
void increaseTail()
{
    size++;
    tx = realloc(tx, size * sizeof(int));
    ty = realloc(ty, size * sizeof(int));
}

// Handle game logic
void logic()
{
    // Update position
    switch (moving)
    {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }

    // Border collision
    if (x > width || x < 0 || y > height || y < 0)
    {
        gameover = 1;
    }

    // Eat fruit
    if (x == fx && y == fy)
    {
        score += 1;
        increaseTail();
        tx[size - 1] = fx;
        ty[size - 1] = fy;
        dropFruit();
    }
}

// Increase tail array
void decreaseTail()
{
    if (size > 0)
    {
        size--;
        tx = realloc(tx, size * sizeof(int));
        ty = realloc(ty, size * sizeof(int));
    }
}

// Main program loop
int main()
{
    setup();

    // Keep game running until gameover
    while (gameover < 1)
    {
        input();
        logic();
        draw();
        // sleep to slow game
        //sleep(10);
    }

    return 0;
}
