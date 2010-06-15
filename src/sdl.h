/*
 * sdl.h
 *
 *  Created on: 14 juin 2010
 *      Author: matthieu
 */

#ifndef SDL_H_
#define SDL_H_

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

SDL_Surface * screen;
SDL_TimerID automove;

TTF_Font * info_font;
TTF_Font * score_font;

#endif /* SDL_H_ */
