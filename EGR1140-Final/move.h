#ifndef MOVE_H
#define MOVE_H

#include "player.h"
#include "enemy.h"
#include "room.h"

void move_player(char input, struct player_struct *player, struct enemy_struct *enemy, struct room_struct *room);
void move_enemy(struct enemy_struct *enemy, struct player_struct *player, struct room_struct *room);

#endif
