/*
 * sdl.c
 *
 *  Created on: 14 juin 2010
 *      Author: matthieu
 */

/* TODO: add a splash and a credits screen. */

#include "rsnake.h"

static SDL_Surface *txt_surf, *info_background;

Uint32 col_wall;
Uint32 col_ground;
Uint32 col_bonus;
Uint32 col_malus;

Uint32 col_snake_hd;
Uint32 col_snake;

void fill_colors() {
	col_wall = SDL_MapRGB(screen->format, 128, 64, 0);
	col_ground = SDL_MapRGB(screen->format, 0, 255, 0);
	col_bonus = SDL_MapRGB(screen->format, 102, 204, 255);
	col_malus = SDL_MapRGB(screen->format, 255, 0, 0);

	col_snake_hd = SDL_MapRGB(screen->format, 255, 128, 0);
	col_snake = SDL_MapRGB(screen->format, 255, 255, 102);
}

void show_info()
{
	char info_txt[100];
	SDL_Rect position;
	SDL_Color white = { 255, 255, 255 };

	sprintf(info_txt, "NIVEAU : %2d SCORE : %4d VIES : %2d", cur_lvl, score, lives);
	txt_surf = TTF_RenderText_Blended(info_font, info_txt, white);

	position.x = 0; position.y = MAX_HEI * PX_SIZE;

	info_background = SDL_CreateRGBSurface(SDL_HWSURFACE, MAX_LEN*PX_SIZE,
			TEXTZONE_SIZE, 32, 0, 0, 0, 0);
	SDL_FillRect(info_background, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
	SDL_BlitSurface(info_background, NULL, screen, &position);

	position.x = 10; position.y += 3;

	SDL_BlitSurface(txt_surf, NULL, screen, &position);
}

void blit_all()
{
	/* TODO: blit the score */

	SDL_Surface *to_blit[MAX_LEN][MAX_HEI];
	SDL_Rect position;
	Uint32 color;
	int i, j;

	/* We blit the whole thing */
	for(i = 0; i < MAX_LEN; i++) {
		for(j = 0; j < MAX_HEI; j++) {
			position.x = i *PX_SIZE;
			position.y = j *PX_SIZE;

			if(has_snake(i, j) == 2)
				color = col_snake_hd;
			else if(has_snake(i, j) == 1)
				color = col_snake;
			else if(ground[i][j] == GROUND)
				color = col_ground;
			else if(ground[i][j] == WALL)
				color = col_wall;
			else if(ground[i][j] == BONUS)
				color = col_bonus;
			else
				color = col_malus;

			to_blit[i][j] = SDL_CreateRGBSurface(SDL_HWSURFACE, PX_SIZE, PX_SIZE,
					32, 0, 0, 0, 0);
			SDL_FillRect(to_blit[i][j], NULL, color);
			SDL_BlitSurface(to_blit[i][j], NULL, screen, &position);
		}
	}

	show_info();
	SDL_Flip(screen);

	for(i = 0; i < MAX_LEN; i++)
		for(j = 0; j < MAX_HEI; j++)
			SDL_FreeSurface(to_blit[i][j]);

	SDL_FreeSurface(txt_surf);
	SDL_FreeSurface(info_background);
}

void handle_input() {
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_KEYDOWN:
				kb.keys[event.key.keysym.sym] = 1;
				curDir =kb.keys[SDLK_UP]?UP:
				kb.keys[SDLK_RIGHT]?RIGHT:
				kb.keys[SDLK_DOWN]?DOWN:
				kb.keys[SDLK_LEFT]?LEFT:
				curDir;
				break;
			case SDL_KEYUP:
				kb.keys[event.key.keysym.sym] = 0;
				break;
			case SDL_QUIT:
				kb.exit = 1;
				break;
			case SDL_MOUSEMOTION:
				kb.mx = event.motion.x;
				kb.my = event.motion.y;
				break;
			case SDL_MOUSEBUTTONDOWN:
				kb.mbut[event.button.button-1] = 1;
				break;
			case SDL_MOUSEBUTTONUP:
				kb.mbut[event.button.button-1] = 0;
				break;
		}
	}
}