#include "Boss.h"
#include "Bullet.h"
#include "Game.h"
#include <cmath>

Boss::Boss(float x, float y, Game &game)
    : Enemy(x, y, EnemyType::Bomber, game), // Use Bomber as base type for now
      phase(BossPhase::Phase1), maxHealth(100), phaseTimer(0.0f),
      moveTimer(0.0f), attackTimer(0.0f), attackPattern(0) {

  width = 100;
  height = 80;
  health = maxHealth;
  scoreValue = 5000;
  speed = 100.0f;

  // Override base initialization
  velocity.x = speed;
  velocity.y = 0;
}

void Boss::update(float deltaTime, Game &game) {
  // Phase transition check
  if (phase == BossPhase::Phase1 && health < maxHealth / 2) {
    phase = BossPhase::Phase2;
    game.createExplosion(position.x, position.y, 50,
                         {255, 50, 50, 255}); // Angry explosion
    speed *= 1.5f;                            // Enrage speed
  }

  if (phase == BossPhase::Phase1) {
    updatePhase1(deltaTime, game);
  } else {
    updatePhase2(deltaTime, game);
  }

  // Screen clamping (don't leave screen)
  if (position.x < width / 2 && velocity.x < 0)
    velocity.x = abs(velocity.x);
  if (position.x > game.getWidth() - width / 2 && velocity.x > 0)
    velocity.x = -abs(velocity.x);

  position.x += velocity.x * deltaTime;
  position.y += velocity.y * deltaTime;
}

void Boss::updatePhase1(float deltaTime, Game &game) {
  // Move side to side
  moveTimer += deltaTime;
  if (moveTimer > 3.0f) {
    moveTimer = 0;
    // Sometimes stop to fire
    if (game.randomInt(0, 2) == 0)
      velocity.x = 0;
    else
      velocity.x = (game.randomInt(0, 1) == 0 ? speed : -speed);
  }

  // Attack logic
  attackTimer -= deltaTime;
  if (attackTimer <= 0) {
    attackTimer = 1.0f;

    // Spread Shot
    for (int i = -2; i <= 2; i++) {
      auto bullet =
          std::make_unique<Bullet>(position.x + i * 10, position.y + height / 2,
                                   i * 30.0f, // X spread
                                   200.0f,    // Y speed
                                   false);
      game.addBullet(std::move(bullet));
    }
  }
}

void Boss::updatePhase2(float deltaTime, Game &game) {
  // More aggressive movement (sine wave ish)
  moveTimer += deltaTime;
  velocity.y = std::sin(moveTimer * 2.0f) * 50.0f;

  if (velocity.x == 0)
    velocity.x = speed; // Always move inside side-to-side

  // Rapid Fire
  attackTimer -= deltaTime;
  if (attackTimer <= 0) {
    attackTimer = 0.5f;

    // Aimed shot at player
    // (Simplified aiming - just general direction)
    for (int i = 0; i < 5; i++) {
      auto bullet = std::make_unique<Bullet>(
          position.x + game.randomFloat(-40, 40), position.y + height / 2,
          game.randomFloat(-50, 50), 300.0f, false);
      game.addBullet(std::move(bullet));
    }
  }
}

void Boss::render(SDL_Renderer *renderer) {
  // Red ominous body
  SDL_SetRenderDrawColor(renderer, 150, 0, 0, 255);
  SDL_Rect body = {static_cast<int>(position.x - width / 2),
                   static_cast<int>(position.y - height / 2),
                   static_cast<int>(width), static_cast<int>(height)};
  SDL_RenderFillRect(renderer, &body);

  // Core (changes color based on phase)
  if (phase == BossPhase::Phase1)
    SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255); // Orange
  else
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Bright Red (Enraged)

  SDL_Rect core = {static_cast<int>(position.x - 20),
                   static_cast<int>(position.y - 20), 40, 40};
  SDL_RenderFillRect(renderer, &core);

  // "Cannons"
  SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
  SDL_Rect leftCannon = {static_cast<int>(position.x - width / 2 - 10),
                         static_cast<int>(position.y), 20, 40};
  SDL_Rect rightCannon = {static_cast<int>(position.x + width / 2 - 10),
                          static_cast<int>(position.y), 20, 40};
  SDL_RenderFillRect(renderer, &leftCannon);
  SDL_RenderFillRect(renderer, &rightCannon);
}
