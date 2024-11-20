void startMenu(char player1Name[], char player2Name[]) {
    // Load the background texture
    SDL_Texture* backgroundTexture = IMG_LoadTexture(renderer, "background.png");
    if (!backgroundTexture) {
        printf("Failed to load background image: %s\n", IMG_GetError());
        exit(1);
    }

    SDL_Event e;
    bool menuActive = true;

    // Define invisible button areas 
    SDL_Rect startButton = {167, 620, 200, 100}; // "START GAME" button position
    SDL_Rect quitButton = {430, 620, 200, 100};  // "QUIT" button position
    SDL_Rect memeButton = {430, 620, 200, 100};  // "meme" button position

    while (menuActive) {
        // Render the background
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        SDL_RenderPresent(renderer);

        // Handle events
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                exit(0); // Exit the application
            } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                // Get mouse position
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                // Check if "START GAME" button clicked
                if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &startButton)) {
                    menuActive = false; // Exit menu and start the game
                }

                // Check if "QUIT" button clicked
                if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &quitButton)) {
                    exit(0); // Quit the program
                }
            }
        }
    }

    SDL_DestroyTexture(backgroundTexture);
}