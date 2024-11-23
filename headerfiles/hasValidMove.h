
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
