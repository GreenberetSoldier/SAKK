#include <SDl.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "board.h"

const char maxBlack = BPawnH;
const char minWhite = WPawnA;

void calculateValidTargets(board_t board, Piece* piece) {

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
				for (int8_t r = piece->rank + 1; r <= maxRank; r++) {
					if (board[piece->file][r].pieceOnSquare == NULL)
						board[piece->file][r].target = true;
					else {
						if (isEnemy(piece, board[piece->file][r].pieceOnSquare))
							board[piece->file][r].target = true;
						break;
					}
				}
			//down: --rank
			if (piece->rank > minRank)
				for (int8_t r = piece->rank - 1; r >= minRank; r--) {
					if (board[piece->file][r].pieceOnSquare == NULL)
						board[piece->file][r].target = true;
					else {
						if (isEnemy(piece, board[piece->file][r].pieceOnSquare))
							board[piece->file][r].target = true;
						break;
					}
				}
			//right: ++file
			if (piece->file < maxFile)
				for (int8_t f = piece->file + 1; f <= maxFile; f++) {
					if (board[f][piece->rank].pieceOnSquare == NULL)
						board[f][piece->rank].target = true;
					else {
						if (isEnemy(piece, board[f][piece->rank].pieceOnSquare))
							board[f][piece->rank].target = true;
						break;
					}
				}
			//left: --file
			if (piece->file > minFile)
				for (int8_t f = piece->file - 1; f >= minFile; f--) {
					if (board[f][piece->rank].pieceOnSquare == NULL)
						board[f][piece->rank].target = true;
					else {
						if (isEnemy(piece, board[f][piece->rank].pieceOnSquare))
							board[f][piece->rank].target = true;
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
				for (int8_t f = piece->file + 1, r = piece->rank + 1; r <= maxRank && f <= maxFile; f++, r++)
					if (board[f][r].pieceOnSquare == NULL)
						board[f][r].target = true;
					else {
						if (isEnemy(piece, board[f][r].pieceOnSquare))
							board[f][r].target = true;
						break; //for
					}

			//upleft: ++rank --file
			if (piece->rank < maxRank && piece->file > minFile)
				for (int8_t f = piece->file - 1, r = piece->rank + 1; f >= minFile && r <= maxRank; f--, r++)
					if (board[f][r].pieceOnSquare == NULL)
						board[f][r].target = true;
					else {
						if (isEnemy(piece, board[f][r].pieceOnSquare))
							board[f][r].target = true;
						break; //for
					}

			//downleft: --rank --file
			if (piece->rank > minRank && piece->file > minFile)
				for (int8_t f = piece->file - 1, r = piece->rank - 1; f >= minFile && r >= minRank; f--, r--)
					if (board[f][r].pieceOnSquare == NULL)
						board[f][r].target = true;
					else {
						if (isEnemy(piece, board[f][r].pieceOnSquare))
							board[f][r].target = true;
						break; //for
					}

			//downright: --rank ++file
			if (piece->rank > minRank && piece->file < maxFile)
				for (int8_t f = piece->file + 1, r = piece->rank - 1; f <= maxFile && r >= minRank; f++, r--)
					if (board[f][r].pieceOnSquare == NULL)
						board[f][r].target = true;
					else {
						if (isEnemy(piece, board[f][r].pieceOnSquare))
							board[f][r].target = true;
						break; //for
					}
			break;

		case WKnightA:
		case WKnightB:
		case BKnightA:
		case BKnightB:

			//upright file+ rank++
			if (piece->file < maxFile && piece->rank < maxRank - 1) {
				if (board[piece->file + 1][piece->rank + 2].pieceOnSquare == NULL || (isEnemy(piece, board[piece->file + 1][piece->rank + 2].pieceOnSquare))) {
					board[piece->file + 1][piece->rank + 2].target = true;
				}
			}
				
			//upleft file- rank++
			if (piece->file > minFile && piece->rank < maxRank - 1) {
				if (board[piece->file - 1][piece->rank + 2].pieceOnSquare == NULL || (isEnemy(piece, board[piece->file - 1][piece->rank + 2].pieceOnSquare))) {
					board[piece->file - 1][piece->rank + 2].target = true;
				}
			}

			//rightup file++ rank+
			if (piece->file < maxFile - 1 && piece->rank < maxRank) {
				if (board[piece->file + 2][piece->rank + 1].pieceOnSquare == NULL || (isEnemy(piece, board[piece->file + 2][piece->rank + 1].pieceOnSquare))) {
					board[piece->file + 2][piece->rank + 1].target = true;
				}
			}

			//rightdown file++ rank-
			if (piece->file < maxFile && piece->rank > minRank) {
				if (board[piece->file + 2][piece->rank - 1].pieceOnSquare == NULL || (isEnemy(piece, board[piece->file + 2][piece->rank - 1].pieceOnSquare))) {
					board[piece->file + 2][piece->rank - 1].target = true;
				}
			}

			//downright file+ rank--
			if (piece->file < maxFile && piece->rank > minRank + 1) {
				if (board[piece->file + 1][piece->rank - 2].pieceOnSquare == NULL || (isEnemy(piece, board[piece->file + 1][piece->rank - 2].pieceOnSquare))) {
					board[piece->file + 1][piece->rank - 2].target = true;
				}
			}

			//downleft file- rank--
			if (piece->file > minFile && piece->rank > minRank + 1) {
				if (board[piece->file - 1][piece->rank - 2].pieceOnSquare == NULL || (isEnemy(piece, board[piece->file - 1][piece->rank - 2].pieceOnSquare))) {
					board[piece->file - 1][piece->rank - 2].target = true;
				}
			}
			
			//leftup file-- rank+
			if (piece->file > minFile + 1 && piece->rank < maxRank) {
				if (board[piece->file - 2][piece->rank + 1].pieceOnSquare == NULL || (isEnemy(piece, board[piece->file - 2][piece->rank + 1].pieceOnSquare))) {
					board[piece->file - 2][piece->rank + 1].target = true;
				}
			}
		
			//leftdown file-- rank-
			if (piece->file > minFile + 1 && piece->rank > minRank) {
				if (board[piece->file - 2][piece->rank - 1].pieceOnSquare == NULL || (isEnemy(piece, board[piece->file - 2][piece->rank - 1].pieceOnSquare))) {
					board[piece->file - 2][piece->rank - 1].target = true;
				}
			}
			
			break;
		case WQueen:
		case BQueen:
			//upright: ++rank ++file
			if (piece->rank < maxRank && piece->file < maxFile)
				for (int8_t f = piece->file + 1, r = piece->rank + 1; r <= maxRank && f <= maxFile; f++, r++)
					if (board[f][r].pieceOnSquare == NULL)
						board[f][r].target = true;
					else {
						if (isEnemy(piece, board[f][r].pieceOnSquare))
							board[f][r].target = true;
						break; //for
					}

			//upleft: ++rank --file
			if (piece->rank < maxRank && piece->file > minFile)
				for (int8_t f = piece->file - 1, r = piece->rank + 1; f >= minFile && r <= maxRank; f--, r++)
					if (board[f][r].pieceOnSquare == NULL)
						board[f][r].target = true;
					else {
						if (isEnemy(piece, board[f][r].pieceOnSquare))
							board[f][r].target = true;
						break; //for
					}

			//downleft: --rank --file
			if (piece->rank > minRank && piece->file > minFile)
				for (int8_t f = piece->file - 1, r = piece->rank - 1; f >= minFile && r >= minRank; f--, r--)
					if (board[f][r].pieceOnSquare == NULL)
						board[f][r].target = true;
					else {
						if (isEnemy(piece, board[f][r].pieceOnSquare))
							board[f][r].target = true;
						break; //for
					}

			//downright: --rank ++file
			if (piece->rank > minRank && piece->file < maxFile)
				for (int8_t f = piece->file + 1, r = piece->rank - 1; f <= maxFile && r >= minRank; f++, r--)
					if (board[f][r].pieceOnSquare == NULL)
						board[f][r].target = true;
					else {
						if (isEnemy(piece, board[f][r].pieceOnSquare))
							board[f][r].target = true;
						break; //for
					}

			//up: ++rank
			if (piece->rank < maxRank)
				for (int8_t r = piece->rank + 1; r <= maxRank; r++) {
					if (board[piece->file][r].pieceOnSquare == NULL)
						board[piece->file][r].target = true;
					else {
						if (isEnemy(piece, board[piece->file][r].pieceOnSquare))
							board[piece->file][r].target = true;
						break;
					}
				}
			//down: --rank
			if (piece->rank > minRank)
				for (int8_t r = piece->rank - 1; r >= minRank; r--) {
					if (board[piece->file][r].pieceOnSquare == NULL)
						board[piece->file][r].target = true;
					else {
						if (isEnemy(piece, board[piece->file][r].pieceOnSquare))
							board[piece->file][r].target = true;
						break;
					}
				}
			//right: ++file
			if (piece->file < maxFile)
				for (int8_t f = piece->file + 1; f <= maxFile; f++) {
					if (board[f][piece->rank].pieceOnSquare == NULL)
						board[f][piece->rank].target = true;
					else {
						if (isEnemy(piece, board[f][piece->rank].pieceOnSquare))
							board[f][piece->rank].target = true;
						break;
					}
				}
			//left: --file
			if (piece->file > minFile)
				for (int8_t f = piece->file - 1; f >= minFile; f--) {
					if (board[f][piece->rank].pieceOnSquare == NULL)
						board[f][piece->rank].target = true;
					else {
						if (isEnemy(piece, board[f][piece->rank].pieceOnSquare))
							board[f][piece->rank].target = true;
						break;
					}
				}
			break;

		case WKing:
		case BKing:
			
			
			//normal movement
			int kingDirectionArray[8][2] = { {1,-1},{1,0},{0,1}, {1,1},{0,-1},{-1,0},{-1,-1},{1,-1} };
			//for each element of direction array
			for (int8_t d = 0; d < 8; ++d) {
				if ((piece->file + kingDirectionArray[d][0] < 8)
					&& (piece->file + kingDirectionArray[d][0] > 0)
					&& (piece->rank + kingDirectionArray[d][1] < 8)
					&& (piece->rank + kingDirectionArray[d][1] > 0)){
						board[piece->file + piece->file + kingDirectionArray[d][0]][piece->rank + kingDirectionArray[d][1]].target = true;
				}

				
			}


			
			//long castle 
			//both pieces havent moved yet 
			if (piece->movedYet == false && board[piece->file - 4][piece->rank].pieceOnSquare->movedYet == false) {
				
				//get all target squares 
				for (int8_t f = 0, r = 0; f < 8 && r < 8; ++f, ++r) {
					if (board[f][r].pieceOnSquare != NULL) {
						calculateValidTargets(board,board[f][r].pieceOnSquare); //talan ez a bug
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
			}
			break;
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

	// a jelenlegi adatmodellünkben egy mezõbõl nem tudhatjuk annak koordinátáit!...
		// ezért a lépéshez szükség van a cél koordinátáira is külön, a cél mezõ önmagában nem elég
	from->pieceOnSquare->file = toFile;
	from->pieceOnSquare->rank = toRank;
	to->pieceOnSquare = from->pieceOnSquare;
	from->pieceOnSquare = NULL;

	to->pieceOnSquare->movedYet = true;
}
