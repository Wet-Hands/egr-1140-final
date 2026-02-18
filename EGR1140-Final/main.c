#include <stdio.h>
#include <conio.h>
#include <windows.h>

// Tile Types

#define TILE_EMPTY  0
#define TILE_PLAYER 1
#define TILE_EXIT   2
#define TILE_ENEMY  3
#define TILE_HEALTH 4
#define TILE_POWER  5
#define TILE_WALL   6
#define TILE_PRIZE  7

// Map Data

#define ROOM_SIZE_X 7
#define ROOM_SIZE_Y 12
int tile_array[ROOM_SIZE_Y][ROOM_SIZE_X];
int refresh_dungeon = 1;

// Tile Declaration

const char *tiles[] = { "[ ]", "[\u263A]", "[\u235F]", "[E]", "[\u2661]", "[P]", "[\u25A0]", "[$]" };

// Enemy Data

int is_enemy_spawned = 1;
int enemy_x = ROOM_SIZE_X-2;
int enemy_y = ROOM_SIZE_Y-2;
int enemy_health = 3;
int enemy_stamina = 2;

struct player_struct
{
    int position_x;
    int position_y;
    int health;
    int stamina;
};

struct enemy_struct
{
    int is_spawned;

    int position_x;
    int position_y;
    int health;
    int stamina;
};

// Function Declaration

void room_generator(struct player_struct *player);
void room_visual(void);
void move_player(char input, struct player_struct *player);
void move_enemy(struct player_struct *player);

void setColor(int colorValue) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, colorValue);
}
int main(void)
{
    system("chcp 65001 > nul");

    struct player_struct player;
    player.position_x = 1;
    player.position_y = 1;
    player.health = 10;
    player.stamina = 3;

    char input;
    room_generator(&player);

    while (1)
    {
        system("cls");

        room_visual();

        printf("\nYour Health: %d\n", player.health);
        printf("Your Stamina: %d\n", player.stamina);
        if(enemy_health > 0) printf("Enemy's Health: %d\n", enemy_health);
        printf("Move with WASD, q to quit\n");

        input = _getch();

        if (input == 'q')
            break;

        move_player(input, &player);

        if (player.stamina == 0)
        {
            for(int i = 0; i < enemy_stamina; i++)
            {
                if(enemy_health > 0)
                {
                    move_enemy(&player);
                }
            }
            player.stamina = 3;
            refresh_dungeon = 1;
        }

        if (player.health <= 0)
        {
            system("cls");
            printf("You were defeated!\n");
            break;
        }
    }

    return 0;
}

// Generates Tile Map

void room_generator(struct player_struct *player)
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

    tile_array[player->position_y][player->position_x] = TILE_PLAYER;
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
            if (tile_array[y][x] == TILE_PLAYER) setColor(14);
            if (tile_array[y][x] == TILE_ENEMY) setColor(12);


            printf("%s", tiles[tile_array[y][x]]);

            if (tile_array[y][x] != TILE_EMPTY)
            {
                setColor(7);
            }

            if(refresh_dungeon == 1)
            {
                Sleep(20);
            }
        }
        printf("\n");
    }
    refresh_dungeon = 0;
}

// Player Movement

void move_player(char input, struct player_struct *player)
{
    if (player->stamina <= 0)
        return;

    int new_x = player->position_x;
    int new_y = player->position_y;

    if (input == 'w') new_y--;
    if (input == 's') new_y++;
    if (input == 'a') new_x--;
    if (input == 'd') new_x++;

    if (new_x < 0 || new_x >= ROOM_SIZE_X || new_y < 0 || new_y >= ROOM_SIZE_Y) return;
    if (tile_array[new_y][new_x] == TILE_WALL) return;
    if (tile_array[new_y][new_x] == TILE_ENEMY)
    {
        enemy_health--;
        player->stamina--;

        if(enemy_health <= 0)
        {
            tile_array[enemy_y][enemy_x] = TILE_EMPTY;
            room_visual();
        }

        Sleep(50);

        return;
    }

    if (input == 'w' || input == 's' || input == 'a' || input == 'd')
    {
        player->stamina--;
    }

    tile_array[player->position_y][player->position_x] = TILE_EMPTY;

    player->position_x = new_x;
    player->position_y = new_y;

    tile_array[player->position_y][player->position_x] = TILE_PLAYER;
}

// Enemy Movement

void move_enemy(struct player_struct *player)
{
    int new_x = enemy_x;
    int new_y = enemy_y;

    if (player->position_x > enemy_x) new_x++;
    else if (player->position_x < enemy_x) new_x--;

    if (player->position_y > enemy_y) new_y++;
    else if (player->position_y < enemy_y) new_y--;

    if (new_x == player->position_x && new_y == player->position_y)
    {
        player->health--;
        return;
    }

    tile_array[enemy_y][enemy_x] = TILE_EMPTY;
    enemy_x = new_x;
    enemy_y = new_y;
    tile_array[enemy_y][enemy_x] = TILE_ENEMY;
}
