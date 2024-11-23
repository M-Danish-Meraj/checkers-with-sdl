
void gameLoop() {
    int currentPlayer = PLAYER1;

    while (true) {
        drawBoard(currentPlayer);
        handlePlayerTurn(currentPlayer);

        // Count pieces for both players
        int player1Count = 0, player2Count = 0;
        countPieces(&player1Count, &player2Count);

        // Check if any player has zero pieces
        if (player1Count == 0 || player2Count == 0) {
            int winner = (player1Count > 0) ? PLAYER1 : PLAYER2;
            displayWinner(winner);
            break;
        }

        // Check if any player has no valid moves
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

        // Switch turn to the next player
        currentPlayer = (currentPlayer == PLAYER1) ? PLAYER2 : PLAYER1;
    }
}
