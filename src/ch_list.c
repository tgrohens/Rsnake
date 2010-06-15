/*
 * ch_list.c
 *
 *  Created on: 14 juin 2010
 *      Author: matthieu
 */

#include "rsnake.h"

void add_head(unsigned x, unsigned y)
{
	Snake * new = malloc(sizeof(Snake));
	new->x = x;
	new->y = y;

	new->next = snake;
	snake = new;
}

void pop_tail()
{
	Snake * snk = snake;

	while(snk->next->next != NULL)
		snk = snk->next;

	free(snk->next);
	snk->next = NULL;
}

void free_snake()
{
	while(snake->next != NULL) pop_tail();
	free(snake);
	snake = NULL;
}

/* The head is checked after for subtle collision checking reasons */
int has_snake(unsigned x, unsigned y)
{
	Snake * snk = snake;

	snk = snk->next;
	while(snk != NULL) {
		if(snk->x == x && snk->y == y) return 1;
		snk = snk->next;
	}
	if(snake->x == x && snake->y == y) return 2; /* 2 : head is here */

	return 0;
}
