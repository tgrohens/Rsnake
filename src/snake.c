/*
 * snake.c
 *
 *  Created on: Jun 15, 2010
 *      Author: matthieu
 */

#include "rsnake.h"

int rnd_max(int max)
{
	return rand() % max;
}

void find_empty(int * x, int * y)
{
	do {
		*x = rnd_max(MAX_X);
		*y = rnd_max(MAX_Y);
	} while(ground[*x][*y] != GROUND);
}

void load_level(int lvl_nb)
{
	char fname[50];
	FILE *lev;
	int i, j, x, y;
	char c;

	sprintf(fname, "level-%d.lvl", lvl_nb);
	lev = fopen(fname, "r");
	if(lev == NULL) {
		printf("Cannot open %s\n", fname); fflush(stdout);
		quit_stuff();
	}

	for(j = 0; j < MAX_Y; j++) {
		for(i = 0; i < MAX_X; i++) {
			c = getc(lev);
			if(c == '#') ground[i][j] = WALL;
			else ground[i][j] = GROUND;
		}
		getc(lev); /* This should be a \n */
	}

	find_empty(&x, &y);
	ground[x][y] = BONUS;
	timeout[x][y] = rnd_max(45) + 5;


	find_empty(&x, &y);
	ground[x][y] = MALUS;
	timeout[x][y] = rnd_max(45) + 5;

	find_empty(&x, &y);
	ground[x][y] = MALUS;
	timeout[x][y] = rnd_max(45) + 5;

	find_empty(&x, &y);
	ground[x][y] = MALUS;
	timeout[x][y] = rnd_max(45) + 5;

	fclose(lev);

	free_snake();
	/* Then the snake */
	snake = malloc(sizeof(Snake));
	snake->x = 9; snake->y = 13;
	snake->next = NULL;

	add_head(10, 13); add_head(11, 13); add_head(12, 13); add_head(13, 13);
}

void check_timeouts()
{
	Tile type;
	int i, j;
	int x, y;

	for(i = 0; i < MAX_X; i++)
		for(j = 0; j < MAX_Y; j++) {
			/*if(ground[i][j] == WALL || ground[i][j] == GROUND)
				break;*/
			timeout[i][j]--;
			if(!timeout[i][j]) {
				type = ground[i][j];
				ground[i][j] = GROUND;

				find_empty(&x, &y);
				ground[x][y] = type;
				timeout[x][y] = rnd_max(45) + 5;
			}
		}
}

/** The params should contain 0, 1 or -1. */
void move(int x, int y)
{
	int new_x = (snake->x + x);
	int new_y = (snake->y + y);

	if(snake->next && snake->next->x == new_x && snake->next->y && new_y)
		return; /* Impossible move */

	if(new_x < 0) new_x = MAX_X -1; if(new_x >= MAX_X) new_x = 0;
	if(new_y < 0) new_y = MAX_Y -1; if(new_y >= MAX_Y) new_y = 0;

	add_head(new_x, new_y);


	if(has_snake(new_x, new_y) == 1)
		die_and_score();
	else if(ground[new_x][new_y] == WALL)
		die_and_score();

	else if(ground[new_x][new_y] == BONUS) {
		no_pop++;
		score += BONUS_PTS;
		ground[new_x][new_y] = GROUND;
		timeout[new_x][new_y] = 0;

		find_empty(&new_x, &new_y);
		ground[new_x][new_y] = BONUS;
		timeout[new_x][new_y] = rnd_max(45) + 5;

	} else if(ground[new_x][new_y] == MALUS) {
		no_pop += 4;
		score -= MALUS_PTS;
		ground[new_x][new_y] = GROUND;
		timeout[new_x][new_y] = 0;

		find_empty(&new_x, &new_y);
		ground[new_x][new_y] = MALUS;
		timeout[new_x][new_y] = rnd_max(45) + 5;
	}

	if(!no_pop) pop_tail();
	else no_pop--;
}


void auto_move()
{
	switch(default_dir) {
	case UP:		move(0, -1); break;
	case DOWN:		move(0, 1); break;
	case LEFT:		move(-1, 0); break;
	case RIGHT:		move(1, 0); break;
	}
}


/* TODO: make auto-move, non blocking management of events */
int loop()
{
	SDL_Event event;

	while(42) {
		SDL_WaitEvent(&event);
		switch(event.type){

		case SDL_QUIT:
			quit_stuff(0);
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_UP:
				default_dir = UP; move(0, -1); break;
			case SDLK_DOWN:
				default_dir = DOWN; move(0, 1); break;
			case SDLK_LEFT:
				default_dir = LEFT; move(-1, 0); break;
			case SDLK_RIGHT:
				default_dir = RIGHT; move(1, 0); break;
			default:
				continue;
				//auto_move();
				break;
			}
			break;

		default: continue;
			//auto_move();
		}
		blit_all();

		if(score >= NXT_LVL_PTS) {
			cur_lvl++;
			load_level(cur_lvl);
			score = 0;
			lives += cur_lvl/3;
		}

		check_timeouts();
	}
}
