void animatePieceMovement(int startX, int startY, int endX, int endY, int player) {
    int frames = 2; // number of frames for animation
    float deltaX = (endX - startX) * TILE_SIZE / (float)frames;
    float deltaY = (endY - startY) * TILE_SIZE / (float)frames;

    float currentX = startX * TILE_SIZE;
    float currentY = startY * TILE_SIZE;

    // Preload piece texture for better performance
    SDL_Texture* pieceTexture = (player == PLAYER1)
        ? IMG_LoadTexture(renderer, "textures/red_piece.png")
        : IMG_LoadTexture(renderer, "textures/blue_piece.png");

    for (int i = 0; i <= frames; i++) {
        // Clear and redraw the board
        drawBoard(player);

        // Render the moving piece at the current position
        SDL_Rect pieceRect = {
            (int)currentX,
            (int)currentY,
            TILE_SIZE,
            TILE_SIZE
        };
        SDL_RenderCopy(renderer, pieceTexture, NULL, &pieceRect);

        // Present the updated frame
        SDL_RenderPresent(renderer);

        // Update position for the next frame
        currentX += deltaX;
        currentY += deltaY;
    }

    // Cleanup
    SDL_DestroyTexture(pieceTexture);
}