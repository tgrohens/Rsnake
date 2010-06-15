/*
 * main.c
 *
 *  Created on: 14 juin 2010
 *      Author: matthieu
 */

#include "rsnake.h"

/* Initialize everything */
void init_stuff() {
	/* First the SDL */
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
	screen = SDL_SetVideoMode(MAX_X*PX_SIZE, MAX_Y*PX_SIZE + TEXTZONE_SIZE, 32,
			SDL_HWSURFACE|SDL_DOUBLEBUF);
	fill_colors();

    SDL_WM_SetCaption("Rsnake_new 0.2-pre", NULL);

    TTF_Init();
    info_font = TTF_OpenFont("comic.ttf", 15);
    score_font = TTF_OpenFont("comic.ttf", 40);

	default_dir = LEFT;

	snake = malloc(sizeof(Snake));
	snake->next = NULL;

	score = 0;
	cur_lvl = 1;
	lives = 2;
	load_level(cur_lvl);

	no_pop = 0;
}

void die_and_score() {
	if(--lives >= 0) {
		load_level(cur_lvl);
		return;
	}
	//show_score();
	quit_stuff();
}

void quit_stuff(int dummy) {
	TTF_CloseFont(score_font);
	TTF_CloseFont(info_font);
	TTF_Quit();

	SDL_Quit();

	free_snake();

	exit(0);
}

int main(int argc, char *argv[]) {
	signal(SIGINT, quit_stuff);
	srand(time(NULL));

	init_stuff();
	blit_all();

	loop();

	quit_stuff(0);

	return 0;
}
