#include "Particle.h"

Particle::Particle(float x, float y, float vx, float vy, float life, float s,
                   SDL_Color c)
    : position(x, y), velocity(vx, vy), lifetime(life), maxLifetime(life),
      size(s), color(c), active(true) {}

void Particle::update(float deltaTime) {
  if (!active)
    return;

  // Update position
  position += velocity * deltaTime;

  // Apply drag
  velocity *= 0.98f;

  // Decrease lifetime
  lifetime -= deltaTime;
  if (lifetime <= 0) {
    active = false;
  }

  // Shrink over time
  float lifePercent = lifetime / maxLifetime;
  size *= (0.99f + 0.01f * lifePercent);
}

void Particle::render(SDL_Renderer *renderer) {
  if (!active)
    return;

  // Fade out over lifetime
  float lifePercent = lifetime / maxLifetime;
  Uint8 alpha = static_cast<Uint8>(color.a * lifePercent);

  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, alpha);

  int halfSize = static_cast<int>(size / 2);
  SDL_Rect rect = {static_cast<int>(position.x) - halfSize,
                   static_cast<int>(position.y) - halfSize,
                   static_cast<int>(size), static_cast<int>(size)};

  SDL_RenderFillRect(renderer, &rect);

  // Brighter core
  if (size > 3) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha / 2);
    int coreSize = static_cast<int>(size / 3);
    SDL_Rect core = {static_cast<int>(position.x) - coreSize / 2,
                     static_cast<int>(position.y) - coreSize / 2, coreSize,
                     coreSize};
    SDL_RenderFillRect(renderer, &core);
  }
}
