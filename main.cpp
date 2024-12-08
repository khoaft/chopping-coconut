#include <SDL3/SDL.h>
#include <random>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <iostream>
#include <tuple>

const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;

std::tuple<int, int> random_position() {
    // Seed with a real random value, if available
  std::random_device rd;
  
  // Initialize the random number generator with the seed
  std::mt19937 gen(rd());
  
  // Define the distribution range, say from 0 to 100 inclusive, and make sure to divide by 10
  std::uniform_int_distribution<> rand_width(0, WINDOW_WIDTH / 10);
  std::uniform_int_distribution<> rand_height(0, WINDOW_HEIGHT / 10);
  
  // Generate a random multiple of 10
  return std::make_tuple(rand_width(gen) * 10, rand_height(gen) * 10);
}


enum Direction {
  UP,
  DOWN,
  LEFT,
  RIGHT
};

class Snake {
  public:
    int size;
    unsigned int length;
    Direction direction;
    std::vector<SDL_Rect> rects;

    Snake(int size, unsigned int length) {
      this->size = size;
      this->length = length;
      this->direction = RIGHT;

      std::tuple<int, int> position = random_position();
      int x = std::get<0>(position);
      int y = std::get<1>(position);

      for (int i = 0; i < this->length; i++) {
        SDL_Rect rect = { x - i * this->size, y, this->size, this->size };
        rects.push_back(rect);
      }
    }

    ~Snake() {
      rects.clear();
    }

    void grow() {
      SDL_Rect rect = { rects[rects.size() - 1].x, rects[rects.size() - 1].y, this->size, this->size };
      switch(direction) {
        case UP:
          rect.y += this->size;
          break;
        case DOWN:
          rect.y -= this->size;
          break;
        case LEFT:
          rect.x += this->size;
          break;
        case RIGHT:
          rect.x -= this->size;
          break;
      }
      rects.push_back(rect);
    }

    void move() {
      int x = rects[0].x;
      int y = rects[0].y;

      switch(direction) {
        case UP:
          rects[0].y -= this->size;
          break;
        case DOWN:
          rects[0].y += this->size;
          break;
        case LEFT:
          rects[0].x -= this->size;
          break;
        case RIGHT:
          rects[0].x += this->size;
          break;
      }

      for (int i = 1; i < rects.size(); i++) {
        int tmp = rects[i].x;
        rects[i].x = x;
        x = tmp;

        tmp = rects[i].y;
        rects[i].y = y;
        y = tmp;

        if (rects[0].x == rects[i].x && rects[0].y == rects[i].y) {
          std::cout << "Game Over" << std::endl;
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

class Food {
  public:
    int x;
    int y;
    int size;

    Food(int size) {
      std::tuple<int, int> position = random_position();
      this->x = std::get<0>(position);
      this->y = std::get<1>(position);
      this->size = size;
    }

    void spawn() {
      std::tuple<int, int> position = random_position();
      this->x = std::get<0>(position);
      this->y = std::get<1>(position);
    }

    void draw(SDL_Surface *surface) {
      Uint32 color = SDL_MapSurfaceRGB(surface, 255, 165, 0);
      SDL_Rect rect = { x, y, size, size };
      SDL_FillSurfaceRect(surface, &rect, color);
    }
};

int main(int argc, char *argv[]) {
  SDL_Window *window;
  SDL_Surface *surface;
  SDL_Event event;

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    return 3;
  }

  // Create an application window with the following settings:
  window = SDL_CreateWindow("Snake Game", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);

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

  Snake* snake = new Snake(10, 5);
  Food* food = new Food(10);

  unsigned int last_time = 0, current_time;
  while(1) {
    SDL_PollEvent(&event);

    if (event.type == SDL_EVENT_QUIT) {
      break;
    }

    if (event.type == SDL_EVENT_KEY_DOWN) {
      switch (event.key.key) {
        case SDLK_UP:
        case SDLK_W:
        case SDLK_K:
          if (snake->direction != DOWN) {
            snake->direction = UP;
          }
          break;
        case SDLK_DOWN:
        case SDLK_S:
        case SDLK_J:
          if (snake->direction != UP) {
            snake->direction = DOWN;
          }
          break;
        case SDLK_LEFT:
        case SDLK_A:
        case SDLK_H:
          if (snake->direction != RIGHT) {
            snake->direction = LEFT;
          }
          break;
        case SDLK_RIGHT:
        case SDLK_D:
        case SDLK_L:
          if (snake->direction != LEFT) {
            snake->direction = RIGHT;
          }
          break;
      }
    }

    current_time = SDL_GetTicks();
    if (current_time > last_time + 50) {
      SDL_FillSurfaceRect(surface, NULL, 0);

      // Fill the rectangle with the color
      snake->move();
      if (snake->rects[0].x == food->x && snake->rects[0].y == food->y) {
        snake->grow();
        food->spawn();
      }

      // Draw the snake and the food
      snake->draw(surface);
      food->draw(surface);

      // Update the window display
      SDL_UpdateWindowSurface( window );
      last_time = current_time;
    }
  }

  delete snake;
  delete food;

  SDL_DestroySurface(surface);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
