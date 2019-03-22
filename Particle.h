#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector2.h"
#include <SDL2/SDL.h>

class Particle {
public:
  Particle(float x, float y, float vx, float vy, float lifetime, float size,
           SDL_Color color);

  void update(float deltaTime);
  void render(SDL_Renderer *renderer);

  bool isActive() const { return active; }

private:
  Vector2 position;
  Vector2 velocity;
  float lifetime;
  float maxLifetime;
  float size;
  SDL_Color color;
  bool active;
};

#endif // PARTICLE_H
