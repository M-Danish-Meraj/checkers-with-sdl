void drawBoard(int currentPlayer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Load textures
    SDL_Surface* woodTexture1Surface = IMG_Load("textures/dark_wood.png");
    SDL_Surface* woodTexture2Surface = IMG_Load("textures/light_wood.png");
    SDL_Surface* player1PieceSurface = IMG_Load("textures/red_piece.png");
    SDL_Surface* player2PieceSurface = IMG_Load("textures/blue_piece.png");

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

    SDL_Color textColor_turn = {255 ,0 ,0 ,255 };
    if(currentPlayer!=1)
        textColor_turn = (SDL_Color){0, 0, 255, 255};
    SDL_Color textColor_red = {255, 0, 0, 255};
    SDL_Color textColor_blue = {0, 0, 255, 255};

    // Render and display each liness
    SDL_Surface* textSurface1 = TTF_RenderText_Blended(font, statusLine1, textColor_turn);
    SDL_Surface* textSurface2 = TTF_RenderText_Blended(font, statusLine2, textColor_red);
    SDL_Surface* textSurface3 = TTF_RenderText_Blended(font, statusLine3, textColor_blue);

    SDL_Texture* textTexture1 = SDL_CreateTextureFromSurface(renderer, textSurface1);
    SDL_Texture* textTexture2 = SDL_CreateTextureFromSurface(renderer, textSurface2);
    SDL_Texture* textTexture3 = SDL_CreateTextureFromSurface(renderer, textSurface3);

    SDL_Rect textRect1 = {260, 805, 250, 35};  // Line 1 position
    SDL_Rect textRect2 = {17, 805, 100, 35};  // Line 2 position (slightly lower)
    SDL_Rect textRect3 = {673, 805, 100, 35};  // Line 3 position

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
