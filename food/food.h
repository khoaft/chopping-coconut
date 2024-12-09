#include <SDL3/SDL.h>

typedef struct Food {
  int x;
  int y;
  unsigned int size;
} Food;

extern Food* new_food(unsigned int size);
extern void destroy_food(Food* food);
extern void draw_food(Food* snake, SDL_Surface* surface);
extern void spawn_food(Food* food, unsigned int window_width, unsigned int window_height);
