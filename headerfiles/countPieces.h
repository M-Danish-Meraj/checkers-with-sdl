
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