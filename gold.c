#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

int HEIGHT;
int WIDTH;
int OBSTACLES;

#define MAX_OBSTACLES 20

int obstacle_x[MAX_OBSTACLES];
int obstacle_y[MAX_OBSTACLES];

enum direction
{
    STOP,
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

enum direction dir;

int score = 0;
int live = 4;
int back_to_menu = 0;

int player_x, player_y;
int sleep_time = 100;

int gold_x, gold_y;

int addpoint_x, addpoint_y;

// BONUS SYSTEM
int bonus_visible = 1;

time_t bonus_spawn_time;
time_t bonus_delay_time;

int bonus_duration = 10;
int bonus_delay = 5;

// Function Prototypes
void setup();
void draw();
void input();
void game_play();
void clear_screen();
void hideCursor();
void setColor(int color);

// COLOR FUNCTION
void setColor(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

int main()
{
    int choice;
    int difficulty;

    system("cls");

    printf("\n\n");
    printf("\tGOLD COLLECTION GAME\n\n");

    printf("\t Find Gold (G)\n");
    printf("\t Avoid Enemy (X)\n\n");

    printf("\tPress any key to continue \n");

    getch();

    while (1)
    {
        system("cls");

        printf("\n\n");
        printf("\tMAIN MENU\n\n");

        printf("\t1. Start Game\n");
        printf("\t2. Instructions\n");
        printf("\t3. Exit\n");

        printf("\n\tEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:

            system("cls");

            printf("\n\n");
            printf("\n\tDifficulty menu:\n");
            printf("\n\t1. Easy");
            printf("\n\t2. medium");
            printf("\n\t3. Hard");

            printf("\n\n\tEnter your choice: ");
            scanf("%d", &difficulty);

            if (difficulty == 1)
            {
                HEIGHT = 20;
                WIDTH = 60;
                OBSTACLES = 8;
            }
            else if (difficulty == 2)
            {
                HEIGHT = 18;
                WIDTH = 55;
                OBSTACLES = 9;
            }
            else if (difficulty == 3)
            {
                HEIGHT = 16;
                WIDTH = 50;
                OBSTACLES = 10;
            }
            else
            {
                printf("\n invalid choice try again..!");
                getch();
                break;
            }

            system("cls");

            score = 0;
            live = 4;

            srand(time(NULL));
            hideCursor();

            setup();

            back_to_menu = 0;
            dir = STOP;

            while (!back_to_menu)
            {
                input();
                game_play();
                draw();

                if (difficulty == 1)
                    sleep_time = 200 - score * 7;
                else if (difficulty == 2)
                    sleep_time = 200 - score * 8;
                else if (difficulty == 3)
                    sleep_time = 200 - score * 11;

                if (sleep_time < 3)
                    sleep_time = 3;

                Sleep(sleep_time);
            }

            break;

        case 2:

            system("cls");

            printf("\n\tHow to play\n\n");
            printf("\tP = Player\n");
            printf("\tG = Gold (+10 points)\n");
            printf("\t* = Bonus (+20 points)\n");
            printf("\tX = Obstacle (-1 life)\n\n");

            printf("\tCONTROLS:\n");
            printf("\tW = UP\n");
            printf("\tS = DOWN\n");
            printf("\tA = LEFT\n");
            printf("\tD = RIGHT\n\n");

            printf("\tPress any key to go back...");
            getch();
            break;

        case 3:
            system("cls");
            FreeConsole();
            ExitProcess(0);
            getch();

        default:
            printf("\n\t\tInvalid Choice try again..!");
            getch();
        }
    }
}

void setup()
{
    player_x = 3;
    player_y = 3;

    for (int i = 0; i < OBSTACLES; i++)
    {
        obstacle_x[i] = 20 + rand() % 20;
        obstacle_y[i] = rand() % HEIGHT;
    }

    gold_x = rand() % WIDTH;
    gold_y = rand() % HEIGHT;

    addpoint_x = rand() % WIDTH;
    addpoint_y = rand() % HEIGHT;

    bonus_spawn_time = time(NULL);
    dir = STOP;
}

void input()
{
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'w':
        case 'W':

            dir = UP;
            break;

        case 's': 
        case 'S':
            dir = DOWN;
            break;

        case 'a': 
        case 'A':
            dir = LEFT;
            break;

        case 'd': 
        case 'D':
            dir = RIGHT;
            break;

        case 'x': 
        case 'X':
            exit(0);
            break;
        }
    }
}

void game_play()
{
    switch (dir)
    {
    case UP: 
        player_y--;
        break;

    case DOWN:
        player_y++; 
        break;

    case LEFT: 
        player_x--; 
        break;

    case RIGHT:
        player_x++; 
        break;

    case STOP:
        break;
    }

    //wall crossing

    if (player_x >= WIDTH)
        player_x = 0;

    if (player_x < 0) 
        player_x = WIDTH - 1;

    if (player_y >= HEIGHT)
        player_y = 0;

    if (player_y < 0) 
        player_y = HEIGHT - 1;

    //gold collection

    if (player_x == gold_x && player_y == gold_y)
    {
        score += 10;
        gold_x = rand() % WIDTH;
        gold_y = rand() % HEIGHT;
    }

    //bonus system
    if (bonus_visible)
    {
        if (player_x == addpoint_x &&
            player_y == addpoint_y)
        {
            score += 20;

            bonus_visible = 0;

            addpoint_x = -1;
            addpoint_y = -1;

            bonus_delay_time = time(NULL);
        }

        else if (time(NULL) - bonus_spawn_time >= bonus_duration)
        {
            bonus_visible = 0;

            addpoint_x = -1;
            addpoint_y = -1;

            bonus_delay_time = time(NULL);
        }
    }

    else
    {
        if (time(NULL) - bonus_delay_time >= bonus_delay)
        {
            addpoint_x = rand() % WIDTH;
            addpoint_y = rand() % HEIGHT;

            bonus_visible = 1;

            bonus_spawn_time = time(NULL);
        }
    }


    for (int i = 0; i < OBSTACLES; i++)
    {
        obstacle_y[i]--;

        //collision after obstacle moves

        if (player_x == obstacle_x[i] &&
            player_y == obstacle_y[i])
        {
            live--;

            player_x = 3;
            player_y = 3;

            if (live <= 0)
            {
                system("cls");
                
                printf("\n\nGAME OVER\n");
                printf("Final Score: %d\n", score);

                printf("\n1.Restart");
                printf("\n2.Go back to menu");

                int choice;
                while (1)
                {
                    printf("\n\nEnter choice: ");
                    scanf("%d", &choice);

                    if (choice == 1)
                    {
                        system("cls");
                        score = 0;
                        live = 4;
                        dir = STOP;
                        setup();
                        return;
                    }
                       else if (choice == 2)
                    {
                        back_to_menu = 1;
                        return;
                    }
                    else
                    {
                        printf("invalid choice try again...");
                        getch();
                        continue;
                   }
               }
           }
        }
        if (obstacle_y[i] < 0)
        {
            obstacle_y[i] = HEIGHT - 1;
            obstacle_x[i] = rand() % WIDTH;
        }
    }
}

void draw()
{
    clear_screen();

    printf("\n\tWELCOME TO THE GOLD COLLECTION GAME\n");
    printf("Lives: %d\n", live);
    
    //top wall

    for (int i = 0; i < WIDTH + 2; i++)
        printf("#");
    printf("\n");
    
    //game area
    for (int i = 0; i < HEIGHT; i++)
    {
        printf("#");

        for (int j = 0; j < WIDTH; j++)
        {
            int printed = 0;

            if (i == player_y && j == player_x)
            {
                setColor(10);
                printf("P");
                setColor(7);
                printed = 1;
            }
            else if (i == gold_y && j == gold_x)
            {
                setColor(14);
                printf("G");
                setColor(7);
                printed = 1;
            }
             else if (i == addpoint_y && j == addpoint_x)
            {
                setColor(11);
                printf("*");
                setColor(7);
                printed = 1;
            }

            else
            {
                for (int k = 0; k < OBSTACLES; k++)
                {
                    if (i == obstacle_y[k] && j == obstacle_x[k])
                    {
                        setColor(12);
                        printf("X");
                        setColor(7);
                        printed = 1;
                        break;
                    }
                }
            }

            if (!printed)
                printf(" ");
        }

        printf("#\n");
    }

    //botom wall 

    for (int i = 0; i < WIDTH + 2; i++)
        printf("#");

    printf("\nScore: %d\n", score);
}

void clear_screen()
{
    COORD cursorPosition;
    cursorPosition.X = 0;
    cursorPosition.Y = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}

void hideCursor()
{
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}