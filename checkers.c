#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 850 // Increase this to make the window taller
#define BOARD_SIZE 8
#define TILE_SIZE (WINDOW_WIDTH/ BOARD_SIZE)
#define EMPTY 0
#define PLAYER1 1
#define PLAYER2 2
#define KING_MASK 4
TTF_Font* font = NULL;
int board[BOARD_SIZE][BOARD_SIZE];
SDL_Window* window = NULL;

#include "headerfiles/meme.h"
#include "headerfiles/showCredits.h"
#include "headerfiles/startMenu.h"
#include "headerfiles/countPieces.h"
#include "headerfiles/initBoard.h" 
#include "headerfiles/drawBoard.h"
#include "headerfiles/isValidMove.h"
#include "headerfiles/animatePieceMovement.h"
#include "headerfiles/movePiece.h"
#include "headerfiles/hasValidMove.h"
#include "headerfiles/displayWinner.h"
#include "headerfiles/handlePlayerturn.h"
#include "headerfiles/gameLoop.h"



int main(int argc, char* argv[]) {
   SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Checkers by Bots with Dots", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                WINDOW_WIDTH,           
                                WINDOW_HEIGHT,           
                                SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Init();
    font = TTF_OpenFont("Montserrat-ExtraBoldItalic.ttf", 64); 
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        exit(1);
    }

    // Show the start menu
    startMenu();

    // Initialize and start the game
    initBoard();
    gameLoop();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyTexture(trophyTexture);

    return 0;
}