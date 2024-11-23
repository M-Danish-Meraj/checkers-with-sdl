
bool movePiece(int startX, int startY, int endX, int endY) {
    int player = board[startY][startX];
    bool captured = false;

    // Animate the piece movement
    animatePieceMovement(startX, startY, endX, endY, player);

    // Update the board state
    board[endY][endX] = player;
    board[startY][startX] = EMPTY;

    if (abs(endY - startY) == 2) {
        int jumpX = (startX + endX) / 2;
        int jumpY = (startY + endY) / 2;
        board[jumpY][jumpX] = EMPTY;
        captured = true; // Capture occurred
    }

    if ((endY == 0 && player == PLAYER1) || (endY == BOARD_SIZE - 1 && player == PLAYER2)) {
        board[endY][endX] |= KING_MASK;
    }

    return captured;
}

