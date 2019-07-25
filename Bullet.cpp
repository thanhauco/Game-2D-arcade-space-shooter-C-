#include "Bullet.h"

Bullet::Bullet(float x, float y, float vx, float vy, bool isPlayer)
    : Entity(x, y, 6, 12), playerBullet(isPlayer), damage(1), lifetime(3.0f),
      trailTimer(0.0f) {

  velocity = Vector2(vx, vy);

  if (playerBullet) {
    color = {0, 255, 200, 255}; // Cyan for player
  } else {
    color = {255, 100, 100, 255}; // Red for enemy
  }
}

void Bullet::update(float deltaTime) {
  Entity::update(deltaTime);

  // Decrease lifetime
  lifetime -= deltaTime;
  if (lifetime <= 0) {
    active = false;
  }

  // Deactivate if off screen
  if (position.y < -20 || position.y > 700 || position.x < -20 ||
      position.x > 900) {
    active = false;
  }

  trailTimer += deltaTime;
}

void Bullet::render(SDL_Renderer *renderer) {
  // Draw bullet glow (larger, semi-transparent)
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 100);
  SDL_Rect glow = {static_cast<int>(position.x - 5),
                   static_cast<int>(position.y - 8), 10, 16};
  SDL_RenderFillRect(renderer, &glow);

  // Draw bullet core
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
  SDL_Rect core = {static_cast<int>(position.x - 3),
                   static_cast<int>(position.y - 6), 6, 12};
  SDL_RenderFillRect(renderer, &core);

  // Draw bright center
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
  SDL_Rect center = {static_cast<int>(position.x - 1),
                     static_cast<int>(position.y - 4), 2, 8};
  SDL_RenderFillRect(renderer, &center);
}
