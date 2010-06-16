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

//char *path = [[NSSearchPathForDirectoriesInDomains(NSApplicationDirectory, NSUserDomainMask, YES) objectAtIndex:0] cStringUsingEncoding:NSASCIIStringEncoding];
#define PATH "/Users/theotime/Desktop/Rsnake/rsnake/"
//#endif

#define MAX_LEN 40
#define MAX_HEI 30
#define PX_SIZE 16
#define TEXTZONE_SIZE 25

#define BONUS_PTS 80
#define MALUS_PTS 150

#define NXT_LVL_PTS 1000
#define MAX_LVL 30

#define BASE_FPS 20

int score;
int cur_lvl;
int lives;

int no_pop;

typedef enum { UP, DOWN, RIGHT, LEFT } Direction;
Direction curDir;

typedef enum { GROUND, WALL, BONUS, MALUS } Tile;
Tile ground[MAX_LEN][MAX_HEI];
unsigned timeout[MAX_LEN][MAX_HEI]; /* Timeout of objects in turns. */

typedef struct Snake {
	unsigned x, y;
	struct Snake * next;
} Snake;

Snake * snake;

typedef struct Input {
	char keys[SDLK_LAST];
	int mx, my; //mouse
	int mbut[6]; //mouse buttons
	char exit;
} Input;

Input kb;

#include "sdl.h"

void init_stuff();
void win();
void die_and_score();
void quit_stuff();

void add_head(unsigned x, unsigned y);
void pop_tail();
void free_snake();
int has_snake(unsigned x, unsigned y);
void check_collisions();
void move();
void handle_input();
void handle_events();

void fill_colors();
void blit_all();

void load_level(int lvl_nb);
void loop();

#endif /* RSNAKE_H_ */
