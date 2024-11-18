#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdbool.h>

#define WINDOW_SIZE 800
#define BOARD_SIZE 8
#define TILE_SIZE (WINDOW_SIZE / BOARD_SIZE)
#define EMPTY 0
#define PLAYER1 1
#define PLAYER2 2
#define KING_MASK 4

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int board[BOARD_SIZE][BOARD_SIZE];

// Helper function to draw a filled circle in SDL
void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // Horizontal offset
            int dy = radius - h; // Vertical offset
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
            }
        }
    }
}


void initBoard() {
    // Initialize an 8x8 board with players' pieces
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            if (y < 3 && (x + y) % 2 == 1) board[y][x] = PLAYER1;
            else if (y > 4 && (x + y) % 2 == 1) board[y][x] = PLAYER2;
            else board[y][x] = EMPTY;
        }
    }
}

void drawBoard() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            SDL_Rect tileRect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};

            // Draw checkerboard pattern
            if ((x + y) % 2 == 0) {
                SDL_SetRenderDrawColor(renderer, 148, 98, 57, 255); // Dark brown
            } else {
                SDL_SetRenderDrawColor(renderer, 240, 217, 181, 255); // Light tan
            }
            SDL_RenderFillRect(renderer, &tileRect);

            int centerX = x * TILE_SIZE + TILE_SIZE / 2;
            int centerY = y * TILE_SIZE + TILE_SIZE / 2;
            int radius = TILE_SIZE / 3;

            // Draw player pieces with outlines
            if (board[y][x] == PLAYER1) {
                SDL_SetRenderDrawColor(renderer, 120, 0, 0, 255); // Dark red for outline
                drawCircle(renderer, centerX, centerY, radius + 4); // Slightly larger radius for outline

                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red for Player 1
                drawCircle(renderer, centerX, centerY, radius); // Main piece circle
            } else if (board[y][x] == PLAYER2) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 70, 255); // Dark blue for outline
                drawCircle(renderer, centerX, centerY, radius + 4); // Slightly larger radius for outline

                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue for Player 2
                drawCircle(renderer, centerX, centerY, radius); // Main piece circle
            }
        }
    }
    SDL_RenderPresent(renderer);
}

bool isValidMove(int player, int startX, int startY, int endX, int endY) {
    int dx = endX - startX;
    int dy = endY - startY;
    int target = board[endY][endX];
    if (target != EMPTY || (abs(dx) != abs(dy)) || abs(dx) > 2 || abs(dy) > 2) return false;

    bool isCapture = abs(dx) == 2;
    if (isCapture) {
        int jumpX = (startX + endX) / 2;
        int jumpY = (startY + endY) / 2;
        if (board[jumpY][jumpX] != (player == PLAYER1 ? PLAYER2 : PLAYER1)) return false;
    }
    return true;
}

void movePiece(int startX, int startY, int endX, int endY) {
    int player = board[startY][startX];
    board[endY][endX] = player;
    board[startY][startX] = EMPTY;

    if (abs(endY - startY) == 2) {
        int jumpX = (startX + endX) / 2;
        int jumpY = (startY + endY) / 2;
        board[jumpY][jumpX] = EMPTY;
    }

    if ((endY == 0 && player == PLAYER1) || (endY == BOARD_SIZE - 1 && player == PLAYER2)) {
        board[endY][endX] |= KING_MASK;
    }
}

void handlePlayerTurn(int player) {
    SDL_Event e;
    bool turnCompleted = false;
    int startX = -1, startY = -1, endX, endY;

    while (!turnCompleted) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) exit(0);
            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int x = e.button.x / TILE_SIZE;
                int y = e.button.y / TILE_SIZE;
                if (startX == -1 && board[y][x] == player) {  // Select piece
                    startX = x;
                    startY = y;
                } else if (startX != -1) {  // Select destination
                    endX = x;
                    endY = y;
                    if (isValidMove(player, startX, startY, endX, endY)) {
                        movePiece(startX, startY, endX, endY);
                        turnCompleted = true;
                    }
                    startX = -1;
                }
            }
        }
        drawBoard();
    }
}

bool hasValidMoves(int player) {
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            if (board[y][x] == player) {
                for (int dy = -2; dy <= 2; dy++) {
                    for (int dx = -2; dx <= 2; dx++) {
                        if (isValidMove(player, x, y, x + dx, y + dy)) return true;
                    }
                }
            }
        }
    }
    return false;
}

void gameLoop() {
    int currentPlayer = PLAYER1;
    while (true) {
        drawBoard();
        handlePlayerTurn(currentPlayer);
        if (!hasValidMoves(PLAYER1) || !hasValidMoves(PLAYER2)) break;
        currentPlayer = (currentPlayer == PLAYER1) ? PLAYER2 : PLAYER1;
    }
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Checkers Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_SIZE, WINDOW_SIZE, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    initBoard();
    gameLoop();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
