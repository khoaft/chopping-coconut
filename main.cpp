#include <SDL3/SDL.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    SDL_Window *window;
    SDL_Surface *surface;
    SDL_Event event;

    int width = 640;
    int height = 480;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    // Create an application window with the following settings:
    window = SDL_CreateWindow("An SDL3 window", width, height, SDL_WINDOW_OPENGL);

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
        return 3;
    }

    // Get the surface of the window
    surface = SDL_GetWindowSurface(window);
    if (!surface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't get surface of window: %s", SDL_GetError());
        return 3;
    }

    while (1) {
        SDL_PollEvent(&event);
        if (event.type == SDL_EVENT_QUIT) {
            break;
        }

        // Init random seed
        srand(time(NULL));

        // Random position and size
        int x_rand = rand() % width + 1;
        int y_rand = rand() % height + 1;
        int w_rand = rand() % (width / 5);
        int h_rand = rand() % (height / 5);

        SDL_Rect rect = { x_rand, y_rand, w_rand, h_rand };

        // Random color
        Uint32 color = SDL_MapSurfaceRGB(surface, rand() % 256, rand() % 256, rand() % 256);

        // Fill the rectangle with the color
        SDL_FillSurfaceRect(surface, &rect, color);

        // Update the window display
        SDL_UpdateWindowSurface( window );
    }

    SDL_DestroySurface(surface);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
