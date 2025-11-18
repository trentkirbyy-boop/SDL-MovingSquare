#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#define APP_NAME   "Space Counter"
#define APP_WIDTH  800
#define APP_HEIGHT 600

static SDL_Window   *window   = NULL;
static SDL_Renderer *renderer = NULL;

int main(void)
{
    int counter = 0;

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        SDL_Log("SDL_Init error: %s", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(
        APP_NAME,
        APP_WIDTH,
        APP_HEIGHT,
        SDL_WINDOW_RESIZABLE
    );

    if (!window)
    {
        SDL_Log("SDL_CreateWindow error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer)
    {
        SDL_Log("SDL_CreateRenderer error: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int running = 1;
    const Uint32 FRAME_MS = 16; // ~60 FPS

    while (running)
    {
        Uint32 frame_start = SDL_GetTicks();

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = 0;

            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.key == SDLK_SPACE)
                    counter++;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); // background (black)
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // text color (white)
        const int charsize = SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE;

        SDL_RenderDebugTextFormat(
            renderer,
            (float)((APP_WIDTH - (charsize * 46)) / 2),
            (APP_HEIGHT / 2),
            "(This program has been running for %" SDL_PRIu64 " seconds.)",
            SDL_GetTicks() / 1000
        );

        SDL_RenderDebugTextFormat(renderer, 100, 100, "Counter: %d", counter);

        // --- RED SQUARE ---
        SDL_FRect rect = { 375, 500, 50, 50};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        if (!SDL_RenderFillRect(renderer, &rect)) {
            SDL_Log("SDL_RenderFillRect error: %s", SDL_GetError());
        }

        SDL_RenderPresent(renderer);

        Uint32 elapsed = SDL_GetTicks() - frame_start;
        if (elapsed < FRAME_MS)
            SDL_Delay(FRAME_MS - elapsed);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}