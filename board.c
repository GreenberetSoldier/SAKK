#include <SDl.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "board.h"


void deselectAll(board_t* board) {
	for (int i = 0; i < filenum; i++)
		for (int j = 0; j < filenum; j++)
			board[i][j]->selected = false;
}

void detargetAll(board_t* board) {
	for (int i = 0; i < filenum; i++)
		for (int j = 0; j < filenum; j++)
			board[i][j]->target = false;
}

void calculateTargets() {

}


