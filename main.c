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
SDL_Texture* trophyTexture = NULL;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int board[BOARD_SIZE][BOARD_SIZE];

void meme() {
    // Load the meme image
    SDL_Texture* memeTexture = IMG_LoadTexture(renderer, "meme.png");
    if (!memeTexture) {
        printf("Failed to load meme image: %s\n", IMG_GetError());
        return;
    }

    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
    SDL_RenderClear(renderer);

    // Get the texture dimensions
    int imgWidth, imgHeight;
    SDL_QueryTexture(memeTexture, NULL, NULL, &imgWidth, &imgHeight);

    // Define the rendering rectangle (centered)
    SDL_Rect renderQuad = { 
        (255 - imgWidth) / 2, 
        (190 - imgHeight) / 2, 
        WINDOW_WIDTH , 
        WINDOW_HEIGHT 
    };

    // Render the meme image
    SDL_RenderCopy(renderer, memeTexture, NULL, &renderQuad);

    // Present the image to the screen
    SDL_RenderPresent(renderer);

    // Wait for a short time before returning to the menu
    SDL_Event e;
    bool showingMeme = true;
    while (showingMeme) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                exit(0); // Exit application
            } else if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN) {
                showingMeme = false; // Exit meme display
            }
        }
    }

    // Clean up resources
    SDL_DestroyTexture(memeTexture);
}


void startMenu(char player1Name[], char player2Name[]) {
    // Load the background texture
    SDL_Texture* backgroundTexture = IMG_LoadTexture(renderer, "background.png");
    if (!backgroundTexture) {
        printf("Failed to load background image: %s\n", IMG_GetError());
        exit(1);
    }

    SDL_Event e;
    bool menuActive = true;

    // Define button areas
    SDL_Rect startButton = {167, 620, 200, 100}; // "START GAME" button position
    SDL_Rect quitButton = {430, 620, 200, 100};  // "QUIT" button position
    SDL_Rect memeButton = {350, 375, 100, 100};  // "MEME" button position

    while (menuActive) {
        // Render the background
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        SDL_RenderPresent(renderer);

        // Handle events
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                exit(0); // Exit the application
            } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                // Get mouse position
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                // Check if "START GAME" button clicked
                if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &startButton)) {
                    menuActive = false; // Exit menu and start the game
                }

                // Check if "QUIT" button clicked
                if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &quitButton)) {
                    exit(0); // Quit the program
                }

                // Check if "MEME" button clicked
                if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &memeButton)) {
                    meme(); // Call the meme function
                }
            }
        }
    }

    SDL_DestroyTexture(backgroundTexture);
}





void countPieces(int* player1Count, int* player2Count) {
    *player1Count = 0;
    *player2Count = 0;
    for (int y = 0; y < BOARD_SIZE; y++) 
    {
        for (int x = 0; x < BOARD_SIZE; x++) 
        {
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Load textures
    SDL_Surface* woodTexture1Surface = IMG_Load("dark_wood.png");
    SDL_Surface* woodTexture2Surface = IMG_Load("light_wood.png");
    SDL_Surface* player1PieceSurface = IMG_Load("red_piece.png");
    SDL_Surface* player2PieceSurface = IMG_Load("blue_piece.png");

    SDL_Texture* woodTexture1 = SDL_CreateTextureFromSurface(renderer, woodTexture1Surface);
    SDL_Texture* woodTexture2 = SDL_CreateTextureFromSurface(renderer, woodTexture2Surface);
    SDL_Texture* player1Piece = SDL_CreateTextureFromSurface(renderer, player1PieceSurface);
    SDL_Texture* player2Piece = SDL_CreateTextureFromSurface(renderer, player2PieceSurface);

    // Free surfaces after creating textures
    SDL_FreeSurface(woodTexture1Surface);
    SDL_FreeSurface(woodTexture2Surface);
    SDL_FreeSurface(player1PieceSurface);
    SDL_FreeSurface(player2PieceSurface);


    for (int y = 0; y < BOARD_SIZE; y++) {
    for (int x = 0; x < BOARD_SIZE; x++) {
        SDL_Rect tileRect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};

        // Alternate between wood textures
        if ((x + y) % 2 == 0) {
            SDL_RenderCopy(renderer, woodTexture1, NULL, &tileRect);
        } else {
            SDL_RenderCopy(renderer, woodTexture2, NULL, &tileRect);
        }

        // Render pieces on top of the tiles
        if (board[y][x] == PLAYER1) {
            SDL_Rect pieceRect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_RenderCopy(renderer, player1Piece, NULL, &pieceRect);
        } else if (board[y][x] == PLAYER2) {
            SDL_Rect pieceRect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_RenderCopy(renderer, player2Piece, NULL, &pieceRect);
        }
    }
}


    // Display text
    char statusLine1[128], statusLine2[128], statusLine3[128];
    int player1Count, player2Count;
    countPieces(&player1Count, &player2Count);

    // Format each line separately
    sprintf(statusLine1, "Turn: Player %s", currentPlayer == 1 ? "RED" : "BLUE");
    sprintf(statusLine2, "RED: %d", player1Count);
    sprintf(statusLine3, "BLUE: %d", player2Count);

    SDL_Color textColor_turn = {255 ,255 ,255 ,255 };
    SDL_Color textColor_red = {255, 0, 0, 255};
    SDL_Color textColor_blue = {0, 0, 255, 255};

    // Render and display each line
    SDL_Surface* textSurface1 = TTF_RenderText_Blended(font, statusLine1, textColor_turn);
    SDL_Surface* textSurface2 = TTF_RenderText_Blended(font, statusLine2, textColor_red);
    SDL_Surface* textSurface3 = TTF_RenderText_Blended(font, statusLine3, textColor_blue);

    SDL_Texture* textTexture1 = SDL_CreateTextureFromSurface(renderer, textSurface1);
    SDL_Texture* textTexture2 = SDL_CreateTextureFromSurface(renderer, textSurface2);
    SDL_Texture* textTexture3 = SDL_CreateTextureFromSurface(renderer, textSurface3);

    SDL_Rect textRect1 = {280, 810, textSurface1->w, textSurface1->h};  // Line 1 position
    SDL_Rect textRect2 = {5, 810, textSurface2->w, textSurface2->h};  // Line 2 position (slightly lower)
    SDL_Rect textRect3 = {673, 810, textSurface3->w, textSurface3->h};  // Line 3 position

    SDL_RenderCopy(renderer, textTexture1, NULL, &textRect1);
    SDL_RenderCopy(renderer, textTexture2, NULL, &textRect2);
    SDL_RenderCopy(renderer, textTexture3, NULL, &textRect3);

    // Free resources
    SDL_FreeSurface(textSurface1);
    SDL_FreeSurface(textSurface2);
    SDL_FreeSurface(textSurface3);
    SDL_DestroyTexture(textTexture1);
    SDL_DestroyTexture(textTexture2);
    SDL_DestroyTexture(textTexture3);

    // Update the renderer
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(woodTexture1);
    SDL_DestroyTexture(woodTexture2);
    SDL_DestroyTexture(player1Piece);
    SDL_DestroyTexture(player2Piece);

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

    SDL_Color textColor = {0, 255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, winnerText, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);

    SDL_Rect textRect = {
    (WINDOW_WIDTH - textWidth) / 2,  // X: Center horizontally
    (WINDOW_HEIGHT - textHeight) / 2 - 100, // Y: Shift 50 pixels down
    textWidth,
    textHeight
    };


   SDL_Rect trophyRect = {
    0,                     // Start at the left edge
    WINDOW_HEIGHT,         // Start off-screen below the visible window
    WINDOW_WIDTH,          // Trophy spans the entire window width
    WINDOW_HEIGHT          // Trophy spans the entire window height
};


    // Animate the trophy sliding up
    for (int i = 0; i < WINDOW_HEIGHT; i += 5) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw the trophy
        trophyRect.y = WINDOW_HEIGHT - i;  // Slide up
        SDL_RenderCopy(renderer, trophyTexture, NULL, &trophyRect);

        // Draw the winner text
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(3);  // Delay to create animation effect
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
window = SDL_CreateWindow("Checkers Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                            WINDOW_WIDTH,           // Use the updated width
                            WINDOW_HEIGHT,          // Use the updated height     
                            SDL_WINDOW_SHOWN);
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