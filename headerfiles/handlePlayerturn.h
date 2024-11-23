
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
                        bool captured = movePiece(startX, startY, endX, endY);

                        // Count pieces after the move
                        int player1Count = 0, player2Count = 0;
                        countPieces(&player1Count, &player2Count);

                        // Check if any player has 0 pieces and end the game immediately
                        if (player1Count == 0 || player2Count == 0) {
                            int winner = (player1Count > 0) ? PLAYER1 : PLAYER2;
                            displayWinner(winner);
                            exit(0);  // Exit the game loop
                        }

                        // Allow another move if capture occurred
                        if (captured && hasValidMoves(player)) {
                            startX = endX;
                            startY = endY;
                            drawBoard(player);
                            continue;
                        }

                        turnCompleted = true;  // End turn if no capture or no valid moves
                    }
                    startX = -1;  // Reset selection
                }
            }
        }
        drawBoard(player);
    }
}