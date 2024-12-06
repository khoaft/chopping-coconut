#include <SDL3/SDL.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

enum Direction {
  UP,
  DOWN,
  LEFT,
  RIGHT
};

class Snake {
  public:
    unsigned int size;
    Direction direction;
    std::vector<SDL_Rect> rects;
    unsigned int velocity;

    Snake(int x, int y, unsigned int size) {
      direction = RIGHT;
      velocity = 10;
      for (int i = 0; i <= size; i++) {
        SDL_Rect rect = { x - i * 10, y, 10, 10 };
        rects.push_back(rect);
      }
    }

    ~Snake() {
      rects.clear();
    }

    void move() {
      switch(direction) {
        case UP:
          rects[0].y -= velocity;
          break;
        case DOWN:
          rects[0].y += velocity;
          break;
        case LEFT:
          rects[0].x -= velocity;
          break;
        case RIGHT:
          rects[0].x += velocity;
          break;
      }

      for (int i = 1; i < rects.size(); i++) {
        switch(direction) {
          case UP:
          case DOWN:
            if (rects[i].x == rects[i - 1].x) {
              rects[i].y = (direction == UP) ? rects[i].y - velocity : rects[i].y + velocity;
            } else {
              rects[i].x = (rects[i - 1].x < rects[i].x) ? rects[i].x - velocity : rects[i].x + velocity;
            }
            break;
          case LEFT:
          case RIGHT:
            if (rects[i].y == rects[i - 1].y) {
              rects[i].x = (direction == LEFT) ? rects[i].x - velocity : rects[i].x + velocity;
            } else {
              rects[i].y = (rects[i - 1].y < rects[i].y) ? rects[i].y - velocity : rects[i].y + velocity;
            }
            break;
        }
      }
    }

    void draw(SDL_Surface *surface) {
      Uint32 color = SDL_MapSurfaceRGB(surface, 255, 255, 255);
      for (int i = 0; i < rects.size(); i++) {
        SDL_FillSurfaceRect(surface, &rects[i], color);
      }
    }
};

int main(int argc, char *argv[]) {
  SDL_Window *window;
  SDL_Surface *surface;
  SDL_Event event;

  int width = 1280;
  int height = 720;

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
  // Init random seed
  srand(time(NULL));

  // Random position
  int x_rand = rand() % width;
  int y_rand = rand() % height;

  Snake* snake = new Snake(x_rand, y_rand, 5);
  unsigned int last_time = 0, current_time;
  while (1) {
    SDL_PollEvent(&event);

    if (event.type == SDL_EVENT_QUIT) {
      break;
    }

    if (event.type == SDL_EVENT_KEY_DOWN) {
      switch (event.key.key) {
        case SDLK_UP:
          snake->direction = UP;
          break;
        case SDLK_DOWN:
          snake->direction = DOWN;
          break;
        case SDLK_LEFT:
          snake->direction = LEFT;
          break;
        case SDLK_RIGHT:
          snake->direction = RIGHT;
          break;
      }
    }

    current_time = SDL_GetTicks();
    if (current_time > last_time + 100) {
      SDL_FillSurfaceRect(surface, NULL, 0);

      // Fill the rectangle with the color
      snake->move();
      snake->draw(surface);

      // Update the window display
      SDL_UpdateWindowSurface( window );
      last_time = current_time;
    }
  }

  delete snake;

  SDL_DestroySurface(surface);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
