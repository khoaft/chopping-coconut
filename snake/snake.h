#include <SDL3/SDL.h>

enum Direction {
  UP,
  DOWN,
  LEFT,
  RIGHT
};

typedef struct SnakeSection {
  SDL_Rect rect;
  struct SnakeSection* next;
} SnakeSection;

typedef struct Snake {
  enum Direction direction;
  SnakeSection* head;
  unsigned int length;
} Snake;

extern Snake* new_snake(unsigned int length, unsigned int size, int x, int y);
extern void destroy_snake(Snake* snake);
extern void draw_snake(Snake* snake, SDL_Surface* surface);
extern bool move_snake(Snake* snake, unsigned int window_width, unsigned int window_height);
extern void grow_snake(Snake* snake);
