enum State {
  RUNNING,
  QUIT,
  GAME_OVER
};

typedef struct Game {
  enum State state;
  unsigned int score;
} Game;
