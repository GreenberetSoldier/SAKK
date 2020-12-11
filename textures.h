#pragma once
#include <SDL.h>

//SDL_Texture* loadTexture(SDL_Renderer* renderer, char* textureName);

void drawPiece(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y);

void drawBoard2(SDL_Renderer* renderer, SDL_Texture* texture);

void drawSelection(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y);

void drawTargets(SDL_Renderer* renderer, board_t* board, SDL_Texture* texture);