#ifndef ROOM_H
#define ROOM_H

#include "types.h"
#include "player.h"
#include "enemy.h"

struct room_struct
{
    int tile_array[ROOM_MAX_Y][ROOM_MAX_X];
    int size_x;
    int size_y;
    int refresh;
    int exit_x;
    int exit_y;
};

void room_generator(struct room_struct *room, struct player_struct *player, struct enemy_struct *enemy);
void room_visual(struct room_struct *room);

#endif
