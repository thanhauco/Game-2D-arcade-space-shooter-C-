#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

class Game;

class Player : public Entity {
public:
  Player(float x, float y);

  void update(float deltaTime, const Uint8 *keyState, Game &game);
  void render(SDL_Renderer *renderer) override;

  void takeDamage(int amount);

  int getHealth() const { return health; }
  int getMaxHealth() const { return maxHealth; }

  void upgradeWeapon();
  void heal(int amount);
  int getWeaponLevel() const { return weaponLevel; }

private:
  void handleInput(const Uint8 *keyState);
  void shoot(Game &game);
  void clampToScreen(int screenWidth, int screenHeight);

  float speed;
  float shootCooldown;
  float shootTimer;
  int health;
  int maxHealth;
  int weaponLevel;

  // Visual
  float engineFlicker;
};

#endif // PLAYER_H
