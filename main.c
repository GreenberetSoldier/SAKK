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
	
		/*load textures*/
		SDL_Texture* targetT = IMG_LoadTexture(renderer, "target.png");
		if (targetT == NULL) {
			printf("Couldn't load texture: targetT");
			return 1;
		}
		SDL_Texture* selectframeT = IMG_LoadTexture(renderer, "selection_frame.png");
		if (selectframeT == NULL) {
			printf("Couldn't load texture: selectframeT");
			return 1;
		}
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
		SDL_Texture* WRookT = IMG_LoadTexture(renderer, "WRook.png");
		if (WPawnT == NULL) {
			printf("Couldn't load texture: WRookT");
			return 1;
		}
		SDL_Texture* BRookT = IMG_LoadTexture(renderer, "BRook.png");
		if (BRookT == NULL) {
			printf("Couldn't load texture: BRookT");
			return 1;
		}
		SDL_Texture* WBishopT = IMG_LoadTexture(renderer, "WBishop.png");
		if (WPawnT == NULL) {
			printf("Couldn't load texture: WBishopT");
			return 1;
		}
		SDL_Texture* BBishopT = IMG_LoadTexture(renderer, "BBishop.png");
		if (BBishopT == NULL) {
			printf("Couldn't load texture: BBishopT");
			return 1;
		}

		SDL_Texture* BoardT = IMG_LoadTexture(renderer, "BoardT.jpg");
		if (BoardT == NULL) {
			printf("Couldn't load texture: BoardT");
			return 1;
		}


	
		//initializing empty board
		for (int file = minFile; file <= maxFile; file++)
			for (int rank = 0; rank <= maxRank; rank++)
				board[file][rank].pieceOnSquare = NULL;
		detargetAll(board);

		//TestMethod(board);

		//create all pieces
		Piece WPawna = { WPawnA, (unsigned char)minFile, (unsigned char)minRank + 1, false, false };
		Piece WRooka = { WRookA, (unsigned char)minFile, (unsigned char)minRank, false, false };

		Piece BPawna = { BPawnA, (unsigned char)minFile, (unsigned char)maxRank - 1, false, false };
		Piece BRooka = { BRookA, (unsigned char)minFile, (unsigned char)maxRank, false, false };

		Piece WBishopa = { WBishopA, (unsigned char)3, (unsigned char)minRank, false, false };
		Piece BBishopa = { BBishopA, (unsigned char)3, (unsigned char)maxRank, false, false };

		board[WPawna.file][WPawna.rank].pieceOnSquare = &WPawna;
		board[BPawna.file][BPawna.rank].pieceOnSquare = &BPawna;
		board[WRooka.file][WRooka.rank].pieceOnSquare = &WRooka;
		board[BRooka.file][BRooka.rank].pieceOnSquare = &BRooka;
		board[WBishopa.file][WBishopa.rank].pieceOnSquare = &WBishopa;
		board[BBishopa.file][BBishopa.rank].pieceOnSquare = &BBishopa;
		/////////////////////////////////////////////////////

	//create utils
	SDL_Event event;
	bool quit = false;
	unsigned char clickedFile, clickedRank;
	//struct Piece* selected = NULL;
	struct Square* selected = NULL;

	while (!quit) {
		SDL_RenderClear(renderer);
		//drawBoard(&initialSquareForDraw, renderer);
		drawBoard2(renderer, BoardT);
		//draw all pieces
		//TODO: taken state of pieces has to be taken into consideration
		drawPiece(renderer, WPawnT, squareSize * WPawna.file, squareSize * (maxRank - WPawna.rank));
		drawPiece(renderer, BPawnT, squareSize * BPawna.file, squareSize * (maxRank - BPawna.rank));
		drawPiece(renderer, WRookT, squareSize * WRooka.file, squareSize * (maxRank - WRooka.rank));
		drawPiece(renderer, BRookT, squareSize * BRooka.file, squareSize * (maxRank - BRooka.rank));
		drawPiece(renderer, WBishopT, squareSize* WBishopa.file, squareSize* (maxRank - WBishopa.rank));
		drawPiece(renderer, BBishopT, squareSize* BBishopa.file, squareSize* (maxRank - BBishopa.rank));
		drawTargets(renderer, board, targetT);
		if (selected != NULL)
			drawSelection(renderer, selectframeT, squareSize * selected->pieceOnSquare->file, squareSize * (maxRank - selected->pieceOnSquare->rank));

		//actual screen change
		SDL_RenderPresent(renderer);
		SDL_WaitEvent(&event);
		switch (event.type) {
			case SDL_MOUSEBUTTONDOWN:
				printf("\n");
				
				//currently only handling left mouse clicks
				if ((event.button.button) != SDL_BUTTON_LEFT)
					break;

				//clicks outside the game window don't matter
				if (event.button.x < 0                ||
					event.button.y < 0                ||
					event.button.x > (int)windowWidht ||
					event.button.y > (int)windowHeight )
					break;
				
				clickedFile = event.button.x / squareSize;
				clickedRank = maxRank - (event.button.y / squareSize);
		
				if (selected != NULL) {
					if (board[clickedFile][clickedRank].target == true) {
						performMove(selected, &board[clickedFile][clickedRank], clickedFile, clickedRank);
					}
					selected = NULL;
					detargetAll(board);
				}
				//if there was no selection before
				else {
					//make clicked field selected
					if (board[clickedFile][clickedRank].pieceOnSquare != NULL) {
						selected = &board[clickedFile][clickedRank];
						calculateValidTargets(board, selected->pieceOnSquare);
					}
					//do nothing if clicked on empty field
				}
				

				//DEBUG
				for (int rank = maxRank; rank >= minRank; rank--) {
					for (int file = minFile; file <= maxFile; file++) {
						if (board[file][rank].target == true)
							printf("1 ");
						else
							printf("0 ");
					}
					printf("\n");
				}
				//DEBUG

				break;

			case SDL_QUIT:
				quit = true;
				break;
		} //switch

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
