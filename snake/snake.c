#include <stdlib.h>
#include "snake.h"

Snake* new_snake(unsigned int length, unsigned int size, int x, int y) {
  Snake* snake = (Snake*)malloc(sizeof(Snake));
  snake->direction = RIGHT;
  snake->length = length;

  SnakeSection* head = (SnakeSection*)malloc(sizeof(SnakeSection));
  SDL_Rect rect = { x, y, size, size };
  head->rect = rect;
  head->next = NULL;
  snake->head = head;

  SnakeSection* current = head;
  for (int i = 1; i < length; i++) {
    SnakeSection* section = (SnakeSection*)malloc(sizeof(SnakeSection));
    SDL_Rect rect = { x - i * size, y, size, size };
    section->rect = rect;
    section->next = NULL;
    current->next = section;
    current = section;
  }

  return snake;
}

void destroy_snake(Snake* snake) {
  SnakeSection* current = snake->head;
  while (current != NULL) {
    SnakeSection* next = current->next;
    free(current);
    current = next;
  }

  free(snake);
}

void draw_snake(Snake* snake, SDL_Surface* surface) {
  Uint32 color = SDL_MapSurfaceRGB(surface, 255, 255, 255);
  SnakeSection* current = snake->head;
  while (current != NULL) {
    SDL_FillSurfaceRect(surface, &current->rect, color);
    current = current->next;
  }
}

bool move_snake(Snake* snake, unsigned int window_width, unsigned int window_height) {
  int x = snake->head->rect.x;
  int y = snake->head->rect.y;

  switch(snake->direction) {
    case UP:
      snake->head->rect.y -= snake->head->rect.h;
      break;
    case DOWN:
      snake->head->rect.y += snake->head->rect.h;
      break;
    case LEFT:
      snake->head->rect.x -= snake->head->rect.w;
      break;
    case RIGHT:
      snake->head->rect.x += snake->head->rect.w;
      break;
  }

  SnakeSection* current = snake->head->next;
  while (current != NULL) {
    int tmp = current->rect.x;
    current->rect.x = x;
    x = tmp;

    tmp = current->rect.y;
    current->rect.y = y;
    y = tmp;

    if (snake->head->rect.x == current->rect.x && snake->head->rect.y == current->rect.y) {
      return false;
    }

    current = current->next;
  }

  if (snake->head->rect.x < 0 || snake->head->rect.x >= window_width || snake->head->rect.y < 0 || snake->head->rect.y >= window_height) {
    return false;
  }

  return true;
}

void grow_snake(Snake* snake) {
  SnakeSection* current = snake->head;
  while (current->next != NULL) {
    current = current->next;
  }

  SDL_Rect rect = { current->rect.x, current->rect.y, current->rect.w, current->rect.h };
  SnakeSection* section = (SnakeSection*)malloc(sizeof(SnakeSection));
  section->rect = rect;
  section->next = NULL;
  current->next = section;
}
