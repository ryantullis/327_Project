#ifndef DUNGEON_H
# define DUNGEON_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <endian.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <sys/time.h>
#include <assert.h>
#include <unistd.h>

#include "dim.h"

#define HARD_WALL_PAIR      2
#define SOFT_WALL_PAIR      3
#define IMMUTABLE_WALL_PAIR 4
#define FLOOR_PAIR          5
#define STAIR_PAIR          6
#define PLAYER_PAIR         7
#define MONSTER_PAIR        8

#define DUNGEON_X              80
#define DUNGEON_Y              21
#define MIN_ROOMS              6
#define MAX_ROOMS              10
#define ROOM_MIN_X             4
#define ROOM_MIN_Y             3
#define ROOM_MAX_X             20
#define ROOM_MAX_Y             15
#define PC_SPEED               10
#define SEMANTIC_FILE_MARKER   "RLG327-S2021"
#define SAVE_DIR               ".rlg327"
#define SAVE_FILE              "dungeon"
#define FILE_VERSION           5

 class Character;
 class Monster;
 class Pc;
 class Windows;

typedef struct heap heap_t;
typedef struct heap_node heap_node_t;

#define mappair(pair) (d->map[pair[dim_y]][pair[dim_x]])
#define pcmappair(pair) (d->pc_map[pair[dim_y]][pair[dim_x]])
#define pcmonsterpair(pair) (d->pc_monster_map[pair[dim_y]][pair[dim_x]])
#define mapxy(x, y) (d->map[y][x])
#define hardnesspair(pair) (d->hardness[pair[dim_y]][pair[dim_x]])
#define hardnessxy(x, y) (d->hardness[y][x])
#define character_mappair(pair) (d->character_map[pair[dim_y]][pair[dim_x]])
#define character_mapxy(x, y) (d->character_map[y][x])
#define MAX(X,Y) (((X) > (Y)) ? (X) : (Y))

typedef enum __attribute__ ((__packed__)) terrain_type {
  ter_debug,
  ter_wall,
  ter_wall_immutable,
  ter_floor,
  ter_floor_room,
  ter_floor_hall,
  ter_stairs,
  ter_stairs_up,
  ter_stairs_down
} terrain_type_t;

class Room {
public:
  pair_t position;
  pair_t size;
};

class Stair{
public:
  pair_t position;
  terrain_type_t direction;
};

class Monster_Path {
public:
    heap_node_t *hn;
    //TODO change to a pair_t
    uint8_t pos[2];
    uint32_t cost;
};

class Dungeon {
public:
  uint32_t num_rooms;
  Room *rooms;
  Character **characters;
  Stair *stairs;
  terrain_type_t map[DUNGEON_Y][DUNGEON_X];
  Character *character_map[DUNGEON_Y][DUNGEON_X];
  uint8_t hardness[DUNGEON_Y][DUNGEON_X];
  terrain_type_t pc_map[DUNGEON_Y][DUNGEON_X];
  Character *pc;
  int num_monsters;
  Monster_Path non_tun_path[DUNGEON_Y][DUNGEON_X];
  Monster_Path tun_path[DUNGEON_Y][DUNGEON_X];
  Windows *windows;
  int fow;
};

uint32_t in_room(Room r, Character *c);
uint32_t is_open_space(Dungeon *d, int16_t y, int16_t x);
int gen_dungeon(Dungeon *d);
void render_dungeon(Dungeon *d);
void render_terrain_map(Dungeon *d);
void render_teleport_select(Dungeon *d, heap_t *h);
void render_hardness_map(Dungeon *d);
void render_dist_map(Dungeon *d);
void render_tun_dist_map(Dungeon *d);
void render(Dungeon *d);
void delete_dungeon(Dungeon *d, heap_t *h);
int load_dungeon(Dungeon *d);
void init_dungeon(Dungeon *d);
int save_dungeon(Dungeon *d);
void new_dungeon(Dungeon *d, heap_t *h);

#endif
