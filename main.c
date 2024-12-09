#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game/game.h"
#include "snake/snake.h"
#include "food/food.h"

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

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

  // Initialize the game
  Game game = { RUNNING, 0 };

  // Create the snake
  srand(time(0));
  int x = (rand() % (WINDOW_WIDTH / 10)) * 10;
  int y = (rand() % (WINDOW_HEIGHT / 10)) * 10;
  Snake* snake = new_snake(5, 10, x, y);

  // Create the food
  Food* food = new_food(10);
  spawn_food(food, WINDOW_WIDTH, WINDOW_HEIGHT);

  unsigned int last_time = 0, current_time;
  while(game.state == RUNNING) {
    SDL_PollEvent(&event);

    if (event.type == SDL_EVENT_QUIT) {
      game.state = QUIT;
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

      if (!move_snake(snake, WINDOW_WIDTH, WINDOW_HEIGHT)) {
        game.state = GAME_OVER;
        SDL_Delay(1000);
        break;
      }

      if (snake->head->rect.x == food->x && snake->head->rect.y == food->y) {
        spawn_food(food, WINDOW_WIDTH, WINDOW_HEIGHT);
        grow_snake(snake);
        game.score++;
      }

      // Draw the snake and the food
      draw_snake(snake, surface);
      draw_food(food, surface);

      // Update the window display
      SDL_UpdateWindowSurface( window );
      last_time = current_time;
    }
  }

  SDL_Log("Game over! Your score: %d", game.score);

  destroy_snake(snake);
  destroy_food(food);

  SDL_DestroySurface(surface);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
