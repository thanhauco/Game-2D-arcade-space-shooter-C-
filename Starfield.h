#ifndef STARFIELD_H
#define STARFIELD_H

#include <SDL2/SDL.h>
#include <vector>

struct Star {
  float x, y;
  float speed;
  int brightness;
  int size;
};

class Starfield {
public:
  Starfield(int width, int height);

  void update(float deltaTime);
  void render(SDL_Renderer *renderer);

private:
  void spawnStar(Star &star, bool randomY = true);

  std::vector<Star> stars;
  int screenWidth;
  int screenHeight;
  static const int NUM_STARS = 150;
};

#endif // STARFIELD_H
