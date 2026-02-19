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

// Tile Declaration
const char *tiles[] = { "[ ]", "[\u263A]", "[\u235F]", "[E]", "[\u2661]", "[P]", "[\u25A0]", "[$]" };

struct room_struct
{
    int tile_array[ROOM_SIZE_Y][ROOM_SIZE_X];
    int refresh;
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
    setColor(12); // Red
    printf("\n");
    printf("  ██████╗ ██╗   ██╗███╗   ██╗ ██████╗ ███████╗ ██████╗ ███╗   ██╗\n");
    printf("  ██╔══██╗██║   ██║████╗  ██║██╔════╝ ██╔════╝██╔═══██╗████╗  ██║\n");
    printf("  ██║  ██║██║   ██║██╔██╗ ██║██║  ███╗█████╗  ██║   ██║██╔██╗ ██║\n");
    printf("  ██║  ██║██║   ██║██║╚██╗██║██║   ██║██╔══╝  ██║   ██║██║╚██╗██║\n");
    printf("  ██████╔╝╚██████╔╝██║ ╚████║╚██████╔╝███████╗╚██████╔╝██║ ╚████║\n");
    printf("  ╚═════╝  ╚═════╝ ╚═╝  ╚═══╝ ╚═════╝ ╚══════╝ ╚═════╝ ╚═╝  ╚═══╝\n");
    printf("\n");
    setColor(14); // Yellow
    printf("                         ██████╗  ██████╗  ██████╗ ███╗   ███╗\n");
    printf("                         ██╔══██╗██╔═══██╗██╔═══██╗████╗ ████║\n");
    printf("                         ██║  ██║██║   ██║██║   ██║██╔████╔██║\n");
    printf("                         ██║  ██║██║   ██║██║   ██║██║╚██╔╝██║\n");
    printf("                         ██████╔╝╚██████╔╝╚██████╔╝██║ ╚═╝ ██║\n");
    printf("                         ╚═════╝  ╚═════╝  ╚═════╝ ╚═╝     ╚═╝\n");
    printf("\n");
    setColor(7); // White
    printf("                      Move with WASD | Q to Quit\n");
    printf("                       Defeat the enemy to win!\n");
    printf("\n");
    setColor(8); // Dark grey
    printf("                       Press any key to begin...\n");
    setColor(7);
    _getch();
}

int main(void)
{
    system("chcp 65001 > nul");

    while (1)
    {
        title_screen();

        struct player_struct player;
        player.position_x = 1;
        player.position_y = 1;
        player.health = 10;
        player.stamina = 3;

        struct enemy_struct enemy;
        enemy.position_x = ROOM_SIZE_X - 2;
        enemy.position_y = ROOM_SIZE_Y - 2;
        enemy.health = 3;
        enemy.stamina = 2;
        enemy.is_spawned = 1;

        struct room_struct room;
        room.refresh = 1;

        char input;
        room_generator(&room, &player, &enemy);

        int running = 1;
        while (running)
        {
            system("cls");

            room_visual(&room);

            printf("\nYour Health: %d\n", player.health);
            printf("Your Stamina: %d\n", player.stamina);
            if (enemy.health > 0) printf("Enemy's Health: %d\n", enemy.health);
            printf("Move with WASD, q to quit\n");

            input = _getch();

            if (input == 'q')
                return 0;

            move_player(input, &player, &enemy, &room);

            if (player.stamina == 0)
            {
                for (int i = 0; i < enemy.stamina; i++)
                {
                    if (enemy.health > 0)
                    {
                        move_enemy(&enemy, &player, &room);
                    }
                }
                player.stamina = 3;
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
    for (int y = 0; y < ROOM_SIZE_Y; y++)
    {
        for (int x = 0; x < ROOM_SIZE_X; x++)
        {
            room->tile_array[y][x] = TILE_EMPTY;
            if (y == 0 || x == 0 || y == ROOM_SIZE_Y - 1 || x == ROOM_SIZE_X - 1)
            {
                room->tile_array[y][x] = TILE_WALL;
            }
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
    for (int y = 0; y < ROOM_SIZE_Y; y++)
    {
        for (int x = 0; x < ROOM_SIZE_X; x++)
        {
            if (room->tile_array[y][x] == TILE_PLAYER) setColor(14);
            if (room->tile_array[y][x] == TILE_ENEMY)  setColor(12);

            printf("%s", tiles[room->tile_array[y][x]]);

            if (room->tile_array[y][x] != TILE_EMPTY)
            {
                setColor(7);
            }

            if (room->refresh == 1)
            {
                Sleep(20);
            }
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

    if (new_x < 0 || new_x >= ROOM_SIZE_X || new_y < 0 || new_y >= ROOM_SIZE_Y) return;
    if (room->tile_array[new_y][new_x] == TILE_WALL) return;
    if (room->tile_array[new_y][new_x] == TILE_ENEMY)
    {
        enemy->health--;
        player->stamina--;

        if (enemy->health <= 0)
        {
            room->tile_array[enemy->position_y][enemy->position_x] = TILE_EMPTY;
        }
        return;
    }

    if (input == 'w' || input == 's' || input == 'a' || input == 'd')
    {
        player->stamina--;
    }

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

    room->tile_array[enemy->position_y][enemy->position_x] = TILE_EMPTY;
    enemy->position_x = new_x;
    enemy->position_y = new_y;
    room->tile_array[enemy->position_y][enemy->position_x] = TILE_ENEMY;
}
