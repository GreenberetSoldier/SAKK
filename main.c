#include <SDl.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "board.h"
#include "textures.h"

//GLOBAL VARIABLES
board_t board;
Move* head = NULL;		// the latest move
Move* tail = NULL;		// the first move (lehet hogy nem is fog kelleni)
Move* current = NULL;	// pointer for walking on the list of moves
const enum PieceType maxBlack = BPawnH;
const enum PieceType minWhite = WPawnA;

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
		SDL_Texture* WKnightT     = IMG_LoadTexture(renderer, "WKnight.png");		 if (WKnightT == NULL)     { printf("Couldn't load texture: WKightT"); exit(1); }
		SDL_Texture* BKnightT     = IMG_LoadTexture(renderer, "BKnight.png");		 if (WKnightT == NULL) { printf("Couldn't load texture: BKightT"); exit(1); }
		SDL_Texture* WQueenT      = IMG_LoadTexture(renderer, "WQueen.png");         if (WQueenT == NULL) { printf("Couldn't load texture: WQuennT"); exit(1); }
		SDL_Texture* BQueenT      = IMG_LoadTexture(renderer, "BQueen.png");         if (BQueenT == NULL) { printf("Couldn't load texture: BQuennT"); exit(1); }
		SDL_Texture* BoardT       = IMG_LoadTexture(renderer, "BoardT.jpg");         if (BoardT == NULL)       {printf("Couldn't load texture: BoardT");exit(1);}
		SDL_Texture* WKingT		  = IMG_LoadTexture(renderer, "WKing.png");         if (WKingT == NULL) { printf("Couldn't load texture: WKingT"); exit(1); }
		SDL_Texture* BKingT       = IMG_LoadTexture(renderer, "BKing.png");         if (BKingT == NULL) { printf("Couldn't load texture: BKingT"); exit(1); }
		
		//initializing empty board
		for (int file = minFile; file <= maxFile; file++)
			for (int rank = 0; rank <= maxRank; rank++)
				board[file][rank].pieceOnSquare = NULL;
		detargetAll(board);

		//create all pieces
		//////////////////////////////////////////////////////////////////////////////////////////////////
		
		//white pawns
		Piece WPawna = { WPawnA, (unsigned char)minFile, (unsigned char)minRank + 1, false, false };
		Piece WPawnb = { WPawnB, (unsigned char)minFile + 1, (unsigned char)minRank + 1, false, false };
		Piece WPawnc = { WPawnC, (unsigned char)minFile + 2, (unsigned char)minRank + 1, false, false };
		Piece WPawnd = { WPawnD, (unsigned char)minFile + 3, (unsigned char)minRank + 1, false, false };
		Piece WPawne = { WPawnE, (unsigned char)minFile + 4, (unsigned char)minRank + 1, false, false };
		Piece WPawnf = { WPawnF, (unsigned char)minFile + 5, (unsigned char)minRank + 1, false, false };
		Piece WPawng = { WPawnG, (unsigned char)minFile + 6, (unsigned char)minRank + 1, false, false };
		Piece WPawnh = { WPawnH, (unsigned char)minFile + 7, (unsigned char)minRank + 1, false, false };

		board[WPawna.file][WPawna.rank].pieceOnSquare = &WPawna;
		board[WPawnb.file][WPawnb.rank].pieceOnSquare = &WPawnb;
		board[WPawnc.file][WPawnc.rank].pieceOnSquare = &WPawnc;
		board[WPawnd.file][WPawnd.rank].pieceOnSquare = &WPawnd;
		board[WPawne.file][WPawne.rank].pieceOnSquare = &WPawne;
		board[WPawnf.file][WPawnf.rank].pieceOnSquare = &WPawnf;
		board[WPawng.file][WPawng.rank].pieceOnSquare = &WPawng;
		board[WPawnh.file][WPawnh.rank].pieceOnSquare = &WPawnh;

		//black pawns
		Piece BPawna = { BPawnA, (unsigned char)minFile, (unsigned char)maxRank - 1, false, false };
		Piece BPawnb = { BPawnB, (unsigned char)minFile + 1, (unsigned char)maxRank - 1, false, false };
		Piece BPawnc = { BPawnC, (unsigned char)minFile + 2, (unsigned char)maxRank - 1, false, false };
		Piece BPawnd = { BPawnD, (unsigned char)minFile + 3, (unsigned char)maxRank - 1, false, false };
		Piece BPawne = { BPawnE, (unsigned char)minFile + 4, (unsigned char)maxRank - 1, false, false };
		Piece BPawnf = { BPawnF, (unsigned char)minFile + 5, (unsigned char)maxRank - 1, false, false };
		Piece BPawng = { BPawnG, (unsigned char)minFile + 6, (unsigned char)maxRank - 1, false, false };
		Piece BPawnh = { BPawnH, (unsigned char)minFile + 7, (unsigned char)maxRank - 1, false, false };

		board[BPawna.file][BPawna.rank].pieceOnSquare = &BPawna;
		board[BPawnb.file][BPawnb.rank].pieceOnSquare = &BPawnb;
		board[BPawnc.file][BPawnc.rank].pieceOnSquare = &BPawnc;
		board[BPawnd.file][BPawnd.rank].pieceOnSquare = &BPawnd;
		board[BPawne.file][BPawne.rank].pieceOnSquare = &BPawne;
		board[BPawnf.file][BPawnf.rank].pieceOnSquare = &BPawnf;
		board[BPawng.file][BPawng.rank].pieceOnSquare = &BPawng;
		board[BPawnh.file][BPawnh.rank].pieceOnSquare = &BPawnh;
		
		//white rooks
		Piece WRooka = { WRookA, (unsigned char)minFile, (unsigned char)minRank, false, false };
		Piece WRookb = { WRookB, (unsigned char)maxFile, (unsigned char)minRank, false, false };

		board[WRooka.file][WRooka.rank].pieceOnSquare = &WRooka;
		board[WRookb.file][WRookb.rank].pieceOnSquare = &WRookb;

		//black rooks
		Piece BRooka = { BRookA, (unsigned char)minFile, (unsigned char)maxRank, false, false };
		Piece BRookb = { BRookB, (unsigned char)maxFile, (unsigned char)maxRank, false, false };

		board[BRooka.file][BRooka.rank].pieceOnSquare = &BRooka;
		board[BRookb.file][BRookb.rank].pieceOnSquare = &BRookb;

		//white bisops
		Piece WBishopa = { WBishopA, (unsigned char)2, (unsigned char)minRank, false, false };
		Piece WBishopb = { WBishopB, (unsigned char)maxFile - 2, (unsigned char)minRank, false, false };

		board[WBishopa.file][WBishopa.rank].pieceOnSquare = &WBishopa;
		board[WBishopb.file][WBishopb.rank].pieceOnSquare = &WBishopb;

		//black bishops
		Piece BBishopa = { BBishopA, (unsigned char)2, (unsigned char)maxRank, false, false };
		Piece BBishopb = { BBishopB, (unsigned char)maxFile - 2, (unsigned char)maxRank, false, false };

		board[BBishopa.file][BBishopa.rank].pieceOnSquare = &BBishopa;
		board[BBishopb.file][BBishopb.rank].pieceOnSquare = &BBishopb;

		//white knights
		Piece WKnighta = { WKnightA, (unsigned char)1, (unsigned char)minRank, false, false };
		Piece WKnightb = { WKnightB, (unsigned char)maxFile - 1, (unsigned char)minRank, false, false };

		board[WKnighta.file][WKnighta.rank].pieceOnSquare = &WKnighta;
		board[WKnightb.file][WKnightb.rank].pieceOnSquare = &WKnightb;

		//black kinghts
		Piece BKnighta = { BKnightA, (unsigned char)1, (unsigned char)maxRank, false, false };
		Piece BKnightb = { BKnightB, (unsigned char)maxFile - 1, (unsigned char)maxRank, false, false };

		board[BKnighta.file][BKnighta.rank].pieceOnSquare = &BKnighta;
		board[BKnightb.file][BKnightb.rank].pieceOnSquare = &BKnightb;

		//queens 
		Piece wQueen = { WQueen, (unsigned char)3, (unsigned char)minRank, false, false };
		Piece bQueen = { BQueen, (unsigned char)3, (unsigned char)maxRank, false, false };

		board[wQueen.file][wQueen.rank].pieceOnSquare = &wQueen;
		board[bQueen.file][bQueen.rank].pieceOnSquare = &bQueen;
		
		//kings
		Piece wKing = { WKing,(unsigned char) 4,  (unsigned char)minRank, false, false };
		Piece bKing = { BKing,(unsigned char)4,  (unsigned char)maxRank, false, false };
		board[wKing.file][wKing.rank].pieceOnSquare = &wKing;
		board[bKing.file][bKing.rank].pieceOnSquare = &bKing;
		///////////////////////////////////////////////////////////////////////////////////////////////////

	//create utility variables
	SDL_Event event;
	bool quit = false;
	bool whiteTurn = true;
	struct Square* clicked = NULL;
	struct Square* selected = NULL;


	while (!quit) {
		
		SDL_RenderClear(renderer);
		drawBoard2(renderer, BoardT);
		
		//draw all pieces
		/////////////////////////////////////////////////////////////////////////////////////
		
		//white pawns
		if (WPawna.taken == false)   drawItem(renderer, WPawnT,   WPawna.file,   WPawna.rank);
		if (WPawnb.taken == false)   drawItem(renderer, WPawnT, WPawnb.file, WPawnb.rank);
		if (WPawnc.taken == false)   drawItem(renderer, WPawnT, WPawnc.file, WPawnc.rank);
		if (WPawnd.taken == false)   drawItem(renderer, WPawnT, WPawnd.file, WPawnd.rank);
		if (WPawne.taken == false)   drawItem(renderer, WPawnT, WPawne.file, WPawne.rank);
		if (WPawnf.taken == false)   drawItem(renderer, WPawnT, WPawnf.file, WPawnf.rank);
		if (WPawng.taken == false)   drawItem(renderer, WPawnT, WPawng.file, WPawng.rank);
		if (WPawnh.taken == false)   drawItem(renderer, WPawnT, WPawnh.file, WPawnh.rank);

		//black pawns
		if (BPawna.taken == false)   drawItem(renderer, BPawnT,   BPawna.file,   BPawna.rank);
		if (BPawnb.taken == false)   drawItem(renderer, BPawnT, BPawnb.file, BPawnb.rank);
		if (BPawnc.taken == false)   drawItem(renderer, BPawnT, BPawnc.file, BPawnc.rank);
		if (BPawnd.taken == false)   drawItem(renderer, BPawnT, BPawnd.file, BPawnd.rank);
		if (BPawne.taken == false)   drawItem(renderer, BPawnT, BPawne.file, BPawne.rank);
		if (BPawnf.taken == false)   drawItem(renderer, BPawnT, BPawnf.file, BPawnf.rank);
		if (BPawng.taken == false)   drawItem(renderer, BPawnT, BPawng.file, BPawng.rank);
		if (BPawnh.taken == false)   drawItem(renderer, BPawnT, BPawnh.file, BPawnh.rank);
		
		//white rooks
		if (WRooka.taken == false)   drawItem(renderer, WRookT,   WRooka.file,   WRooka.rank);
		if (WRookb.taken == false)   drawItem(renderer, WRookT, WRookb.file, WRookb.rank);

		//black rooks
		if (BRooka.taken == false)   drawItem(renderer, BRookT,   BRooka.file,   BRooka.rank);
		if (BRookb.taken == false)   drawItem(renderer, BRookT, BRookb.file, BRookb.rank);

		//white bishops
		if (WBishopa.taken == false) drawItem(renderer, WBishopT, WBishopa.file, WBishopa.rank);
		if (WBishopb.taken == false) drawItem(renderer, WBishopT, WBishopb.file, WBishopb.rank);

		//black bishops
		if (BBishopa.taken == false) drawItem(renderer, BBishopT, BBishopa.file, BBishopa.rank);
		if (BBishopb.taken == false) drawItem(renderer, BBishopT, BBishopb.file, BBishopb.rank);

		//white knights
		if (WKnighta.taken == false) drawItem(renderer, WKnightT, WKnighta.file, WKnighta.rank);
		if (WKnightb.taken == false) drawItem(renderer, WKnightT, WKnightb.file, WKnightb.rank);

		//black knights
		if (BKnighta.taken == false) drawItem(renderer, BKnightT, BKnighta.file, BKnighta.rank);
		if (BKnightb.taken == false) drawItem(renderer, BKnightT, BKnightb.file, BKnightb.rank);

		//queens
		if (wQueen.taken == false) drawItem(renderer, WQueenT, wQueen.file, wQueen.rank);
		if (bQueen.taken == false) drawItem(renderer, BQueenT, bQueen.file, bQueen.rank);

		//kings
		if (wKing.taken == false) drawItem(renderer, WKingT, wKing.file, wKing.rank);
		if (bKing.taken == false) drawItem(renderer, BKingT, bKing.file, bKing.rank);

		////////////////////////////////////////////////////////////////////////////////////////
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
				if (selected == NULL /*&& (whiteTurn)*/) {
					//make clicked field selected only if it has a piece
					if (clicked->pieceOnSquare != NULL /*&& isWhite(clicked->pieceOnSquare)*/) {
						selected = clicked;
						calculateTargets(selected->pieceOnSquare, false);
					}
					//do nothing if clicked on empty field
				}
			
			/*	if (selected == NULL && (!whiteTurn)) {
					//make clicked field selected only if it has a piece
					if (clicked->pieceOnSquare != NULL && isBlack(clicked->pieceOnSquare)) {
						selected = clicked;
						calculateTargets(selected->pieceOnSquare, false);
					}
					//do nothing if clicked on empty field
				}
				*/

				else {
					if (clicked->target == true) {
						/*//swicth turns
						if (whiteTurn)
							whiteTurn = false;
						else
							whiteTurn = true;*/
						//make move
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


