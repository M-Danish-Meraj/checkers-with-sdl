
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
