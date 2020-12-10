#include <SDl.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "board.h"
#include "textures.h"
#include "pawn.h"

void TestMethod(Square*** board);

int main(int argc, char* argv[]) {


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

	Color color = { 0,0,0,0 };
	Square initialSquareForDraw;
	initialSquareForDraw.fullness = full;
	initialSquareForDraw.x1 = squareSize;
	initialSquareForDraw.x2 = 0;
	initialSquareForDraw.y1 = 0;
	initialSquareForDraw.y2 = squareSize;

	
		/*load textures*/
	SDL_Texture* WPawnT = IMG_LoadTexture(renderer, "WPawn.png");
	if (WPawnT == NULL) {
		printf("Couldn't load texture: WPawnT");
	}
	SDL_Texture* BPawnT = IMG_LoadTexture(renderer, "BPawn.png");
	if (BPawnT == NULL) {
		printf("Couldn't load texture: BPawnT");
	}
	SDL_Texture* BoardT = IMG_LoadTexture(renderer, "BoardT.jpg");
	if (BoardT == NULL) {
		printf("Couldn't load texture: BoardT");
	}



	Square** board = (Square**)malloc(8 * sizeof(Square));
	board[0] = (Square*)malloc(8 * 8 * sizeof(Square));
	for (int i = 0; i < 8; i++)
		board[i] = board[0] + i * 8;

	for (int k = 0; k < 8; k++) {
		for (int i = 0; i < 8; i++) {
			board[k][i].pieceOnSquare.type = empty;
			board[k][i].rank = i;
			board[k][i].file = k;
		}
	}

	//TestMethod(board);

	Piece WPawna = { WPawnA, 0, 1, false };
	Piece BPawna = { BPawnA, 0, 4, false };
	board[WPawna.rank][WPawna.file].pieceOnSquare = WPawna;
	board[BPawna.rank][BPawna.file].pieceOnSquare = BPawna;

	SDL_Event event;
	bool quit = false;
	while (!quit) {
		SDL_RenderClear(renderer);
		//drawBoard(&initialSquareForDraw, renderer);
		drawBoard2(renderer, BoardT);
		drawPiece(renderer, WPawnT, WPawna.rank * 90, 90 * (8 - (WPawna.file + 1)));
		drawPiece(renderer, BPawnT, BPawna.rank * 90, 90 * (8 - (BPawna.file + 1)));
		SDL_RenderPresent(renderer);
		SDL_WaitEvent(&event);
		switch (event.type) {
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT) {
				if (!WPawna.movedYet) {
					movePawnForwards2(&board, &WPawna);
					movePawnForwards2(&board, &BPawna);
					break;
				}
					movePawnForwards(&board, &WPawna);
					movePawnForwards(&board, &BPawna);
					printf("%d \n", board[WPawna.rank + 1][WPawna.file + 1].pieceOnSquare.type);
					SDL_RenderPresent(renderer);
					break;
				}
			else if (event.button.button == SDL_BUTTON_RIGHT) {
					//if (canWPawnTake(&board, &WPawna) == 0) {
					//	TestMethod(&board);
						pawnTakesRight(board, &WPawna);
					//}
					//else if (canWPawnTake(&board, &WPawna) == 1) {
						//pawnTakesLeft(&board, &WPawna);
				//	}
				}
		case SDL_QUIT:
			quit = true;
			break;
		}

	}

		free(board[0]);
		free(board);
		SDL_DestroyTexture(WPawnT);
		SDL_DestroyTexture(BPawnT);
		SDL_DestroyTexture(BoardT);
		SDL_Quit();

		return 0;
}

void TestMethod(Square*** board)
{
	for (int k = 0; k < 8; k++) {
		for (int i = 0; i < 8; i++) {
			board[k][i] = board[k][i];
		}
	}
}
