#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

// Tile Types
#define TILE_EMPTY  0
#define TILE_PLAYER 1
#define TILE_EXIT   2
#define TILE_ENEMY  3
#define TILE_HEALTH 4
#define TILE_POWER  5
#define TILE_WALL   6
#define TILE_PRIZE  7

// Map Size Constraints
#define ROOM_MIN    5
#define ROOM_MAX_X  15
#define ROOM_MAX_Y  15

// Difficulty Constraints
#define PLAYER_HEALTH_NORMAL  10
#define PLAYER_STAMINA_NORMAL 5
#define ENEMY_HEALTH_NORMAL   3
#define ENEMY_STAMINA_NORMAL  2

#define PLAYER_HEALTH_HARD    8
#define PLAYER_STAMINA_HARD   4
#define ENEMY_HEALTH_HARD     4
#define ENEMY_STAMINA_HARD    3

#define PLAYER_HEALTH_LUKA    1
#define PLAYER_STAMINA_LUKA   3
#define ENEMY_HEALTH_LUKA     3
#define ENEMY_STAMINA_LUKA    2

// Tile Declaration
const char *tiles[] = { "[ ]", "[\u263A]", "[\u235F]", "[E]", "[\u2661]", "[P]", "[\u25A0]", "[$]" };

struct room_struct
{
    int tile_array[ROOM_MAX_Y][ROOM_MAX_X];
    int size_x;
    int size_y;
    int refresh;
    int exit_x;
    int exit_y;
};

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

// Function Declarations
void title_screen(void);
void room_generator(struct room_struct *room, struct player_struct *player, struct enemy_struct *enemy);
void room_visual(struct room_struct *room);
void move_player(char input, struct player_struct *player, struct enemy_struct *enemy, struct room_struct *room);
void move_enemy(struct enemy_struct *enemy, struct player_struct *player, struct room_struct *room);

void setColor(int colorValue) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, colorValue);
}

void title_screen(void)
{
    system("cls");
    setColor(12);
    printf("\n");
    printf("  ██████╗ ██╗   ██╗███╗   ██╗ ██████╗ ███████╗ ██████╗ ███╗   ██╗\n");
    printf("  ██╔══██╗██║   ██║████╗  ██║██╔════╝ ██╔════╝██╔═══██╗████╗  ██║\n");
    printf("  ██║  ██║██║   ██║██╔██╗ ██║██║  ███╗█████╗  ██║   ██║██╔██╗ ██║\n");
    printf("  ██║  ██║██║   ██║██║╚██╗██║██║   ██║██╔══╝  ██║   ██║██║╚██╗██║\n");
    printf("  ██████╔╝╚██████╔╝██║ ╚████║╚██████╔╝███████╗╚██████╔╝██║ ╚████║\n");
    printf("  ╚═════╝  ╚═════╝ ╚═╝  ╚═══╝ ╚═════╝ ╚══════╝ ╚═════╝ ╚═╝  ╚═══╝\n");
    printf("\n");
    setColor(14);
    printf("                         ██████╗  ██████╗  ██████╗ ███╗   ███╗\n");
    printf("                         ██╔══██╗██╔═══██╗██╔═══██╗████╗ ████║\n");
    printf("                         ██║  ██║██║   ██║██║   ██║██╔████╔██║\n");
    printf("                         ██║  ██║██║   ██║██║   ██║██║╚██╔╝██║\n");
    printf("                         ██████╔╝╚██████╔╝╚██████╔╝██║ ╚═╝ ██║\n");
    printf("                         ╚═════╝  ╚═════╝  ╚═════╝ ╚═╝     ╚═╝\n");
    printf("\n");
    setColor(7);
    printf("                      Move with WASD | Q to Quit\n");
    printf("                    Defeat the enemy, find the exit!\n");
    printf("\n");
    setColor(8);
    printf("                       Press any key to begin...\n");
    setColor(7);
    _getch();
}

int main(void)
{
    system("chcp 65001 > nul");
    srand((unsigned int)time(NULL));

    while (1)
    {
        title_screen();

        struct room_struct room;
        room.size_x = 7;
        room.size_y = 10;
        room.refresh = 1;

        struct player_struct player;
        player.position_x = 1;
        player.position_y = 1;
        player.health = 10;
        player.stamina = 5;

        struct enemy_struct enemy;
        enemy.position_x = room.size_x-2;
        enemy.position_y = room.size_y-2;
        enemy.health = 3;
        enemy.stamina = 2;
        enemy.is_spawned = 1;

        char input;
        room_generator(&room, &player, &enemy);

        int running = 1;
        int floor = 1;

        while (running)
        {
            system("cls");

            room_visual(&room);

            printf("\nFloor: %d\n", floor);
            printf("Your Health: %d\n", player.health);
            printf("Your Stamina: %d\n", player.stamina);
            if (enemy.is_spawned && enemy.health > 0)
                printf("Enemy's Health: %d\n", enemy.health);
            else if (!enemy.is_spawned)
                printf("Find the exit! [%s]\n", tiles[TILE_EXIT]);
            printf("Move with WASD, q to quit\n");

            input = _getch();

            if (input == 'q')
                return 0;

            move_player(input, &player, &enemy, &room);

            // Check if player stepped onto the exit
            if (player.position_y == room.exit_y && player.position_x == room.exit_x)
            {
                floor++;

                int new_size_x = ROOM_MIN + rand() % (ROOM_MAX_X - ROOM_MIN + 1);
                int new_size_y = ROOM_MIN + rand() % (ROOM_MAX_Y - ROOM_MIN + 1);

                room.size_x = new_size_x;
                room.size_y = new_size_y;
                room.refresh = 1;

                player.position_x = 1;
                player.position_y = 1;
                player.stamina = 5; // Resetting Stamina

                // Scale enemy stats to floor
                enemy.position_x = new_size_x - 2;
                enemy.position_y = new_size_y - 2;
                enemy.health = 2 + (floor / 2);
                enemy.stamina = 2;
                enemy.is_spawned = 1;

                room_generator(&room, &player, &enemy);
                continue;
            }

            if (player.stamina == 0)
            {
                for (int i = 0; i < enemy.stamina; i++)
                {
                    if (enemy.is_spawned && enemy.health > 0)
                    {
                        move_enemy(&enemy, &player, &room);
                    }
                }
                player.stamina = 5;
                room.refresh = 1;
            }

            if (player.health <= 0)
            {
                system("cls");
                setColor(12);
                printf("\n\n");
                printf("   ██╗   ██╗ ██████╗ ██╗   ██╗    ██████╗ ██╗███████╗██████╗ ██╗\n");
                printf("   ╚██╗ ██╔╝██╔═══██╗██║   ██║    ██╔══██╗██║██╔════╝██╔══██╗██║\n");
                printf("    ╚████╔╝ ██║   ██║██║   ██║    ██║  ██║██║█████╗  ██║  ██║██║\n");
                printf("     ╚██╔╝  ██║   ██║██║   ██║    ██║  ██║██║██╔══╝  ██║  ██║╚═╝\n");
                printf("      ██║   ╚██████╔╝╚██████╔╝    ██████╔╝██║███████╗██████╔╝██╗\n");
                printf("      ╚═╝    ╚═════╝  ╚═════╝     ╚═════╝ ╚═╝╚══════╝╚═════╝ ╚═╝\n");
                setColor(8);
                printf("\n                      You made it to floor %d.\n", floor);
                setColor(7);
                printf("\n                   Press any key to return to title...\n");
                _getch();
                running = 0;
            }
        }
    }

    return 0;
}

// Generates Tile Map
void room_generator(struct room_struct *room, struct player_struct *player, struct enemy_struct *enemy)
{
    // Clear the full max-size array first
    for (int y = 0; y < ROOM_MAX_Y; y++)
        for (int x = 0; x < ROOM_MAX_X; x++)
            room->tile_array[y][x] = TILE_EMPTY;

    // Build walls for the actual room size
    for (int y = 0; y < room->size_y; y++)
    {
        for (int x = 0; x < room->size_x; x++)
        {
            room->tile_array[y][x] = TILE_EMPTY;
            if (y == 0 || x == 0 || y == room->size_y - 1 || x == room->size_x - 1)
                room->tile_array[y][x] = TILE_WALL;
        }
    }

    room->tile_array[player->position_y][player->position_x] = TILE_PLAYER;

    if (enemy->is_spawned == 1)
    {
        room->tile_array[enemy->position_y][enemy->position_x] = TILE_ENEMY;
    }
    else
    {
        enemy->position_x = -1;
        enemy->position_y = -1;
    }
}

// Draws Room based on Tile Map
void room_visual(struct room_struct *room)
{
    printf("\n");
    for (int y = 0; y < room->size_y; y++)
    {
        for (int x = 0; x < room->size_x; x++)
        {
            if (room->tile_array[y][x] == TILE_PLAYER) setColor(14);
            if (room->tile_array[y][x] == TILE_ENEMY)  setColor(12);
            if (room->tile_array[y][x] == TILE_EXIT)   setColor(5);

            printf("%s", tiles[room->tile_array[y][x]]);

            if (room->tile_array[y][x] != TILE_EMPTY)
                setColor(7);

            if (room->refresh == 1)
                Sleep(5);
        }
        printf("\n");
    }
    room->refresh = 0;
}

// Player Movement
void move_player(char input, struct player_struct *player, struct enemy_struct *enemy, struct room_struct *room)
{
    if (player->stamina <= 0)
        return;

    int new_x = player->position_x;
    int new_y = player->position_y;

    if (input == 'w') new_y--;
    if (input == 's') new_y++;
    if (input == 'a') new_x--;
    if (input == 'd') new_x++;

    if (new_x < 0 || new_x >= room->size_x || new_y < 0 || new_y >= room->size_y) return;
    if (room->tile_array[new_y][new_x] == TILE_WALL) return;

    if (room->tile_array[new_y][new_x] == TILE_ENEMY)
    {
        enemy->health--;
        player->stamina--;

        if (enemy->health <= 0)
        {
            room->tile_array[enemy->position_y][enemy->position_x] = TILE_EMPTY;
            enemy->is_spawned = 0;

            // Spawn the exit in the far corner
            int exit_x = room->size_x - 2;
            int exit_y = room->size_y - 2;


            // Safety: don't spawn exit on the player
            if (exit_x == player->position_x && exit_y == player->position_y)
                exit_x--;

            room->exit_x = exit_x;
            room->exit_y = exit_y;
            room->tile_array[exit_y][exit_x] = TILE_EXIT;
        }
        return;
    }

    if (input == 'w' || input == 's' || input == 'a' || input == 'd')
        player->stamina--;

    room->tile_array[player->position_y][player->position_x] = TILE_EMPTY;
    player->position_x = new_x;
    player->position_y = new_y;
    room->tile_array[player->position_y][player->position_x] = TILE_PLAYER;
}

// Enemy Movement
void move_enemy(struct enemy_struct *enemy, struct player_struct *player, struct room_struct *room)
{
    int new_x = enemy->position_x;
    int new_y = enemy->position_y;

    if (player->position_x > enemy->position_x) new_x++;
    else if (player->position_x < enemy->position_x) new_x--;

    if (player->position_y > enemy->position_y) new_y++;
    else if (player->position_y < enemy->position_y) new_y--;

    if (new_x == player->position_x && new_y == player->position_y)
    {
        player->health--;
        return;
    }

    // Don't walk onto the exit tile
    if (room->tile_array[new_y][new_x] == TILE_EXIT) return;

    room->tile_array[enemy->position_y][enemy->position_x] = TILE_EMPTY;
    enemy->position_x = new_x;
    enemy->position_y = new_y;
    room->tile_array[enemy->position_y][enemy->position_x] = TILE_ENEMY;
}
