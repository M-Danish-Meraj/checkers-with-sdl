#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_SIZE 800
#define BOARD_SIZE 8
#define TILE_SIZE (WINDOW_SIZE / BOARD_SIZE)
#define EMPTY 0
#define PLAYER1 1
#define PLAYER2 2
#define KING_MASK 4
TTF_Font* font = NULL;
SDL_Texture* trophyTexture = NULL;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int board[BOARD_SIZE][BOARD_SIZE];

void startMenu(char player1Name[], char player2Name[]) {
    SDL_Event e;
    bool menuActive = true;

    // Colors
    SDL_Color textColor = {148, 98, 57, 255};
    SDL_Color buttonColor = {230, 207, 171, 255};
    SDL_Color buttonHoverColor = {255, 232, 196, 255};

    // Title text
    SDL_Surface* titleSurface = TTF_RenderText_Blended(font, "Checkers Game", textColor);
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_FreeSurface(titleSurface);

    SDL_Rect titleRect = {(WINDOW_SIZE - 400) / 2, 100, 400, 50};

    // Button rectangles
    SDL_Rect startButton = {(WINDOW_SIZE - 200) / 2, 300, 200, 50};
    SDL_Rect quitButton = {(WINDOW_SIZE - 200) / 2, 375, 200, 50};

    while (menuActive) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Clear screen
        SDL_RenderClear(renderer);

        // Render the title
        SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);

        // Get mouse position
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        // Check for hover and draw buttons
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        // "Start" button
        if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &startButton)) {
            SDL_SetRenderDrawColor(renderer, buttonHoverColor.r, buttonHoverColor.g, buttonHoverColor.b, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, buttonColor.r, buttonColor.g, buttonColor.b, 255);
        }
        SDL_RenderFillRect(renderer, &startButton);

        // "Quit" button
        if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &quitButton)) {
            SDL_SetRenderDrawColor(renderer, buttonHoverColor.r, buttonHoverColor.g, buttonHoverColor.b, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, buttonColor.r, buttonColor.g, buttonColor.b, 255);
        }
        SDL_RenderFillRect(renderer, &quitButton);

        // Render button text
        SDL_Surface* startSurface = TTF_RenderText_Blended(font, "Start", textColor);
        SDL_Surface* quitSurface = TTF_RenderText_Blended(font, "Quit", textColor);

        SDL_Texture* startTexture = SDL_CreateTextureFromSurface(renderer, startSurface);
        SDL_Texture* quitTexture = SDL_CreateTextureFromSurface(renderer, quitSurface);

        SDL_Rect startTextRect = {startButton.x + 50, startButton.y + 10, 100, 30};
        SDL_Rect quitTextRect = {quitButton.x + 50, quitButton.y + 10, 100, 30};

        SDL_RenderCopy(renderer, startTexture, NULL, &startTextRect);
        SDL_RenderCopy(renderer, quitTexture, NULL, &quitTextRect);

        SDL_FreeSurface(startSurface);
        SDL_FreeSurface(quitSurface);
        SDL_DestroyTexture(startTexture);
        SDL_DestroyTexture(quitTexture);

        SDL_RenderPresent(renderer);

        // Handle events
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                exit(0);
            } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                // Check if "Start" button clicked
                if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &startButton)) {
                    menuActive = false; // Exit menu and start the game
                }
                // Check if "Quit" button clicked
                if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &quitButton)) {
                    exit(0); // Quit the program
                }
            }
        }
    }

    SDL_DestroyTexture(titleTexture);
}

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

void countPieces(int* player1Count, int* player2Count) {
    *player1Count = 0;
    *player2Count = 0;
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            if (board[y][x] == PLAYER1) (*player1Count)++;
            else if (board[y][x] == PLAYER2) (*player2Count)++;
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

void drawBoard(int currentPlayer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            SDL_Rect tileRect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            if ((x + y) % 2 == 0) {
                SDL_SetRenderDrawColor(renderer, 148, 98, 57, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 240, 217, 181, 255);
            }
            SDL_RenderFillRect(renderer, &tileRect);

            int centerX = x * TILE_SIZE + TILE_SIZE / 2;
            int centerY = y * TILE_SIZE + TILE_SIZE / 2;
            int radius = TILE_SIZE / 3;

            if (board[y][x] == PLAYER1) {
                SDL_SetRenderDrawColor(renderer, 120, 0, 0, 255); // Actual pieces
                drawCircle(renderer, centerX, centerY, radius+4);

                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Outlines
                drawCircle(renderer, centerX, centerY, radius);
            } else if (board[y][x] == PLAYER2) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 80, 255);
                drawCircle(renderer, centerX, centerY, radius+4);

                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                drawCircle(renderer, centerX, centerY, radius);
            }
        }
    }

    // Display text
    char statusText[128];
    int player1Count, player2Count;
    countPieces(&player1Count, &player2Count);
    sprintf(statusText, "Turn: Player %d  (1=RED, 2=BLUE) --- RED: %d --- BLUE: %d", currentPlayer, player1Count, player2Count);

    SDL_Color textColor = {0, 0, 0, 255};
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, statusText, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textRect = {10, 10, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

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
        drawBoard(player);

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

void displayWinner(int winner) {
    char winnerText[128];
    if (winner == PLAYER1) {
        sprintf(winnerText, "Player 1 Wins!");
    } else if (winner == PLAYER2) {
        sprintf(winnerText, "Player 2 Wins!");
    } else {
        sprintf(winnerText, "It's a Draw!");
    }

    SDL_Color textColor = {0, 0, 0, 255};
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, winnerText, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);

    SDL_Rect textRect = {
    (WINDOW_SIZE - textWidth) / 2,  // X: Center horizontally
    (WINDOW_SIZE - textHeight) / 2 - 150, // Y: Shift 50 pixels down
    textWidth,
    textHeight
    };


    SDL_Rect trophyRect = {
        0,                     // Start at the left edge
        WINDOW_SIZE,           // Start off-screen
        WINDOW_SIZE,           // Trophy fills the entire window
        WINDOW_SIZE
    };

    // Animate the trophy sliding up
    for (int i = 0; i < WINDOW_SIZE; i += 5) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Draw the trophy
        trophyRect.y = WINDOW_SIZE - i;  // Slide up
        SDL_RenderCopy(renderer, trophyTexture, NULL, &trophyRect);

        // Draw the winner text
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(10);  // Delay to create animation effect
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(5000);  // Pause for 5 seconds before exiting

    SDL_DestroyTexture(textTexture);
}



void gameLoop() {
    int currentPlayer = PLAYER1;
    while (true) {
        drawBoard(currentPlayer);
        handlePlayerTurn(currentPlayer);

        if (!hasValidMoves(PLAYER1) || !hasValidMoves(PLAYER2)) {
            int winner = PLAYER1;
            if (!hasValidMoves(PLAYER1)) {
                winner = PLAYER2;
            } else if (!hasValidMoves(PLAYER2)) {
                winner = PLAYER1;
            } else {
                winner = 0; // Draw condition
            }
            displayWinner(winner);
            break;
        }

        currentPlayer = (currentPlayer == PLAYER1) ? PLAYER2 : PLAYER1;
    }
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Checkers Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_SIZE, WINDOW_SIZE, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Init();
    font = TTF_OpenFont("Montserrat-ExtraBoldItalic.ttf", 26); // Replace with the actual path
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        exit(1);
    }

    trophyTexture = IMG_LoadTexture(renderer, "trophy.png");
    if (!trophyTexture) {
        printf("Failed to load trophy image: %s\n", IMG_GetError());
        exit(1);
    }

    char player1Name[50] = "Player 1";
    char player2Name[50] = "Player 2";

    // Show the start menu
    startMenu(player1Name, player2Name);

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
