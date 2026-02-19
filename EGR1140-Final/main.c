#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#include "types.h"
#include "player.h"
#include "enemy.h"
#include "room.h"
#include "move.h"
#include "ui.h"
#include "save.h"

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
            else { setColor(13); printf("[LUKA]"); }
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
