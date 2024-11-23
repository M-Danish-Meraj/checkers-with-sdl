
void displayWinner(int winner) {

    trophyTexture = IMG_LoadTexture(renderer, "textures/trophy.png");
    if (!trophyTexture) {
        printf("Failed to load trophy image: %s\n", IMG_GetError());
        exit(1);
    }

    char winnerText[128];
    if (winner == PLAYER1) {
        sprintf(winnerText, "Player 1 Wins!");
    } else if (winner == PLAYER2) {
        sprintf(winnerText, "Player 2 Wins!");
    } else {
        sprintf(winnerText, "It's a Draw!");
    }

    SDL_Color textColor_winner = {255 ,0 ,0 ,255 };
    if(winner!=1)
        textColor_winner = (SDL_Color){0, 0, 255, 255};

    SDL_Surface* textSurface = TTF_RenderText_Blended(font, winnerText, textColor_winner);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);

    SDL_Rect textRect = { 120, 210, textWidth+100, textHeight+100};


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
    SDL_Delay(20000);  // Pause for 5 seconds before exiting

    SDL_DestroyTexture(textTexture);
}
