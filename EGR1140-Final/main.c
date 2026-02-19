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
#define ROOM_MAX_X  10
#define ROOM_MAX_Y  10

// Difficulty Constraints
#define PLAYER_HEALTH_NORMAL  8
#define PLAYER_STAMINA_NORMAL 5
#define ENEMY_HEALTH_NORMAL   3
#define ENEMY_STAMINA_NORMAL  2
#define SCORE_NORMAL          1

#define PLAYER_HEALTH_HARD    7
#define PLAYER_STAMINA_HARD   4
#define ENEMY_HEALTH_HARD     4
#define ENEMY_STAMINA_HARD    3
#define SCORE_HARD            2

#define PLAYER_HEALTH_LUKA    1
#define PLAYER_STAMINA_LUKA   3
#define ENEMY_HEALTH_LUKA     3
#define ENEMY_STAMINA_LUKA    2
#define SCORE_LUKA            5

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
int  load_highscore(void);
void save_highscore(int score);
int  title_screen(int highscore);
void room_generator(struct room_struct *room, struct player_struct *player, struct enemy_struct *enemy);
void room_visual(struct room_struct *room);
void move_player(char input, struct player_struct *player, struct enemy_struct *enemy, struct room_struct *room);
void move_enemy(struct enemy_struct *enemy, struct player_struct *player, struct room_struct *room);

void setColor(int colorValue) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, colorValue);
}

// Reads high score from save.txt; returns 0 if missing or unreadable
int load_highscore(void)
{
    int score = 0;
    FILE *f = fopen("save.txt", "r");
    if (f != NULL)
    {
        fscanf(f, "%d", &score);
        fclose(f);
    }
    return score;
}

// Writes new high score to save.txt only if it beats the current one
void save_highscore(int score)
{
    int current = load_highscore();
    if (score > current)
    {
        FILE *f = fopen("save.txt", "w");
        if (f != NULL)
        {
            fprintf(f, "%d", score);
            fclose(f);
        }
    }
}

// Returns selected difficulty: 1=Normal, 2=Hard, 3=LUKA. Returns -1 to quit.
int title_screen(int highscore)
{
    int selected = 0; // 0=Normal, 1=Hard, 2=LUKA

    while (1)
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

        // High score display
        if (highscore > 0)
        {
            setColor(14);
            printf("                        \u2605 High Score: %d Points \u2605\n\n", highscore);
        }
        else
        {
            setColor(8);
            printf("                          High Score: None\n\n");
        }

        setColor(7);
        printf("                     W / S to navigate  |  ENTER to confirm\n\n");

        // Normal
        if (selected == 0) setColor(10); else setColor(8);
        printf("                        %s  NORMAL (Score x 1)\n", selected == 0 ? ">" : " ");
        printf("                             HP: %-3d  Stamina: %d\n\n",
               PLAYER_HEALTH_NORMAL, PLAYER_STAMINA_NORMAL);

        // Hard
        if (selected == 1) setColor(12); else setColor(8);
        printf("                        %s  HARD (Score x 2)\n", selected == 1 ? ">" : " ");
        printf("                             HP: %-3d  Stamina: %d\n\n",
               PLAYER_HEALTH_HARD, PLAYER_STAMINA_HARD);

        // LUKA
        if (selected == 2) setColor(13); else setColor(8);
        printf("                        %s  EXPERT (Score x 5)\n", selected == 2 ? ">" : " ");
        printf("                             HP: %-3d  Stamina: %d\n\n",
               PLAYER_HEALTH_LUKA, PLAYER_STAMINA_LUKA);

        setColor(8);
        printf("                             Q to quit\n");
        setColor(7);

        char key = _getch();

        if (key == 'w' && selected > 0) selected--;
        if (key == 's' && selected < 2) selected++;
        if (key == '\r') break;
        if (key == 'q') return -1;
    }

    return selected + 1; // 1=Normal, 2=Hard, 3=LUKA
}

int main(void)
{
    system("chcp 65001 > nul");
    srand((unsigned int)time(NULL));

    while (1)
    {
        int highscore = load_highscore();
        int difficulty = title_screen(highscore);
        if (difficulty == -1) return 0;

        int p_health, p_stamina, e_health, e_stamina, score_multiplier;

        if (difficulty == 1)
        {
            p_health         = PLAYER_HEALTH_NORMAL;
            p_stamina        = PLAYER_STAMINA_NORMAL;
            e_health         = ENEMY_HEALTH_NORMAL;
            e_stamina        = ENEMY_STAMINA_NORMAL;
            score_multiplier = SCORE_NORMAL;
        }
        else if (difficulty == 2)
        {
            p_health         = PLAYER_HEALTH_HARD;
            p_stamina        = PLAYER_STAMINA_HARD;
            e_health         = ENEMY_HEALTH_HARD;
            e_stamina        = ENEMY_STAMINA_HARD;
            score_multiplier = SCORE_HARD;
        }
        else
        {
            p_health         = PLAYER_HEALTH_LUKA;
            p_stamina        = PLAYER_STAMINA_LUKA;
            e_health         = ENEMY_HEALTH_LUKA;
            e_stamina        = ENEMY_STAMINA_LUKA;
            score_multiplier = SCORE_LUKA;
        }

        struct room_struct room;
        room.size_x = 7;
        room.size_y = 10;
        room.refresh = 1;
        room.exit_x = -1;
        room.exit_y = -1;

        struct player_struct player;
        player.position_x = 1;
        player.position_y = 1;
        player.health = p_health;
        player.stamina = p_stamina;

        struct enemy_struct enemy;
        enemy.position_x = room.size_x - 2;
        enemy.position_y = room.size_y - 2;
        enemy.health = e_health;
        enemy.stamina = e_stamina;
        enemy.is_spawned = 1;

        char input;
        room_generator(&room, &player, &enemy);

        int running = 1;
        int floor = 1;

        while (running)
        {
            system("cls");
            room_visual(&room);

            printf("\nFloor: %d  ", floor);
            if (difficulty == 1) { setColor(10); printf("[NORMAL]"); }
            else if (difficulty == 2) { setColor(12); printf("[HARD]"); }
            else { setColor(13); printf("[EXPERT]"); }
            setColor(7);

            printf("\nYour Health: %d/%d\n", player.health, p_health);
            printf("Your Stamina: %d/%d\n", player.stamina, p_stamina);
            if (enemy.is_spawned && enemy.health > 0)
                printf("Enemy's Health: %d/%d\n", enemy.health, e_health + (floor / 2));
            else if (!enemy.is_spawned)
                printf("Find the exit! [%s]\n", tiles[TILE_EXIT]);
            printf("Move with WASD, q to quit\n");

            input = _getch();

            if (input == 'q') return 0;

            move_player(input, &player, &enemy, &room);

            if (player.position_y == room.exit_y && player.position_x == room.exit_x)
            {
                floor++;

                int new_size_x = ROOM_MIN + rand() % (ROOM_MAX_X - ROOM_MIN + 1);
                int new_size_y = ROOM_MIN + rand() % (ROOM_MAX_Y - ROOM_MIN + 1);

                room.size_x = new_size_x;
                room.size_y = new_size_y;
                room.refresh = 1;
                room.exit_x = -1;
                room.exit_y = -1;

                player.position_x = 1;
                player.position_y = 1;
                player.stamina = p_stamina;

                enemy.position_x = new_size_x - 2;
                enemy.position_y = new_size_y - 2;
                enemy.health = e_health + (floor / 2);
                enemy.stamina = e_stamina;
                enemy.is_spawned = 1;

                room_generator(&room, &player, &enemy);
                continue;
            }

            if (player.stamina == 0)
            {
                for (int i = 0; i < enemy.stamina; i++)
                {
                    if (enemy.is_spawned && enemy.health > 0)
                        move_enemy(&enemy, &player, &room);
                }
                player.stamina = p_stamina;
                room.refresh = 1;
            }

            if (player.health <= 0)
            {
                int final_score = floor * score_multiplier;
                int new_best = (final_score > highscore);
                save_highscore(final_score);

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
                printf("                      Your Final Score: %d Points\n", final_score);
                if (new_best)
                {
                    setColor(14);
                    printf("                      \u2605 NEW HIGH SCORE! \u2605\n");
                }
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
    for (int y = 0; y < ROOM_MAX_Y; y++)
        for (int x = 0; x < ROOM_MAX_X; x++)
            room->tile_array[y][x] = TILE_EMPTY;

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
        room->tile_array[enemy->position_y][enemy->position_x] = TILE_ENEMY;
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
                Sleep(1);
        }
        printf("\n");
    }
    room->refresh = 0;
}

// Player Movement
void move_player(char input, struct player_struct *player, struct enemy_struct *enemy, struct room_struct *room)
{
    if (player->stamina <= 0) return;

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

            int exit_x = room->size_x - 2;
            int exit_y = room->size_y - 2;

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

    if (room->tile_array[new_y][new_x] == TILE_EXIT) return;

    room->tile_array[enemy->position_y][enemy->position_x] = TILE_EMPTY;
    enemy->position_x = new_x;
    enemy->position_y = new_y;
    room->tile_array[enemy->position_y][enemy->position_x] = TILE_ENEMY;
}
