#include "Enemy.h"
#include "Bullet.h"
#include "Game.h"
#include "Player.h"
#include <cmath>

Enemy::Enemy(float x, float y, EnemyType t)
    : Entity(x, y, 30, 30), type(t), health(1), maxHealth(1), scoreValue(100),
      shootTimer(0.0f), shootCooldown(2.0f), animTimer(0.0f) {

  switch (type) {
  case EnemyType::Drifter:
    width = 30;
    height = 30;
    health = maxHealth = 1;
    scoreValue = 100;
    shootCooldown = 2.5f;
    color = {255, 150, 50, 255}; // Orange
    break;

  case EnemyType::Hunter:
    width = 35;
    height = 35;
    health = maxHealth = 2;
    scoreValue = 200;
    shootCooldown = 1.5f;
    color = {255, 50, 100, 255}; // Magenta
    break;

  case EnemyType::Bomber:
    width = 50;
    height = 45;
    health = maxHealth = 4;
    scoreValue = 500;
    shootCooldown = 3.0f;
    color = {150, 50, 255, 255}; // Purple
    break;
  }

  shootTimer = shootCooldown * 0.5f; // Start halfway to first shot
}

void Enemy::update(float deltaTime, Game &game) {
  animTimer += deltaTime;

  switch (type) {
  case EnemyType::Drifter:
    updateDrifter(deltaTime, game);
    break;
  case EnemyType::Hunter:
    updateHunter(deltaTime, game);
    break;
  case EnemyType::Bomber:
    updateBomber(deltaTime, game);
    break;
  }

  Entity::update(deltaTime);

  // Deactivate if off screen (bottom)
  if (position.y > 700) {
    active = false;
  }
}

void Enemy::updateDrifter(float deltaTime, Game &game) {
  // Simple downward drift with slight horizontal wobble
  velocity.y = 80.0f;
  velocity.x = std::sin(animTimer * 2.0f) * 30.0f;

  // Shoot occasionally
  shootTimer -= deltaTime;
  if (shootTimer <= 0) {
    shootTimer = shootCooldown;

    auto bullet = std::make_unique<Bullet>(position.x, position.y + height / 2,
                                           0, 250.0f, false);
    game.addBullet(std::move(bullet));
  }
}

void Enemy::updateHunter(float deltaTime, Game &game) {
  // Move downward initially
  velocity.y = 60.0f;

  // Track player horizontally if game is playing
  if (game.getState() == GameState::Playing) {
    // We need to get player position somehow
    // For now, move toward center with some tracking
    float targetX = game.getWidth() / 2.0f;
    float diff = targetX - position.x;
    velocity.x = diff * 0.5f;

    // Clamp horizontal speed
    if (velocity.x > 150)
      velocity.x = 150;
    if (velocity.x < -150)
      velocity.x = -150;
  }

  // Shoot more aggressively
  shootTimer -= deltaTime;
  if (shootTimer <= 0) {
    shootTimer = shootCooldown;

    // Shoot downward
    auto bullet = std::make_unique<Bullet>(position.x, position.y + height / 2,
                                           0, 300.0f, false);
    game.addBullet(std::move(bullet));
  }
}

void Enemy::updateBomber(float deltaTime, Game &game) {
  // Slow, steady descent
  velocity.y = 40.0f;
  velocity.x = std::sin(animTimer * 0.8f) * 20.0f;

  // Drop cluster bombs
  shootTimer -= deltaTime;
  if (shootTimer <= 0) {
    shootTimer = shootCooldown;

    // Drop 3 bullets in a spread
    for (int i = -1; i <= 1; i++) {
      auto bullet = std::make_unique<Bullet>(position.x + i * 15.0f,
                                             position.y + height / 2, i * 50.0f,
                                             200.0f, false);
      game.addBullet(std::move(bullet));
    }
  }
}

void Enemy::takeDamage(int amount) {
  health -= amount;
  if (health <= 0) {
    active = false;
  }
}

void Enemy::render(SDL_Renderer *renderer) {
  switch (type) {
  case EnemyType::Drifter:
    renderDrifter(renderer);
    break;
  case EnemyType::Hunter:
    renderHunter(renderer);
    break;
  case EnemyType::Bomber:
    renderBomber(renderer);
    break;
  }
}

void Enemy::renderDrifter(SDL_Renderer *renderer) {
  // Diamond shape using rectangles
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

  // Main body
  SDL_Rect body = {static_cast<int>(position.x - 12),
                   static_cast<int>(position.y - 12), 24, 24};
  SDL_RenderFillRect(renderer, &body);

  // Darker accent
  SDL_SetRenderDrawColor(renderer, color.r / 2, color.g / 2, color.b / 2, 255);
  SDL_Rect accent = {static_cast<int>(position.x - 6),
                     static_cast<int>(position.y - 6), 12, 12};
  SDL_RenderFillRect(renderer, &accent);
}

void Enemy::renderHunter(SDL_Renderer *renderer) {
  // Aggressive angular shape
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

  // Main body
  SDL_Rect body = {static_cast<int>(position.x - 15),
                   static_cast<int>(position.y - 12), 30, 24};
  SDL_RenderFillRect(renderer, &body);

  // Wings
  SDL_Rect leftWing = {static_cast<int>(position.x - 20),
                       static_cast<int>(position.y - 5), 8, 15};
  SDL_Rect rightWing = {static_cast<int>(position.x + 12),
                        static_cast<int>(position.y - 5), 8, 15};
  SDL_RenderFillRect(renderer, &leftWing);
  SDL_RenderFillRect(renderer, &rightWing);

  // Eye glow
  float pulse = 0.5f + 0.5f * std::sin(animTimer * 5.0f);
  SDL_SetRenderDrawColor(renderer, 255, static_cast<Uint8>(255 * pulse), 255,
                         255);
  SDL_Rect eye = {static_cast<int>(position.x - 3),
                  static_cast<int>(position.y - 8), 6, 6};
  SDL_RenderFillRect(renderer, &eye);
}

void Enemy::renderBomber(SDL_Renderer *renderer) {
  // Large, bulky shape
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

  // Main body
  SDL_Rect body = {static_cast<int>(position.x - 22),
                   static_cast<int>(position.y - 18), 44, 36};
  SDL_RenderFillRect(renderer, &body);

  // Top section
  SDL_SetRenderDrawColor(renderer, color.r + 30, color.g + 30, color.b, 255);
  SDL_Rect top = {static_cast<int>(position.x - 12),
                  static_cast<int>(position.y - 24), 24, 10};
  SDL_RenderFillRect(renderer, &top);

  // Bomb bays (bottom)
  SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
  for (int i = -1; i <= 1; i++) {
    SDL_Rect bay = {static_cast<int>(position.x + i * 12 - 4),
                    static_cast<int>(position.y + 14), 8, 8};
    SDL_RenderFillRect(renderer, &bay);
  }

  // Health indicator (for multi-hit enemies)
  if (maxHealth > 1) {
    float healthPercent = static_cast<float>(health) / maxHealth;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
    SDL_Rect healthBg = {static_cast<int>(position.x - 20),
                         static_cast<int>(position.y - 30), 40, 4};
    SDL_RenderFillRect(renderer, &healthBg);

    SDL_SetRenderDrawColor(renderer, 100, 255, 100, 255);
    SDL_Rect healthBar = {static_cast<int>(position.x - 20),
                          static_cast<int>(position.y - 30),
                          static_cast<int>(40 * healthPercent), 4};
    SDL_RenderFillRect(renderer, &healthBar);
  }
}
