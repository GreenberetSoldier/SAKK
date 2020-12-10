#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "board.h"
#include "pawn.h"

void movePawnForwards(Square*** board, Piece* pawn) {
	board[pawn->rank][pawn->file]->pieceOnSquare.type = empty;
	board[pawn->rank][pawn->file+1]->pieceOnSquare.type = pawn->type;
	pawn->file++;
	pawn->movedYet = true;
}

void movePawnForwards2(Square*** board, Piece* pawn) {
	board[pawn->rank][pawn->file]->pieceOnSquare.type = empty;
	board[pawn->rank][pawn->file + 2]->pieceOnSquare.type = pawn->type;
	pawn->file += 2;
	pawn->movedYet = true;
}

int canWPawnTake(Square*** board, Piece* pawn) {
	if (board[(pawn->rank) + 1][(pawn->file) + 1]->pieceOnSquare.type > 15)
		return 0;
	else if (board[(pawn->rank) + 1][(pawn->file) - 1]->pieceOnSquare.type > 15)
		return 1;
	else
		return -1;
}

int canBPawnTake(Square*** board, Piece* pawn) {
	if (board[pawn->rank + 1][pawn->file + 1]->pieceOnSquare.type < 15 && board[pawn->rank + 1][pawn->file + 1]->pieceOnSquare.type != -1)
		return 0;
	else if (board[pawn->rank + 1][pawn->file - 1]->pieceOnSquare.type < 15 && board[pawn->rank + 1][pawn->file + 1]->pieceOnSquare.type != -1)
		return 1;
	else
		return -1;
}

void takePiece(Square* board[8][8], Piece* piece) {
	board[piece->rank][piece->file]->pieceOnSquare = *piece;
}

void pawnTakesLeft(Square* board[8][8], Piece* pawn) {
	takePiece(board, &board[pawn->rank - 1][pawn->file + 1]->pieceOnSquare);
	board[pawn->rank][pawn->file]->pieceOnSquare.type = empty;
	board[pawn->rank - 1][pawn->file + 1]->pieceOnSquare.type = pawn->type;
	pawn->rank--;
	pawn->file++;
}



void pawnTakesRight(Square* board[8][8], Piece* pawn) {
	/*for (int k = 0; k < 8; k++) {
		for (int i = 0; i < 8; i++) {
			board[k][i] = board[k][i];
		}
	}*/

	takePiece(board, &board[pawn->rank + 1][pawn->file + 1]->pieceOnSquare);
	board[pawn->rank][pawn->file]->pieceOnSquare.type = empty;
	board[pawn->rank + 1][pawn->file + 1]->pieceOnSquare.type = pawn->type;
	pawn->rank++;
	pawn->file++;
}

void wPawnPromotion(Square*** board, Piece* pawn) {
	board[pawn->rank][pawn->file]->pieceOnSquare.type = WQueen;
	pawn->type = WQueen;
}

void bPawnPromotion(Square*** board, Piece* pawn) {
	board[pawn->rank][pawn->file]->pieceOnSquare.type = BQueen;
	pawn->type = BQueen;
 }