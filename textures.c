#include <SDl.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "board.h"
#include "textures.h"


void drawPiece(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y) {
	SDL_Rect src = { 0,0,90,90 };
	SDL_Rect dest = { x,y,90,90 };

	SDL_RenderCopy(renderer, texture, &src, &dest);
}

void drawBoard2(SDL_Renderer* renderer, SDL_Texture* texture) {
	SDL_Rect dest = { 0,0,720,720 };
	SDL_RenderCopy(renderer, texture, NULL, &dest);
}