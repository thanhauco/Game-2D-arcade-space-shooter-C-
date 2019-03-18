#include "Player.h"
#include "Bullet.h"
#include "Game.h"
#include <cmath>

Player::Player(float x, float y)
    : Entity(x, y, 40, 50), speed(300.0f), shootCooldown(0.15f),
      shootTimer(0.0f), health(5), maxHealth(5), engineFlicker(0.0f) {

  color = {0, 200, 255, 255}; // Cyan player ship
}

void Player::update(float deltaTime, const Uint8 *keyState, Game &game) {
  handleInput(keyState);

  // Apply velocity
  Entity::update(deltaTime);

  // Keep player on screen
  clampToScreen(game.getWidth(), game.getHeight());

  // Update shoot timer
  if (shootTimer > 0) {
    shootTimer -= deltaTime;
  }

  // Shoot if space is pressed
  if (keyState[SDL_SCANCODE_SPACE] && shootTimer <= 0) {
    shoot(game);
    shootTimer = shootCooldown;
  }

  // Update engine flicker for visual effect
  engineFlicker += deltaTime * 10.0f;
}

void Player::handleInput(const Uint8 *keyState) {
  velocity = Vector2(0, 0);

  if (keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP]) {
    velocity.y = -speed;
  }
  if (keyState[SDL_SCANCODE_S] || keyState[SDL_SCANCODE_DOWN]) {
    velocity.y = speed;
  }
  if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT]) {
    velocity.x = -speed;
  }
  if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]) {
    velocity.x = speed;
  }

  // Normalize diagonal movement
  if (velocity.x != 0 && velocity.y != 0) {
    velocity = velocity.normalized() * speed;
  }
}

void Player::shoot(Game &game) {
  // Create bullet at player's position
  auto bullet = std::make_unique<Bullet>(position.x, position.y - height / 2, 0,
                                         -500.0f, // Shoot upward
                                         true     // Player bullet
  );
  game.addBullet(std::move(bullet));
}

void Player::clampToScreen(int screenWidth, int screenHeight) {
  float halfWidth = width / 2;
  float halfHeight = height / 2;

  if (position.x < halfWidth)
    position.x = halfWidth;
  if (position.x > screenWidth - halfWidth)
    position.x = screenWidth - halfWidth;
  if (position.y < halfHeight)
    position.y = halfHeight;
  if (position.y > screenHeight - halfHeight)
    position.y = screenHeight - halfHeight;
}

void Player::takeDamage(int amount) {
  health -= amount;
  if (health < 0)
    health = 0;
}

void Player::render(SDL_Renderer *renderer) {
  // Draw ship body (triangle-ish shape using rectangles)
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

  // Main body
  SDL_Rect body = {static_cast<int>(position.x - 15),
                   static_cast<int>(position.y - 20), 30, 40};
  SDL_RenderFillRect(renderer, &body);

  // Nose
  SDL_Rect nose = {static_cast<int>(position.x - 8),
                   static_cast<int>(position.y - 30), 16, 15};
  SDL_RenderFillRect(renderer, &nose);

  // Wings
  SDL_SetRenderDrawColor(renderer, 0, 150, 200, 255);
  SDL_Rect leftWing = {static_cast<int>(position.x - 25),
                       static_cast<int>(position.y), 12, 20};
  SDL_Rect rightWing = {static_cast<int>(position.x + 13),
                        static_cast<int>(position.y), 12, 20};
  SDL_RenderFillRect(renderer, &leftWing);
  SDL_RenderFillRect(renderer, &rightWing);

  // Engine glow (flickering)
  int glowIntensity = static_cast<int>(150 + 100 * std::sin(engineFlicker));
  SDL_SetRenderDrawColor(renderer, 255, glowIntensity, 50, 255);
  SDL_Rect engine = {static_cast<int>(position.x - 8),
                     static_cast<int>(position.y + 15), 16, 10};
  SDL_RenderFillRect(renderer, &engine);
}
