#include <stdlib.h>
#include <stdio.h>

#include "monster.h"
#include "dungeon.h"
#include "heap.h"

typedef struct monster_path {
    heap_node_t *hn;
    uint8_t pos[2];
    int32_t cost;
} monster_path_t;

static int32_t monster_path_cmp(const void *key, const void *with) {
    return ((monster_path_t *) key)->cost - ((monster_path_t *) with)->cost;
}

//mostly complete, needs to be able to save and print
static void dijkstra_normal(dungeon_t *d)
{
    static monster_path_t path[DUNGEON_Y][DUNGEON_X], *p;
    static uint32_t initialized = 0;
    heap_t h;
    uint32_t x, y;

    if (!initialized) {
        for (y = 0; y < DUNGEON_Y; y++) {
            for (x = 0; x < DUNGEON_X; x++) {
                path[y][x].pos[dim_y] = y;
                path[y][x].pos[dim_x] = x;
            }
        }
        initialized = 1;
    }

    for (y = 0; y < DUNGEON_Y; y++) {
        for (x = 0; x < DUNGEON_X; x++) {
            path[y][x].cost = INT_MAX;
        }
    }

    path[d->pc.y][d->pc.x].cost = 0;

    heap_init(&h, monster_path_cmp, NULL);

    for (y = 0; y < DUNGEON_Y; y++) {
        for (x = 0; x < DUNGEON_X; x++) {
            if (mapxy(x, y) > ter_floor) {
                path[y][x].hn = heap_insert(&h, &path[y][x]);
            } else {
                path[y][x].hn = NULL;
            }
        }
    }

    while ((p = heap_remove_min(&h))) {
        p->hn = NULL;

        for(int i = -1; i <= 1; ++i){
            for(int j = -1; j <= 1; ++j){
                if ((path[p->pos[dim_y] + j][p->pos[dim_x] + i].hn) &&
                    (path[p->pos[dim_y] + j][p->pos[dim_x] + i].cost >
                     p->cost + hardnesspair(p->pos))) {
                    path[p->pos[dim_y] + j][p->pos[dim_x] + i].cost =
                            p->cost + hardnesspair(p->pos);
                    heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + j]
                    [p->pos[dim_x] + i].hn);
                }
            }
        }
    }
}

//not modified
static void dijkstra_tunneling(dungeon *d)
{
    static monster_path_t path[DUNGEON_Y][DUNGEON_X], *p;
    static uint32_t initialized = 0;
    heap_t h;
    uint32_t x, y;
    // initalize array of paths
    if (!initialized) {
        for (y = 0; y < DUNGEON_Y; y++) {
            for (x = 0; x < DUNGEON_X; x++) {
                path[y][x].pos[dim_y] = y;
                path[y][x].pos[dim_x] = x;
            }
        }
        initialized = 1;
    }
    
    //set each path cost to infinity
    for (y = 0; y < DUNGEON_Y; y++) {
        for (x = 0; x < DUNGEON_X; x++) {
            path[y][x].cost = INT_MAX;
        }
    }
    //set PC location cost to 0
    path[d->pc.y][d->pc.x].cost = 0;

    heap_init(&h, monster_path_cmp, NULL);

    //if it is not a border wall, add it to the heap
    for (y = 0; y < DUNGEON_Y; y++) {
        for (x = 0; x < DUNGEON_X; x++) {
            if (mapxy(x, y) != ter_wall_immutable) {
                path[y][x].hn = heap_insert(&h, &path[y][x]);
            } else {
                path[y][x].hn = NULL;
            }
        }
    }

    while ((p = heap_remove_min(&h))) {
        p->hn = NULL;
	

	//iterate through neighbors are if cost is more than cost from node, update
	for(int i = -1; i <= 1; ++i){
            for(int j = -1; j <= 1; ++j){
                if ((path[p->pos[dim_y] + j][p->pos[dim_x] + i].hn) &&
                    (path[p->pos[dim_y] + j][p->pos[dim_x] + i].cost >
                     p->cost + 1 + (hardnesspair(p->pos)/85))) {
                    path[p->pos[dim_y] + j][p->pos[dim_x] + i].cost =
		      p->cost + 1 + (hardnesspair(p->pos)/85);
                    heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + j]
                    [p->pos[dim_x] + i].hn);
                }
            }
        }
    }
}

