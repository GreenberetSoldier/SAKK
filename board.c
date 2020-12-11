#include <SDl.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "board.h"


char maxBlack = BPawnH;
char minWhite = WPawnA;


void calculateValidTargets(board_t board, Piece* piece) {

	uint8_t i;

	switch (piece->type) {
		case WPawnA:
		case WPawnB:
		case WPawnC:
		case WPawnD:
		case WPawnE:
		case WPawnF:
		case WPawnG:
		case WPawnH:
			//up: rank++
			if (piece->rank < maxRank) {
				//up (self fw, director's up)
				if (board[piece->file][piece->rank + 1].pieceOnSquare == NULL)
					board[piece->file][piece->rank + 1].target = true;
				//take up left (self fw left, directors's up left)
				if (piece->file > minFile && isEnemy(piece, board[piece->file - 1][piece->rank + 1].pieceOnSquare))
					board[piece->file - 1][piece->rank + 1].target = true;
				//take up right (self fw right, directors's up right)
				if (piece->file < maxFile && isEnemy(piece, board[piece->file + 1][piece->rank + 1].pieceOnSquare))
					board[piece->file + 1][piece->rank + 1].target = true;
			}
			//unmoved, up: rank++++
			if (piece->movedYet == false && board[piece->file][piece->rank + 2].pieceOnSquare == NULL)
				board[piece->file][piece->rank + 2].target = true;
			break;
		case BPawnA:
		case BPawnB:
		case BPawnC:
		case BPawnD:
		case BPawnE:
		case BPawnF:
		case BPawnG:
		case BPawnH:
			//down: rank--
			if (piece->rank > minRank) {
				//down (self fw, director's down)
				if (board[piece->file][piece->rank - 1].pieceOnSquare == NULL)
					board[piece->file][piece->rank - 1].target = true;
				//take down right (self fw left, director's down right)
				if (piece->file < maxFile && isEnemy(piece, board[piece->file + 1][piece->rank - 1].pieceOnSquare))
					board[piece->file + 1][piece->rank - 1].target = true;
				//take down left (self fw right, directors's down left)
				if (piece->file > minFile && isEnemy(piece, board[piece->file - 1][piece->rank - 1].pieceOnSquare))
					board[piece->file - 1][piece->rank - 1].target = true;
			}
			//unmoved, down: rank----
			if (piece->movedYet == false && board[piece->file][piece->rank - 2].pieceOnSquare == NULL)
				board[piece->file][piece->rank - 2].target = true;
			
			//TODO: enpass 
			break;
		
		case WRookA:
		case WRookB:
		case BRookA:
		case BRookB:
			//up: ++rank
			if (piece->rank < maxRank)
				for (int8_t i = piece->rank + 1; i <= maxRank ; i++) {
					if (board[piece->file][i].pieceOnSquare == NULL)
						board[piece->file][i].target = true;
					else {
						if (isEnemy(piece, board[piece->file][i].pieceOnSquare))
							board[piece->file][i].target = true;
						break;
					}
				}
			//down: --rank
			if (piece->rank > minRank)
				for (int8_t i = piece->rank - 1; i >= minRank; i--) {
					if (board[piece->file][i].pieceOnSquare == NULL)
						board[piece->file][i].target = true;
					else {
						if (isEnemy(piece, board[piece->file][i].pieceOnSquare))
							board[piece->file][i].target = true;
						break;
					}
				}
			//right: ++file
			if (piece->file < maxFile)
				for (int8_t i = piece->file + 1; i <= maxFile; i++) {
					if (board[i][piece->rank].pieceOnSquare == NULL)
						board[i][piece->rank].target = true;
					else {
						if (isEnemy(piece, board[piece->file][i].pieceOnSquare))
							board[piece->file][i].target = true;
						break;
					}
				}
			//left: --file
			if (piece->file > minFile)
				for (int8_t i = piece->file - 1; i >= minFile; i--) {
					if (board[i][piece->rank].pieceOnSquare == NULL)
						board[i][piece->rank].target = true;
					else {
						if (isEnemy(piece, board[piece->file][i].pieceOnSquare))
							board[piece->file][i].target = true;
						break;
					}
				}
			break;
		
		case WBishopA:
		case WBishopB:
		case BBishopA:
		case BBishopB:
			//upright: ++rank ++file
			if (piece->rank < maxRank && piece->file < maxFile)
				for (int8_t i = piece->file + 1; i <= maxFile; i++)
				for (int8_t j = piece->rank + 1; j <= maxRank; j++) {
					if (board[i][j].pieceOnSquare == NULL)
						board[i][j].target = true;
					else {
						if (isEnemy(piece, board[i][j].pieceOnSquare))
							board[i][j].target = true;
						break;
					}
				}
			//upleft: ++rank --file
			if (piece->rank < maxRank && piece->file > minFile)
				for (int8_t i = piece->file - 1, j = piece->rank + 1; i >= minFile && j <= maxRank; i--, j++)
				{
					if (board[i][j].pieceOnSquare == NULL)
						board[i][j].target = true;
					else {
						if (isEnemy(piece, board[i][j].pieceOnSquare))
							board[i][j].target = true;
						break;
					}
				}

		
		default:
			break;
	}

}


void detargetAll(board_t board) {
	for (int file = minFile; file <= maxFile; file++)
		for (int rank = minRank; rank <= maxRank; rank++)
			board[file][rank].target = false;
}

//returns true  if that is an enemy
//returns false if that is an ally
//returns false if either parameter is NULL
bool isEnemy(Piece* me, Piece* that) {
	if (me == NULL || that == NULL)
		return false;

	if (isWhite(me))
		return isBlack(that);
	else
		return isWhite(that);
}

bool isWhite(Piece* piece) {
	if (piece == NULL)
		return false;

	return piece->type >= minWhite;
}

bool isBlack(Piece* piece) {
	if (piece == NULL)
		return false;

	return piece->type <= maxBlack;
}

void performMove(struct Square* from, struct Square* to, unsigned char toFile, unsigned char toRank) {
	if (from == NULL || to == NULL)
		exit(-1);
	
	if (to->pieceOnSquare != NULL)
		to->pieceOnSquare->taken = true;

	from->pieceOnSquare->file = toFile;
	from->pieceOnSquare->rank = toRank;
	to->pieceOnSquare = from->pieceOnSquare;
	from->pieceOnSquare = NULL;

	to->pieceOnSquare->movedYet = true;
}
