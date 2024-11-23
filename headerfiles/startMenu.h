// #define SDL_MAIN_HANDLED
// #include <SDL2/SDL.h>
// #include <stdbool.h>
// #include <stdio.h>
// #include <SDL2/SDL_image.h>
// #include <SDL2/SDL_ttf.h>

void startMenu() {
    // Load the background texture
    SDL_Texture* backgroundTexture = IMG_LoadTexture(renderer, "textures/background.png");
    if (!backgroundTexture) {
        printf("Failed to load background image: %s\n", IMG_GetError());
        exit(1);
    }

    SDL_Event e;
    bool menuActive = true;

    // Define button areas
    SDL_Rect startButton = {167, 620, 200, 100}; // "START GAME" button position
    SDL_Rect quitButton = {430, 620, 200, 100};  // "QUIT" button position
    SDL_Rect memeButton = {350, 375, 100, 100};  // "MEME" button position
    SDL_Rect moreButton = {650, 760, 100, 35};   // "More" button position

    while (menuActive) {
        // Render the background
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        // Render the "More" button
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); // Light gray
        SDL_Rect moreButtonBorder = {moreButton.x - 2, moreButton.y - 2, moreButton.w + 4, moreButton.h + 4};
        SDL_RenderFillRect(renderer, &moreButtonBorder);

        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); // Darker gray
        SDL_RenderFillRect(renderer, &moreButton);

        // Render the text "More" on the button
        SDL_Surface* textSurface = TTF_RenderText_Blended(font, "MORE", (SDL_Color){255, 255, 255, 255});
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = {moreButton.x +17 , moreButton.y + 5 , 70, 25}; // Text centered on the button
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);

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

                // Check if "MEME" button clicked
                if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &memeButton)) {
                    meme(); // Call the meme function
                }

                // Check if "More" button clicked
                if (SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &moreButton)) {
                    showCredits(); // Show the credits window
                }
            }
        }
    }

    SDL_DestroyTexture(backgroundTexture);
}
