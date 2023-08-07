#ifndef _STRUCTS
#define _STRUCTS

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "SDL.h"
#include "SDL_image.h"
#include "constants.h"

typedef struct
{
	int x, y;
} Coord;

typedef struct
{
	Coord initial, final;
} Move;

typedef struct
{
	int length;
	Move moves[MAX_NUM_OF_MOVES];
} Moves;

typedef struct
{
	bool moved;
	int value;
	int color;
	Coord coords;
	Moves moves;
} Piece;

typedef struct
{
	SDL_Renderer* renderer;
	SDL_Window* window;
	SDL_Surface* screen;
} SDL_Handler;

#endif