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

void find_empty(unsigned *x, unsigned *y)
{
	do {
		*x = rnd_max(MAX_LEN);
		*y = rnd_max(MAX_HEI);
	} while(ground[*x][*y] != GROUND);
}

void load_level(int lvl_nb)
{
	char fname[100];
	FILE *lev;
	unsigned iLin, iCol, x, y;
	char c;

	sprintf(fname, PATH"lvls/level-%d.lvl", lvl_nb);
	lev = fopen(fname, "r");
	if(lev == NULL) {
		printf("Cannot open %s\n", fname); fflush(stdout);
		quit_stuff();
	}

	for(iCol = 0; iCol < MAX_HEI; iCol++) {
		for(iLin = 0; iLin < MAX_LEN; iLin++) {
			c = getc(lev);
			if(c == '#') ground[iCol][iLin] = WALL;
			else ground[iCol][iLin] = GROUND;
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
	unsigned x, y;

	for(i = 0; i < MAX_LEN; i++)
		for(j = 0; j < MAX_HEI; j++) {
			if(ground[i][j] == WALL || ground[i][j] == GROUND)
				break;
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

/* No params, just use curDir */
void move() {
	int nx, ny;
	switch (curDir) {
		case RIGHT:
			nx = snake->x+1;
			ny = snake->y;
			break;
		case LEFT:
			nx = (int)snake->x-1;
			ny = snake->y;
			break;
		case UP:
			nx = snake->x;
			ny = (int)snake->y-1;
			break;
		case DOWN:
			nx = snake->x;
			ny = snake->y+1;
			break;
	}
		
	if(nx == -1) nx += MAX_LEN; if(nx == MAX_LEN) nx = 0;
	if(ny == -1) ny += MAX_HEI; if(ny == MAX_HEI) ny = 0;

	add_head(nx, ny);

	check_collisions();
	
	if(!no_pop) pop_tail();
	else no_pop--;
}

void check_collisions() {
	int nx = snake->x, ny = snake->y;
	if(ground[nx][ny] == WALL)
		die_and_score();
	
	else if(ground[nx][ny] == BONUS) {
		no_pop++;
		score += BONUS_PTS;
		ground[nx][ny] = GROUND;
		timeout[nx][ny] = 0;
		
		find_empty(&nx, &ny);
		ground[nx][ny] = BONUS;
		timeout[nx][ny] = rnd_max(45) + 5;
		
	} else if(ground[nx][ny] == MALUS) {
		no_pop += 4;
		score -= MALUS_PTS;
		ground[nx][ny] = GROUND;
		timeout[nx][ny] = 0;
		
		find_empty(&nx, &ny);
		ground[nx][ny] = MALUS;
		timeout[nx][ny] = rnd_max(45) + 5;
	}
	
}

void loop()
{
	while(!kb.keys[SDLK_ESCAPE] && !kb.exit) {
		check_timeouts();
		handle_events(); //handles input, then moves
		blit_all();
		SDL_Delay(1000/BASE_FPS);
	}
}

void handle_events() {
	handle_input();
	move();
	kb.keys[SDLK_UP] = kb.keys[SDLK_RIGHT] = kb.keys[SDLK_DOWN] = kb.keys[SDLK_LEFT] = 0; //we want the player to touch *again*, so we say it ain't typed.
}
