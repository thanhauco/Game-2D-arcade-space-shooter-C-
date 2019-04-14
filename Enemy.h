#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"

class Game;

enum class EnemyType {
  Drifter, // Floats down, fires occasionally
  Hunter,  // Tracks player, aggressive
  Bomber   // Large, drops cluster bombs
};

class Enemy : public Entity {
public:
  Enemy(float x, float y, EnemyType type, Game &game);

  void update(float deltaTime, Game &game);
  void render(SDL_Renderer *renderer) override;

  void takeDamage(int amount);

  int getScoreValue() const { return scoreValue; }
  EnemyType getType() const { return type; }

protected:
  // Base update methods (can be used by derived classes or standard types)
  void updateDrifter(float deltaTime, Game &game);
  void updateHunter(float deltaTime, Game &game);
  void updateBomber(float deltaTime, Game &game);

  void renderDrifter(SDL_Renderer *renderer);
  void renderHunter(SDL_Renderer *renderer);
  void renderBomber(SDL_Renderer *renderer);

  EnemyType type;
  int health;
  int maxHealth;
  int scoreValue;
  float speed;
  float shootTimer;
  float shootCooldown;
  float animTimer;
};

#endif // ENEMY_H
