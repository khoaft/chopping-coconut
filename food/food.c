#include <stdlib.h>
#include "food.h"

Food* new_food(unsigned int size) {
  Food* food = (Food*)malloc(sizeof(Food));
  food->x = 0;
  food->y = 0;
  food->size = size;
  return food;
}

void destroy_food(Food* food) {
  free(food);
}

void draw_food(Food* food, SDL_Surface* surface) {
  Uint32 color = SDL_MapSurfaceRGB(surface, 255, 165, 0);
  SDL_Rect rect = { food->x, food->y, food->size, food->size };
  SDL_FillSurfaceRect(surface, &rect, color);
}

void spawn_food(Food* food, unsigned int window_width, unsigned int window_height) {
  food->x = (rand() % (window_width / food->size)) * food->size;
  food->y = (rand() % (window_height / food->size)) * food->size;
}
