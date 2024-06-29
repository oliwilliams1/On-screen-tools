#include <iostream>
#include <SDL2/SDL.h>
#undef main
#include <SDL2/SDL_image.h>
#include "screenshotTaker.h"

#ifdef _DEBUG
#undef _DEBUG
#include "Python.h"
#define _DEBUG
#else
#include "Python.h"
#endif

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;

bool transparent = false;

static bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
        return false;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        fprintf(stderr, "could not initialize sdl2_image: %s\n", IMG_GetError());
        return false;
    }
    SDL_DisplayMode displayMode;
    if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0) {
        std::cerr << "SDL_GetCurrentDisplayMode failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    window = SDL_CreateWindow(
        "hello_sdl2",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    SDL_SetWindowOpacity(window, (transparent ? 0.0f : 1.0f));
    if (window == NULL) {
        fprintf(stderr, "could not create window: %s\n", SDL_GetError());
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "could not create renderer: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

static void close()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

static int sdlThing(std::vector<uint8_t> imageData)
{
    if (!init()) return 1;

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (texture == NULL) {
        fprintf(stderr, "could not create texture: %s\n", SDL_GetError());
        close();
        return 1;
    }

    void* pixels;
    int pitch;
    if (SDL_LockTexture(texture, NULL, &pixels, &pitch) != 0) {
        fprintf(stderr, "could not lock texture: %s\n", SDL_GetError());
        close();
        return 1;
    }

    memcpy(pixels, imageData.data(), imageData.size());

    SDL_UnlockTexture(texture);

    bool running = true;
    while (running) {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render the texture
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        // Delay for a short time
        SDL_Delay(16); // Approximately 60 FPS
    }

    close();

    return 0;
}

int main() {
    ScreenshotTaker* screenshotTaker = new ScreenshotTaker();
    std::vector<uint8_t> imageData = screenshotTaker->takeScreenshot();

    sdlThing(imageData);

    if (screenshotTaker != NULL) {
        delete screenshotTaker;
    }

    return 0;
}