#include <SDl.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "board.h"

int windowWidht = 1280;
int windowHeight = 720;
int squareSize = 90;
int pieceWidht = 285;
int pieceHeight = 340;

void slideSquare(Square* square) {
	square->x1 += squareSize;
	square->x2 += squareSize;
}

void slideSquareColumn(Square* square) {
	square->y1 += squareSize;
	square->y2 += squareSize;
}

void squareChangeColour(Square* square) {
	if (square->fullness == clear) {
		square->fullness = full;
		square->color.r = 192;
		square->color.g = 152;
		square->color.b = 82;
		square->color.a = 255;
	}

	else {
		square->fullness = clear;
		square->color.r = 75;
		square->color.g = 152;
		square->color.b = 82;
		square->color.a = 77;
	}

}

void squareResetX(Square* square) {
	square->x1 = squareSize;
	square->x2 = 0;
}

void drawSqaure(Square* square, SDL_Renderer* renderer) {
	if (square->fullness == clear) {
		boxRGBA(renderer, square->x1, square->y1, square->x2, square->y2, square->color.r, square->color.g, square->color.b, square->color.a);
		slideSquare(square);
		squareChangeColour(square);
	}
	else {
		boxRGBA(renderer, square->x1, square->y1, square->x2, square->y2, square->color.r, square->color.g, square->color.b, square->color.a);
		slideSquare(square);
		squareChangeColour(square);
	}

}

void drawFile(Square* square, SDL_Renderer* renderer) {
	for (int sor = 0; sor < 8; sor++)
		drawSqaure(square, renderer);
}

void resetSquare(Square* square) {
	square->fullness = full;
	square->x1 = squareSize;
	square->x2 = 0;
	square->y1 = squareSize;
	square->y2 = 0;
}

void drawBoard(Square* square, SDL_Renderer* renderer) {
	for (int rank = 0; rank < 8; rank++) {
		squareChangeColour(square);
		drawFile(square, renderer);
		squareResetX(square);
		slideSquareColumn(square);
	}
	resetSquare(square);
}

void initializeBoardValues(Square*** board) {
	
}