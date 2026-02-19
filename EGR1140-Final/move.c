#include "move.h"
#include "types.h"

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
