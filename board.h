#pragma once
#include <stdbool.h>
#include <SDL.h>

extern int windowWidht;
extern int windowHeight;
extern int squareSize;
extern int pieceWidht;
extern int pieceHeight;

typedef enum PieceType {
	WRookA, WKinghtA, WBishopW, WQueen, WKing, WBishopB, WKnightB, WRookH, WPawnA, WPawnB, WPawnC, WPawnD, WPawnE, WPawnF, WPawnG, WPawnH,
	BRookA, BKinghtA, BBishopW, BQueen, BKing, BBishopB, BKnightB, BRookH, BPawnA, BPawnB, BPawnC, BPawnD, BPawnE, BPawnF, BPawnG, BPawnH,
	empty = -1
} PieceType;

typedef struct Piece {
	PieceType type;
	int rank, file;
	bool movedYet;
} Piece;

typedef enum Fullness {
	full, clear
} Fullness;

typedef struct Color {
	int r, g, b, a;
} Color;

typedef struct Square {
	Piece pieceOnSquare;
	Color color;
	Fullness fullness;
	int rank, file;
	int x1, x2, y1, y2;
} Square;

void slideSquare(Square* square);

void slideSquareColumn(Square* square);

void squareChangeColour(Square* square);

void squareResetX(Square* square);

void drawSqaure(Square* square, SDL_Renderer* renderer);

void drawFile(Square* square, SDL_Renderer* renderer);

void resetSquare(Square* square);

void drawBoard(Square* square, SDL_Renderer* renderer);

void initializeBoardValues(Square*** board);
