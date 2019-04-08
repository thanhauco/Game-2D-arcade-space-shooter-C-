#include "PowerUp.h"
#include <cmath>

PowerUp::PowerUp(float x, float y, PowerUpType t)
    : Entity(x, y, 20, 20), type(t), blinkTimer(0.0f) {

  width = 20;
  height = 20;

  // Slow downward drift
  velocity.y = 50.0f;
  velocity.x = 0.0f;

  // Set colors based on type
  switch (type) {
  case PowerUpType::WeaponUpgrade:
    color = {0, 255, 255, 255}; // Cyan
    break;
  case PowerUpType::HealthRestore:
    color = {50, 255, 50, 255}; // Green
    break;
  }
}

void PowerUp::update(float deltaTime) {
  // Update blinking effect
  blinkTimer += deltaTime * 10.0f;

  // Pulse size slightly
  float pulse = 1.0f + 0.2f * std::sin(blinkTimer * 0.5f);
  int pulsedSize = static_cast<int>(20 * pulse);

  // Move down
  Entity::update(deltaTime);

  // Deactivate if off screen
  if (position.y > 600) { // Screen height assumption
    active = false;
  }
}

void PowerUp::render(SDL_Renderer *renderer) {
  // Draw outer glow/pulse
  float pulse = 0.5f + 0.5f * std::sin(blinkTimer);
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b,
                         static_cast<Uint8>(100 * pulse));

  SDL_Rect glowRect = {
      static_cast<int>(position.x - 5), static_cast<int>(position.y - 5),
      static_cast<int>(width + 10), static_cast<int>(height + 10)};
  SDL_RenderFillRect(renderer, &glowRect);

  // Draw solid center
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
  SDL_Rect rect = {static_cast<int>(position.x), static_cast<int>(position.y),
                   static_cast<int>(width), static_cast<int>(height)};
  SDL_RenderFillRect(renderer, &rect);

  // Draw symbol inside
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  if (type == PowerUpType::WeaponUpgrade) {
    // "W" shape (simplified as 2 lines)
    SDL_RenderDrawLine(renderer, position.x + 5, position.y + 5,
                       position.x + 10, position.y + 15);
    SDL_RenderDrawLine(renderer, position.x + 10, position.y + 15,
                       position.x + 15, position.y + 5);
  } else {
    // "+" shape
    SDL_RenderDrawLine(renderer, position.x + 10, position.y + 5,
                       position.x + 10, position.y + 15);
    SDL_RenderDrawLine(renderer, position.x + 5, position.y + 10,
                       position.x + 15, position.y + 10);
  }
}
