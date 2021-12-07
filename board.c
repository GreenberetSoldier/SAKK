#include <SDl.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "board.h"

const char maxBlack = BPawnH;
const char minWhite = WPawnA;

void setSquareTarget(struct Square* sq, bool white, bool colorSpecific) {
	if (colorSpecific) {
		if (white)
			sq->targetOfWhite = true;
		else
			sq->targetOfBlack = true;
	}
	else {
		sq->target = true;
	}
}

void calculateTargets(Piece* piece, bool colorSpecific) {
	
	const int8_t rdirs[4][2] = { {1, 0},{0, 1},{-1, 0},{0, -1} }; // {fileoffset, rankoffset}
	const int8_t bdirs[4][2] = { {1, 1},{-1, 1},{-1, -1},{1, -1} }; // {fileoffset, rankoffset}
	const int8_t kndirs[8][2] = { {1, 2},{-1, 2},{2, 1},{2, -1},{1, -2},{-1, -2},{-2, 1},{-2, -1} }; // {fileoffset, rankoffset}
	const int8_t qdirs[8][2] = { {1, 0},{1, 1},{0, 1},{-1, 1},{-1, 0},{-1, -1},{0, -1},{1, -1} }; // {fileoffset, rankoffset}
	const int8_t kdirs[8][2] = { {-1,1},{1,0},{0,1}, {1,1},{0,-1},{-1,0},{-1,-1},{1,-1} };

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
					setSquareTarget(&board[piece->file][piece->rank + 1],true, colorSpecific);
					
				//take up left (self fw left, directors's up left)
				if (piece->file > minFile && colorSpecific ^ isEnemy(piece, board[piece->file - 1][piece->rank + 1].pieceOnSquare))
					setSquareTarget(&board[piece->file - 1][piece->rank + 1], true, colorSpecific);
					
				//take up right (self fw right, directors's up right)
				if (piece->file < maxFile && colorSpecific ^ isEnemy(piece, board[piece->file + 1][piece->rank + 1].pieceOnSquare))
					setSquareTarget(&board[piece->file + 1][piece->rank + 1], true, colorSpecific);
			}
			//unmoved, up: rank++++
			if (piece->movedYet == false && board[piece->file][piece->rank + 2].pieceOnSquare == NULL)
				setSquareTarget(&board[piece->file][piece->rank + 2], true, colorSpecific);
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
					setSquareTarget(&board[piece->file][piece->rank - 1], false, colorSpecific);

				//take down right (self fw left, director's down right)
				if (piece->file < maxFile && colorSpecific ^ isEnemy(piece, board[piece->file + 1][piece->rank - 1].pieceOnSquare))
					setSquareTarget(&board[piece->file + 1][piece->rank - 1], false, colorSpecific);

				//take down left (self fw right, directors's down left)
				if (piece->file > minFile && colorSpecific ^ isEnemy(piece, board[piece->file - 1][piece->rank - 1].pieceOnSquare))
					setSquareTarget(&board[piece->file - 1][piece->rank - 1], false, colorSpecific);

			}
			//unmoved, down: rank----
			if (piece->movedYet == false && board[piece->file][piece->rank - 2].pieceOnSquare == NULL)
				setSquareTarget(&board[piece->file][piece->rank - 2], false,  colorSpecific);
			break;
		
		case WRookA:
		case WRookB:
			for (int i = 0; i < 4; ++i)     // in each direction..
				for (int dist = 1; dist <= 7; ++dist) {  // ..seven is the maximum possible moving distance
					int8_t f = piece->file + rdirs[i][0] * dist;
					int8_t r = piece->rank + rdirs[i][1] * dist;
					if (f <= maxFile &&
						f >= minFile &&
						r <= maxRank &&
						r >= minRank) {
						if (board[f][r].pieceOnSquare == NULL || colorSpecific ^ isEnemy(piece, board[f][r].pieceOnSquare))
							setSquareTarget(&board[f][r], true,  colorSpecific);
						if (board[f][r].pieceOnSquare != NULL)
							break; //goto next direction
					}
				}
			break;

		case BRookA:
		case BRookB:
			for (int i = 0; i < 4; ++i)     // in each direction..
				for (int dist = 1; dist <= 7; ++dist) {  // ..seven is the maximum possible moving distance
					int8_t f = piece->file + rdirs[i][0] * dist;
					int8_t r = piece->rank + rdirs[i][1] * dist;
					if (f <= maxFile &&
						f >= minFile &&
						r <= maxRank &&
						r >= minRank) {
						if (board[f][r].pieceOnSquare == NULL || colorSpecific ^ isEnemy(piece, board[f][r].pieceOnSquare))
							setSquareTarget(&board[f][r], false,  colorSpecific);
						if (board[f][r].pieceOnSquare != NULL)
							break; //goto next direction
					}
				}
			break;

		case WBishopA:
		case WBishopB:
			for (int i = 0; i < 4; ++i)     // in each direction..
				for (int dist = 1; dist <= 7; ++dist) {  // ..seven is the maximum possible moving distance
					int8_t f = piece->file + bdirs[i][0] * dist;
					int8_t r = piece->rank + bdirs[i][1] * dist;
					if (f <= maxFile &&
						f >= minFile &&
						r <= maxRank &&
						r >= minRank) {
						if (board[f][r].pieceOnSquare == NULL || colorSpecific ^ isEnemy(piece, board[f][r].pieceOnSquare))
							setSquareTarget(&board[f][r], true,  colorSpecific);
						if (board[f][r].pieceOnSquare != NULL)
							break; //goto next direction
					}
				}
			break;

		case BBishopA:
		case BBishopB:
			for (int i = 0; i < 4; ++i)     // in each direction..
				for (int dist = 1; dist <= 7; ++dist) {  // ..seven is the maximum possible moving distance
					int8_t f = piece->file + bdirs[i][0] * dist;
					int8_t r = piece->rank + bdirs[i][1] * dist;
					if (f <= maxFile &&
						f >= minFile &&
						r <= maxRank &&
						r >= minRank) {
						if (board[f][r].pieceOnSquare == NULL || colorSpecific ^ isEnemy(piece, board[f][r].pieceOnSquare))
							setSquareTarget(&board[f][r], false,  colorSpecific);
						if (board[f][r].pieceOnSquare != NULL)
							break; //goto next direction
					}
				}
			break;

		case WKnightA:
		case WKnightB:
			// in case of knight, the maximum moving distance in each direction is 1
			for (int i = 0; i < 8; ++i) {
				int8_t f = piece->file + kndirs[i][0];
				int8_t r = piece->rank + kndirs[i][1];
				if (f <= maxFile &&
					f >= minFile &&
					r <= maxRank &&
					r >= minRank)
					if (board[f][r].pieceOnSquare == NULL || (colorSpecific ^ isEnemy(piece, board[f][r].pieceOnSquare)))
						setSquareTarget(&board[f][r], true,  colorSpecific);
			}
			break;

		case BKnightA:
		case BKnightB:
			for (int i = 0; i < 8; ++i) {
				int8_t f = piece->file + kndirs[i][0];
				int8_t r = piece->rank + kndirs[i][1];
				if (f <= maxFile &&
					f >= minFile &&
					r <= maxRank &&
					r >= minRank)
					if (board[f][r].pieceOnSquare == NULL || (colorSpecific ^ isEnemy(piece, board[f][r].pieceOnSquare)))
					setSquareTarget(&board[f][r], false,  colorSpecific);
			}
			break;

		case WQueen:
			for (int i = 0; i < 8; ++i)     // in each direction..
				for (int dist = 1; dist <= 7; ++dist) {  // ..seven is the maximum possible moving distance
					int8_t f = piece->file + qdirs[i][0] * dist;
					int8_t r = piece->rank + qdirs[i][1] * dist;
					if (f <= maxFile &&
						f >= minFile &&
						r <= maxRank &&
						r >= minRank) {
						if (board[f][r].pieceOnSquare == NULL || colorSpecific ^ isEnemy(piece, board[f][r].pieceOnSquare))
							setSquareTarget(&board[f][r], true,  colorSpecific);
						if (board[f][r].pieceOnSquare != NULL)
							break; //goto next direction
					}
				}
			break;

		case BQueen:
			for (int i = 0; i < 8; ++i)     // in each direction..
				for (int dist = 1; dist <= 7; ++dist){  // ..seven is the maximum possible moving distance
					int8_t f = piece->file + qdirs[i][0] * dist;
					int8_t r = piece->rank + qdirs[i][1] * dist;
					if (f <= maxFile &&
						f >= minFile &&
						r <= maxRank &&
						r >= minRank) {
						if (board[f][r].pieceOnSquare == NULL || colorSpecific ^ colorSpecific ^ isEnemy(piece, board[f][r].pieceOnSquare))
							setSquareTarget(&board[f][r], false,  colorSpecific);
						if (board[f][r].pieceOnSquare != NULL)
							break; //goto next direction
					}
				}
			break;

		case WKing:
			for (int8_t d = 0; d < 8; d++) {

				int8_t f = piece->file + kdirs[d][0];
				int8_t r = piece->rank + kdirs[d][1];

				if ((f <= maxFile)
					&& (f >= minFile)
					&& (r <= maxFile)
					&& (r >= minRank)) {
					//check if squares are empty, have enemy or are attacked
					if (board[f][r].targetOfBlack == false && (board[f][r].pieceOnSquare == NULL || colorSpecific ^ isEnemy(piece, board[f][r].pieceOnSquare))) {
						setSquareTarget(&board[f][r], true,  colorSpecific);
					}
				}
			}

			//long castle
			//both pieces havent moved yet
			//if (piece->movedYet == false && piece.);

			/*//check if the squares vbetween are targets and empty
			bool isItSafe = true;
			for (int8_t i = 1; 1 <= 3; ++i) {
				if (board[piece->file - i][piece->rank].target == true && board[piece->file - i][piece->rank].pieceOnSquare != NULL) {
					isItSafe = false;
				}
			}

			//if both havent moved and its safe
			if (isItSafe) {
				board[piece->file - 2][piece->rank].target = true;
			}
			*/
			break;

		case BKing:
			//normal movement
			//for each element of direction array
			for (int8_t d = 0; d < 8; d++) {
				
				int8_t f = piece->file + kdirs[d][0];
				int8_t r = piece->rank + kdirs[d][1];
				
				if ((f <= maxFile)
					&& (f >= minFile)
					&& (r <= maxFile)
					&& (r >= minRank)){
					//check if squares are empty, have enemy or are attacked
					if (board[f][r].targetOfWhite == false && (board[f][r].pieceOnSquare == NULL || colorSpecific ^ isEnemy(piece, board[f][r].pieceOnSquare))) {
						setSquareTarget(&board[f][r], true, colorSpecific);
					}
				}
			}

			/*
			//long castle 
			//both pieces havent moved yet 
			if (piece->movedYet == false && board[piece->file - 4][piece->rank].pieceOnSquare->movedYet == false) {
				
				//get all target squares 
				for (int8_t f = 0, r = 0; f < 8 && r < 8; ++f, ++r) {
					if (board[f][r].pieceOnSquare != NULL) {
						calculateTargets(board,board[f][r].pieceOnSquare); //talan ez a bug
					}
				}
				
				//check if the squares vbetween are targets and empty
				bool isItSafe = true;
				for (int8_t i = 1; 1 <= 3; ++i) {
					if (board[piece->file - i][piece->rank].target == true && board[piece->file - i][piece->rank].pieceOnSquare != NULL) {
						isItSafe = false;
					}
				}

				//if both havent moved and its safe
				if (isItSafe) {
					board[piece->file - 2][piece->rank].target = true;
				}
			}*/
			break;
		default:
				break;
		}
	}



void calculateAllColorSpecificTargets() {
	// clear current color specific targets
	for (int8_t r = minRank; r <= maxRank; ++r)
		for (int8_t f = minFile; f <= maxFile; ++f) {
			board[f][r].targetOfBlack = false;
			board[f][r].targetOfWhite = false;
	}
	
	// recalculate them
	for (int8_t r = minRank; r <= maxRank; ++r)
		for (int8_t f = minFile; f <= maxFile; ++f)
			if (board[f][r].pieceOnSquare != NULL)
				calculateTargets(board[f][r].pieceOnSquare, true);
}



void detargetAll() {
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

	// a jelenlegi adatmodell�nkben egy mez�b�l nem tudhatjuk annak koordin�t�it!...
	// (ez eredetileg benne volt, �s �n vettem ki.. - G�bor)
	// ez�rt a l�p�shez sz�ks�g van a c�l koordin�t�ira is k�l�n, a c�l mez� �nmag�ban nem el�g
	from->pieceOnSquare->file = toFile;
	from->pieceOnSquare->rank = toRank;
	to->pieceOnSquare = from->pieceOnSquare;
	from->pieceOnSquare = NULL;

	to->pieceOnSquare->movedYet = true;

	calculateAllColorSpecificTargets();

	//newMove();
}


newMove() {

	//head->p = ;
	//head->fdelta = ;
	//head->rdelta = ;
	//head->taken = ;
	//head->promoted = ;


	head->next = (struct Move*) malloc(sizeof (struct Move));
	head = head->next;
}