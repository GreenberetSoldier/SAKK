#include <SDl.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "board.h"
#include "textures.h"


void drawItem(SDL_Renderer* renderer, SDL_Texture* texture, int file, int rank) {
	SDL_Rect dest = { squareSize * file, squareSize * (maxRank - rank), squareSize, squareSize };
	SDL_RenderCopy(renderer, texture, NULL, &dest);
}

void drawBoard2(SDL_Renderer* renderer, SDL_Texture* texture) {
	SDL_Rect dest = { 0, 0, windowWidht, windowHeight };
	SDL_RenderCopy(renderer, texture, NULL, &dest);
}
