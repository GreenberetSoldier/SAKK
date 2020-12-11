#include <SDl.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "board.h"
#include "textures.h"


void drawPiece(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y) {
	SDL_Rect src  = { 0, 0, squareSize, squareSize };
	SDL_Rect dest = { x, y, squareSize, squareSize };

	SDL_RenderCopy(renderer, texture, &src, &dest);
}

void drawSelection(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y) {
	SDL_Rect dest = { x, y, squareSize, squareSize };

	SDL_RenderCopy(renderer, texture, NULL, &dest);
}

void drawBoard2(SDL_Renderer* renderer, SDL_Texture* texture) {
	SDL_Rect dest = { 0, 0, 720, 720 };
	SDL_RenderCopy(renderer, texture, NULL, &dest);
}


void drawTargets(SDL_Renderer* renderer, board_t* board, SDL_Texture* texture) {
	
	SDL_Rect dest; //(x,y,w,h)

	for (int rank = maxRank; rank >= minRank; rank--)
		for (int file = minFile; file <= maxFile; file++)
			if (board[file][rank]->target == true) {
				dest = (SDL_Rect){ squareSize * rank, squareSize * (maxFile - file), squareSize, squareSize };
				if (SDL_RenderCopy(renderer, texture, NULL, &dest) != 0)
					printf("XXX");
			}
}
