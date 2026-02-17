#include <stdio.h>
#include <conio.h>

// Tile Types

#define TILE_EMPTY  0
#define TILE_PLAYER 1
#define TILE_EXIT   2
#define TILE_ENEMY  3
#define TILE_HEALTH 4
#define TILE_POWER  5
#define TILE_WALL   6

// Map Data

#define ROOM_SIZE_X 7
#define ROOM_SIZE_Y 12
int tile_array[ROOM_SIZE_Y][ROOM_SIZE_X];

// Tile Declaration

const char *tiles[] = { "[ ]", "[\u263A]", "[X]", "[E]", "[H]", "[P]", "[\u25A0]" };

// Player Data

int player_x = 1;
int player_y = 1;
int player_health = 10;
int player_stamina = 100;

// Enemy Data

int is_enemy_spawned = 1;
int enemy_x = ROOM_SIZE_X-2;
int enemy_y = ROOM_SIZE_Y-2;
int enemy_health = 3;
int enemy_stamina = 2;

// Function Declaration

void room_generator(void);
void room_visual(void);
void move_player(char input);
void move_enemy(void);

struct enemy
{
    int position_x;
    int position_y;
    int health;
    int stamina;
};

int main(void)
{
    system("chcp 65001 > nul");
    char input;

    room_generator();

    while (1)
    {
        system("cls");

        room_visual();

        printf("\nHealth: %d\n", player_health);
        printf("Stamina: %d\n", player_stamina);
        printf("Move with WASD, q to quit\n");

        input = _getch();

        if (input == 'q')
            break;

        move_player(input);
        //if (input == 'w' || input == 's' || input == 'a' || input == 'd')
        //{
            //move_enemy();   /* Enemy acts after player */
        //}


        if (player_health <= 0)
        {
            system("cls");
            printf("You were defeated!\n");
            break;
        }
    }

    return 0;
}

// Generates Tile Map

void room_generator(void)
{
    for (int y = 0; y < ROOM_SIZE_Y; y++)
    {
        for (int x = 0; x < ROOM_SIZE_X; x++)
        {
            tile_array[y][x] = TILE_EMPTY;
            if(y == 0 || x == 0 || y == ROOM_SIZE_Y-1 || x == ROOM_SIZE_X-1)
            {
                tile_array[y][x] = TILE_WALL;
            }
        }
    }

    tile_array[player_y][player_x] = TILE_PLAYER;
    if(is_enemy_spawned == 1)
    {
        tile_array[enemy_y][enemy_x]   = TILE_ENEMY;
    }
    else
    {
        enemy_x = -1;
        enemy_y = -1;
    }
}

// Draws Room based on Tile Map

void room_visual(void)
{
    printf("\n");
    for (int y = 0; y < ROOM_SIZE_Y; y++)
    {
        for (int x = 0; x < ROOM_SIZE_X; x++)
        {
            printf("%s", tiles[tile_array[y][x]]);
        }
        printf("\n");
    }
}

/* ===============================
   Player movement
   =============================== */
void move_player(char input)
{
    if (player_stamina <= 0)
        return;

    int new_x = player_x;
    int new_y = player_y;

    if (input == 'w') new_y--;
    if (input == 's') new_y++;
    if (input == 'a') new_x--;
    if (input == 'd') new_x++;

    if (new_x < 0 || new_x >= ROOM_SIZE_X || new_y < 0 || new_y >= ROOM_SIZE_Y) return;
    if (tile_array[new_y][new_x] == TILE_WALL) return;
    if (tile_array[new_y][new_x] == TILE_ENEMY) return;

    if (input == 'w' || input == 's' || input == 'a' || input == 'd')
    {
        player_stamina--;
    }

    tile_array[player_y][player_x] = TILE_EMPTY;

    player_x = new_x;
    player_y = new_y;

    tile_array[player_y][player_x] = TILE_PLAYER;
}

/* ===============================
   Enemy movement (chases player)
   =============================== */
void move_enemy(void)
{
    int new_x = enemy_x;
    int new_y = enemy_y;

    /* Simple chase AI */
    if (player_x > enemy_x) new_x++;
    else if (player_x < enemy_x) new_x--;

    if (player_y > enemy_y) new_y++;
    else if (player_y < enemy_y) new_y--;

    /* Check bounds */
    if (new_x < 0 || new_x >= ROOM_SIZE_X ||
        new_y < 0 || new_y >= ROOM_SIZE_Y)
        return;

    /* If enemy reaches player */
    if (new_x == player_x && new_y == player_y)
    {
        player_health--;
        return;
    }

    /* Move enemy */
    tile_array[enemy_y][enemy_x] = TILE_EMPTY;
    enemy_x = new_x;
    enemy_y = new_y;
    tile_array[enemy_y][enemy_x] = TILE_ENEMY;
}
