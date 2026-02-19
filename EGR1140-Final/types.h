#ifndef TYPES_H
#define TYPES_H

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

// Tile glyphs
extern const char *tiles[];

#endif
