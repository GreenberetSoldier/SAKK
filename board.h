#pragma once

#ifndef BOARD_H
#define BOARD_H


#include <stdbool.h>
#include <SDL.h>

const unsigned int windowWidht  = 1280;
const unsigned int windowHeight = 720;
const unsigned int squareSize   = 90;
const unsigned int pieceWidht   = 285;
const unsigned int pieceHeight  = 340;


#define filenum 8
#define ranknum 8

enum PieceType {
	WRookA, WKinghtA, WBishopW, WQueen, WKing, WBishopB, WKnightB, WRookH, WPawnA, WPawnB, WPawnC, WPawnD, WPawnE, WPawnF, WPawnG, WPawnH,
	BRookA, BKinghtA, BBishopW, BQueen, BKing, BBishopB, BKnightB, BRookH, BPawnA, BPawnB, BPawnC, BPawnD, BPawnE, BPawnF, BPawnG, BPawnH
};

typedef struct Piece {
	enum PieceType type;
	unsigned char file;
	unsigned char rank;
	bool movedYet;
	bool taken;
} Piece;

/*typedef enum Fullness {
	full, clear
} Fullness;*/

typedef struct Color {
	int r, g, b, a;
} Color;

struct Square {
	Piece* pieceOnSquare;
	//Color color;
	//Fullness fullness;
	//int rank, file;
	//int x1, x2, y1, y2;
	bool selected;
	bool target;
};


//order:                        [file][rank]
typedef struct Square board_t[filenum][ranknum];


void deselectAll(board_t* board);

void detargetAll(board_t* board);

void calculateTargets();


#endif // BOARD_H