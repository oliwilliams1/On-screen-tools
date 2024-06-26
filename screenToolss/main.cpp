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
SDL_Surface* screenSurface = NULL;

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
        (int)displayMode.w, (int)displayMode.h,
        SDL_WINDOW_SHOWN
    );
    if (window == NULL) {
        fprintf(stderr, "could not create window: %s\n", SDL_GetError());
        return false;
    }
    screenSurface = SDL_GetWindowSurface(window);
    if (screenSurface == NULL) {
        fprintf(stderr, "could not get window: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

static SDL_Surface* loadImage(std::string path)
{
    SDL_Surface* img = IMG_Load(path.c_str());
    if (img == NULL) {
        fprintf(stderr, "could not load image: %s\n", IMG_GetError());
        return NULL;
    }
    SDL_Surface* optimizedImg = SDL_ConvertSurface(img, screenSurface->format, 0);
    if (optimizedImg == NULL) fprintf(stderr, "could not optimize image: %s\n", SDL_GetError());
    SDL_FreeSurface(img);
    return optimizedImg;
}

static void close()
{
    SDL_FreeSurface(screenSurface); screenSurface = NULL;
    SDL_DestroyWindow(window); window = NULL;
    SDL_Quit();
}

static int sdlThing()
{
    if (!init()) return 1;

    SDL_Surface* img = loadImage("../stupid map.png");
    if (img == NULL) return 1;

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
        SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 0));

        // Blit the image to the screen
        SDL_BlitSurface(img, NULL, screenSurface, NULL);
        SDL_UpdateWindowSurface(window);

        // Delay for a short time
        SDL_Delay(16); // Approximately 60 FPS
    }

    SDL_FreeSurface(img);
    img = NULL;
    close();
    
}

void takeScreenshot()
{
    PyRun_SimpleString(R"(ImageGrab.grab().save("Screenshot.png"))");
}

int main() {
    ScreenshotTaker* screenshotTaker = new ScreenshotTaker();
	screenshotTaker->takeScreenshot("Screenshot.png");

	if (screenshotTaker != NULL) {
		delete screenshotTaker;
	}

    return 0;
}