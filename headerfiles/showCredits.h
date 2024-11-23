// #define SDL_MAIN_HANDLED
// #include <SDL2/SDL.h>
// #include <stdbool.h>
// #include <stdio.h>
// #include <SDL2/SDL_image.h>
// #include <SDL2/SDL_ttf.h>

SDL_Texture* trophyTexture = NULL;
void showCredits() {
    // Load the credits image
    SDL_Texture* creditsTexture = IMG_LoadTexture(renderer, "textures/credits.png");
    if (!creditsTexture) {
        printf("Failed to load credits image: %s\n", IMG_GetError());
        return;
    }

    // Get the dimensions of the texture and the window
    int textureWidth, textureHeight;
    SDL_QueryTexture(creditsTexture, NULL, NULL, &textureWidth, &textureHeight);

    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

    // Set up the initial position for the animation (off-screen at the bottom)
    SDL_Rect destRect = {0, windowHeight, textureWidth, textureHeight};

    // Animation parameters
    int finalY = (windowHeight - textureHeight) / 2; // Center vertically
    int speed = 5; // Pixels per frame

    // Animation loop
    while (destRect.y > finalY) {
        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        // Move the texture upwards
        destRect.y -= speed;
        if (destRect.y < finalY) destRect.y = finalY; // Clamp to final position

        // Render the credits texture
        SDL_RenderCopy(renderer, creditsTexture, NULL, &destRect);
        SDL_RenderPresent(renderer);

        // Delay to control the animation speed
        SDL_Delay(16); // ~60 FPS
    }

    // Wait for user input to close the credits screen
    SDL_Event e;
    bool creditsActive = true;
    while (creditsActive) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                exit(0); // Exit the application
            } else if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN) {
                creditsActive = false; // Exit credits screen
            }
        }
    }

    SDL_DestroyTexture(creditsTexture);
}
