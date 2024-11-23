
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

SDL_Renderer* renderer = NULL;

void meme() {
    // Load the meme image
    SDL_Texture* memeTexture = IMG_LoadTexture(renderer, "textures/meme.png");
    if (!memeTexture) {
        printf("Failed to load meme image: %s\n", IMG_GetError());
        return;
    }

    // Get the texture dimensions
    int imgWidth, imgHeight;
    SDL_QueryTexture(memeTexture, NULL, NULL, &imgWidth, &imgHeight);

    // Get the window dimensions
    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

    // Define the initial scale for the zoom-in animation
    float scale = 1.5f; // Start at 10% of the original size
    float scaleStep = 0.05f; // Increment per frame

    // Animation loop
    while (scale < 4.1f) {
        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        // Calculate the size and position for the current scale
        int currentWidth = (int)(imgWidth * scale);
        int currentHeight = (int)(imgHeight * scale);
        SDL_Rect renderQuad = {
            (windowWidth - currentWidth) / 2, // Center horizontally
            (windowHeight - currentHeight) / 2, // Center vertically
            currentWidth,
            currentHeight
        };

        // Render the scaled image
        SDL_RenderCopy(renderer, memeTexture, NULL, &renderQuad);
        SDL_RenderPresent(renderer);

        // Increase the scale for the next frame
        scale += scaleStep;
        if (scale > 4.1f) scale = 4.1f; // Clamp to final size

        // Delay to control the animation speed
        SDL_Delay(20); // ~60 FPS
    }
}