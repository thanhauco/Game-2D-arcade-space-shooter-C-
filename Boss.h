#pragma once

#include "Enemy.h"
#include <SDL2/SDL.h>

enum class BossPhase {
  Phase1, // Standard movement + spread shot
  Phase2  // Aggressive movement + rapid fire / summon
};

class Boss : public Enemy {
public:
  Boss(float x, float y, Game &game);

  void update(float deltaTime, Game &game) override;
  void render(SDL_Renderer *renderer) override;

  int getMaxHealth() const { return maxHealth; }

private:
  void updatePhase1(float deltaTime, Game &game);
  void updatePhase2(float deltaTime, Game &game);

  BossPhase phase;
  int maxHealth;
  float phaseTimer;
  float moveTimer;
  float attackTimer;
  int attackPattern; // 0, 1, or 2
};
