/*
 * rsnake.h
 *
 *  Created on: 14 juin 2010
 *      Author: matthieu
 */

#ifndef RSNAKE_H_
#define RSNAKE_H_

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//#ifdef __OBJC__

char *path = [[NSSearchPathForDirectoriesInDomains(NSApplicationDirectory, NSUserDomainMask, YES) objectAtIndex:0] cStringUsingEncoding:NSASCIIStringEncoding];
//#endif

#define MAX_X 40
#define MAX_Y 30
#define PX_SIZE 16
#define TEXTZONE_SIZE 25

#define BONUS_PTS 80
#define MALUS_PTS 150

#define NXT_LVL_PTS 1000
#define MAX_LVL 30

int score;
int cur_lvl;
int lives;

int no_pop;

typedef enum { UP, DOWN, RIGHT, LEFT } Direction;
Direction default_dir;

typedef enum { GROUND, WALL, BONUS, MALUS } Tile;
Tile ground[MAX_X][MAX_Y];
unsigned timeout[MAX_X][MAX_Y]; /* Timeout of objects in turns. */

typedef struct Snake {
	unsigned x, y;
	struct Snake * next;
} Snake;

Snake * snake;

#include "sdl.h"

void init_stuff();
void die_and_score();
void quit_stuff();

void add_head(unsigned x, unsigned y);
void pop_tail();
void free_snake();
int has_snake(unsigned x, unsigned y);

void fill_colors();
void blit_all();

void load_level(int lvl_nb);
int loop();

#endif /* RSNAKE_H_ */
