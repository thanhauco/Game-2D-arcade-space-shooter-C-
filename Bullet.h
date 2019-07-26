#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"

class Bullet : public Entity {
public:
  Bullet(float x, float y, float vx, float vy, bool isPlayerBullet);

  void update(float deltaTime) override;
  void render(SDL_Renderer *renderer) override;

  bool isPlayerBullet() const { return playerBullet; }
  int getDamage() const { return damage; }

private:
  bool playerBullet;
  int damage;
  float lifetime;
  float trailTimer;
};

#endif // BULLET_H
