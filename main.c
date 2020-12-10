#include <SDl.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "board.h"
#include "textures.h"

//void TestMethod(Square*** board);

//GLOBAL BOARD ARRAY
board_t board;

int main(int argc, char* argv[]) {

	//init area
	/////////////////////////////////////////////////////
	
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			SDL_Log("Couldn't start SDL SDL: %s", SDL_GetError());
			exit(1);
		}

		SDL_Window* window = SDL_CreateWindow("Sakk", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidht, windowHeight, 0);
		if (window == NULL) {
			SDL_Log("Window creation failed: %s", SDL_GetError());
			exit(1);
		}

		Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
		SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, render_flags);
		if (renderer == NULL) {
			SDL_Log("Couldn't create renderer: %s", SDL_GetError());
			exit(1);
		}
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);
		SDL_RenderClear(renderer);

		//Color color = { 0,0,0,0 };
		//Square initialSquareForDraw;
		//initialSquareForDraw.fullness = full;
		//initialSquareForDraw.x1 = squareSize;
		//initialSquareForDraw.x2 = 0;
		//initialSquareForDraw.y1 = 0;
		//initialSquareForDraw.y2 = squareSize;

	
		/*load textures*/
		SDL_Texture* WPawnT = IMG_LoadTexture(renderer, "WPawn.png");
		if (WPawnT == NULL) {
			printf("Couldn't load texture: WPawnT");
			return 1;
		}
		SDL_Texture* BPawnT = IMG_LoadTexture(renderer, "BPawn.png");
		if (BPawnT == NULL) {
			printf("Couldn't load texture: BPawnT");
			return 1;
		}
		SDL_Texture* BoardT = IMG_LoadTexture(renderer, "BoardT.jpg");
		if (BoardT == NULL) {
			printf("Couldn't load texture: BoardT");
			return 1;
		}

	
		//initializing empty board
		for (unsigned char k = 0; k < filenum; k++)
			for (unsigned char i = 0; i < ranknum; i++)
				board[k][i].pieceOnSquare = NULL;

	/////////////////////////////////////////////////////

	//TestMethod(board);

	//create all pieces
	Piece WPawna = { WPawnA, (unsigned char) 1, (unsigned char) 0, false, false};
	Piece BPawna = { BPawnA, (unsigned char) 4, (unsigned char) 0, false, false};
	board[WPawna.rank][WPawna.file].pieceOnSquare = &WPawna;
	board[BPawna.rank][BPawna.file].pieceOnSquare = &BPawna;

	//create utils
	SDL_Event event;
	bool quit = false;
	unsigned char file, rank;

	while (!quit) {
		SDL_RenderClear(renderer);
		//drawBoard(&initialSquareForDraw, renderer);
		drawBoard2(renderer, BoardT);
		
		//draw all pieces
		drawPiece(renderer, WPawnT, WPawna.rank * squareSize, squareSize * (filenum - (WPawna.file + 1)));
		drawPiece(renderer, BPawnT, BPawna.rank * squareSize, squareSize * (filenum - (BPawna.file + 1)));
		
		//actual screen change
		SDL_RenderPresent(renderer);
		SDL_WaitEvent(&event);
		switch (event.type) {
			case SDL_MOUSEBUTTONDOWN:
				//currently only handling left mouse clicks
				if ((event.button.button) != SDL_BUTTON_LEFT)
					break;

				//clicks outside the game window don't matter
				if (event.button.x < 0                ||
					event.button.y < 0                ||
					event.button.x > (int)windowWidht ||
					event.button.y > (int)windowHeight )
					break;
				
				file = event.button.x / squareSize + 1;
				rank = event.button.y / squareSize + 1;
				
				deselectAll(board);
				detargetAll(board);
				
				//make clicked field selected
				if (board[file][rank].pieceOnSquare != NULL)
					board[file][rank].selected = true;

				//calculate possible target fields
				calculateTargets();
				

				break;

			case SDL_QUIT:
				quit = true;
				break;
		}

	} //while


	SDL_DestroyTexture(WPawnT);
	SDL_DestroyTexture(BPawnT);
	SDL_DestroyTexture(BoardT);
	SDL_Quit();

	return 0;
}

/*
void TestMethod(Square*** board){
	for (int k = 0; k < 8; k++) {
		for (int i = 0; i < 8; i++) {
			board[k][i] = board[k][i];
		}
	}
}

*/
