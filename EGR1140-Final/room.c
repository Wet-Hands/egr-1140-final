#include <stdio.h>
#include <windows.h>
#include "room.h"
#include "ui.h"

const char *tiles[] = { "[ ]", "[\u263A]", "[\u235F]", "[E]", "[\u2661]", "[P]", "[\u25A0]", "[$]" };

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
