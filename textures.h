#pragma once
#include <SDL.h>

//SDL_Texture* loadTexture(SDL_Renderer* renderer, char* textureName);

void drawBoard2(SDL_Renderer* renderer, SDL_Texture* texture);

void drawItem(SDL_Renderer* renderer, SDL_Texture* texture, int file, int rank);
