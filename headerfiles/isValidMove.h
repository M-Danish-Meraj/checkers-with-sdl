
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
