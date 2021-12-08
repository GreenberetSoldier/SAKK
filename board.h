#pragma once

#ifndef BOARD_H
#define BOARD_H


#include <stdbool.h>
#include <SDL.h>

#define windowWidht  720
#define windowHeight 720
#define squareSize   90
#define pieceWidht   285
#define pieceHeight  340


#define filenum 8
#define ranknum 8

#define maxFile 7
#define minFile 0
#define maxRank 7
#define minRank 0

enum PieceType {
	BRookA, BKnightA, BBishopA, BQueen,  BKing, BBishopB, BKnightB, BRookB,
	BPawnA,   BPawnB,   BPawnC, BPawnD, BPawnE,   BPawnF,   BPawnG, BPawnH,

	WPawnA,   WPawnB,   WPawnC, WPawnD, WPawnE,   WPawnF,   WPawnG, WPawnH,
	WRookA, WKnightA, WBishopA, WQueen,  WKing, WBishopB, WKnightB, WRookB
};

extern const char maxBlack;
extern const char minWhite;

typedef struct Piece {
	enum PieceType type;
	uint8_t file;
	uint8_t rank;
	bool movedYet;
	bool taken;
} Piece;

typedef struct Color {
	int r, g, b, a;
} Color;

struct Square {
	Piece* pieceOnSquare;
	bool target;
	bool targetOfWhite;
	bool targetOfBlack;
};

//order: [file][rank]
typedef struct Square board_t[filenum][ranknum];

// the contents of struct move represent what happened in it
typedef struct Move {
	struct Move* next;          // NULL only if this is the last move (head)
	struct Move* prev;          // NULL only if this is the first move (tail)
	Piece* movedPiece;			// piece moved								(compulsory) 
	uint8_t fdelta;				// delta file								(compulsory) 
	uint8_t rdelta;				// delta rank								(compulsory) 
	Piece* takenPiece;			// piece taken (or vanished by promotion)	(optional) 
	Piece* promoted;			// piece appeared by promotion				(optional) 
} Move ;

//GLOBAL VARIABLES
board_t board;
Move* head = NULL;		// the latest move
Move* tail = NULL;		// the first move (lehet hogy nem is fog kelleni)
Move* current = NULL;	// pointer for walking on the list of moves
 
// FUNCTION DECLARATIONS 
void detargetAll(); 
void performMove(struct Square* from, struct Square* to, unsigned char toFile, unsigned char toRank); 
 
void calculateTargets(Piece* piece, bool colorSpecific); 
void calculateAllColorSpecificTargets();

bool isWhite(Piece* piece);
bool isBlack(Piece* piece);
bool isEnemy(Piece* me, Piece* that);


#endif // BOARD_H