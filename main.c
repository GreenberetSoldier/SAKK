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
		SDL_Texture* targetT      = IMG_LoadTexture(renderer, "target.png");         if (targetT == NULL)      {printf("Couldn't load texture: targetT");exit(1);}
		SDL_Texture* selectframeT = IMG_LoadTexture(renderer, "selection_frame.png");if (selectframeT == NULL) {printf("Couldn't load texture: selectframeT");exit(1);}
		SDL_Texture* WPawnT       = IMG_LoadTexture(renderer, "WPawn.png");          if (WPawnT == NULL)       {printf("Couldn't load texture: WPawnT");exit(1);}
		SDL_Texture* BPawnT       = IMG_LoadTexture(renderer, "BPawn.png");          if (BPawnT == NULL)       {printf("Couldn't load texture: BPawnT");exit(1);}
		SDL_Texture* WRookT       = IMG_LoadTexture(renderer, "WRook.png");			 if (WPawnT == NULL)       {printf("Couldn't load texture: WRookT");exit(1);}
		SDL_Texture* BRookT       = IMG_LoadTexture(renderer, "BRook.png");          if (BRookT == NULL)       {printf("Couldn't load texture: BRookT");exit(1);}
		SDL_Texture* WBishopT     = IMG_LoadTexture(renderer, "WBishop.png");        if (WPawnT == NULL)       {printf("Couldn't load texture: WBishopT");exit(1);}
		SDL_Texture* BBishopT     = IMG_LoadTexture(renderer, "BBishop.png");		 if (BBishopT == NULL)     {printf("Couldn't load texture: BBishopT");exit(1);}
		SDL_Texture* BoardT       = IMG_LoadTexture(renderer, "BoardT.jpg");         if (BoardT == NULL)       {printf("Couldn't load texture: BoardT");exit(1);}

		//initializing empty board
		for (int file = minFile; file <= maxFile; file++)
			for (int rank = 0; rank <= maxRank; rank++)
				board[file][rank].pieceOnSquare = NULL;
		detargetAll(board);

		//create all pieces
		Piece WPawna = { WPawnA, (unsigned char)minFile, (unsigned char)minRank + 1, false, false };
		Piece WRooka = { WRookA, (unsigned char)minFile, (unsigned char)minRank, false, false };
		Piece BPawna = { BPawnA, (unsigned char)minFile, (unsigned char)maxRank - 1, false, false };
		Piece BRooka = { BRookA, (unsigned char)minFile, (unsigned char)maxRank, false, false };
		Piece WBishopa = { WBishopA, (unsigned char)2, (unsigned char)minRank, false, false };
		Piece BBishopa = { BBishopA, (unsigned char)2, (unsigned char)maxRank, false, false };

		board[WPawna.file][WPawna.rank].pieceOnSquare = &WPawna;
		board[BPawna.file][BPawna.rank].pieceOnSquare = &BPawna;
		board[WRooka.file][WRooka.rank].pieceOnSquare = &WRooka;
		board[BRooka.file][BRooka.rank].pieceOnSquare = &BRooka;
		board[WBishopa.file][WBishopa.rank].pieceOnSquare = &WBishopa;
		board[BBishopa.file][BBishopa.rank].pieceOnSquare = &BBishopa;
		/////////////////////////////////////////////////////

	//create utility variables
	SDL_Event event;
	bool quit = false;
	struct Square* clicked = NULL;
	struct Square* selected = NULL;

	while (!quit) {
		SDL_RenderClear(renderer);
		drawBoard2(renderer, BoardT);
		//draw all pieces
		if (WPawna.taken == false)   drawItem(renderer, WPawnT,   WPawna.file,   WPawna.rank);
		if (BPawna.taken == false)   drawItem(renderer, BPawnT,   BPawna.file,   BPawna.rank);
		if (WRooka.taken == false)   drawItem(renderer, WRookT,   WRooka.file,   WRooka.rank);
		if (BRooka.taken == false)   drawItem(renderer, BRookT,   BRooka.file,   BRooka.rank);
		if (WBishopa.taken == false) drawItem(renderer, WBishopT, WBishopa.file, WBishopa.rank);
		if (BBishopa.taken == false) drawItem(renderer, BBishopT, BBishopa.file, BBishopa.rank);
		//draw selection frame and all target points
		if (selected != NULL)
			drawItem(renderer, selectframeT, selected->pieceOnSquare->file, selected->pieceOnSquare->rank);
		for (int rank = maxRank; rank >= minRank; rank--) {
			for (int file = minFile; file <= maxFile; file++) {
				if (board[file][rank].target == true)
					drawItem(renderer, targetT, file, rank);
			}
		}
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
				
				clicked = &board[event.button.x / squareSize][maxRank - (event.button.y / squareSize)];
		
				//if there was no selection before
				if (selected == NULL) {
					//make clicked field selected only if it has a piece
					if (clicked->pieceOnSquare != NULL) {
						selected = clicked;
						calculateValidTargets(board, selected->pieceOnSquare);
					}
					//do nothing if clicked on empty field
				}
				else {
					if (clicked->target == true) {
						performMove(selected, clicked, event.button.x / squareSize, maxRank - (event.button.y / squareSize));
					}
					selected = NULL;
					detargetAll(board);
				}

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


