#pragma once

void movePawnForwards(Square*** board, Piece* pawn);

void movePawnForwards2(Square*** board, Piece* pawn);

int canWPawnTake(Square*** board, Piece* pawn);

int canBPawnTake(Square*** board, Piece* pawn);

void takePiece(Square*** board, Piece* piece);

void pawnTakesLeft(Square*** board, Piece* pawn);

void pawnTakesRight(Square*** board, Piece* pawn);

void wPawnPromotion(Square*** board, Piece* pawn);

void bPawnPromotion(Square*** board, Piece* pawn);
