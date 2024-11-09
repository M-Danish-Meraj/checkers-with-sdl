// drawBoard.c
#include <SDL2/SDL.h>
#include "drawBoard.h"
#include "globals.h"  // For constants like BOARD_SIZE, TILE_SIZE, etc.

void drawBoard(SDL_Renderer* renderer, char board[BOARD_SIZE][BOARD_SIZE]) {
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            SDL_Rect tile = {col * TILE_SIZE, row * TILE_SIZE, TILE_SIZE, TILE_SIZE};

            // Alternate between light and dark tiles
            if ((row + col) % 2 == 0) {
                SDL_SetRenderDrawColor(renderer, 240, 217, 181, 255);  // Light color
            } else {
                SDL_SetRenderDrawColor(renderer, 181, 136, 99, 255);   // Dark color
            }
            SDL_RenderFillRect(renderer, &tile);

            // Draw pieces
            if (board[row][col] == BLACK) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black piece
            } else if (board[row][col] == RED) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red piece
            } else {
                continue; // Empty tile, skip to next
            }
            SDL_Rect piece = {col * TILE_SIZE + TILE_SIZE / 4, row * TILE_SIZE + TILE_SIZE / 4, TILE_SIZE / 2, TILE_SIZE / 2};
            SDL_RenderFillRect(renderer, &piece);
        }
    }
}
