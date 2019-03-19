#ifndef ENTITY_H
#define ENTITY_H

#include "Vector2.h"
#include <SDL2/SDL.h>

class Entity {
public:
  Entity(float x, float y, float width, float height);
  virtual ~Entity() = default;

  virtual void update(float deltaTime);
  virtual void render(SDL_Renderer *renderer);

  // Collision detection
  bool collidesWith(const Entity &other) const;
  SDL_Rect getBoundingBox() const;

  // Position
  float getX() const { return position.x; }
  float getY() const { return position.y; }
  void setPosition(float x, float y) { position = Vector2(x, y); }
  Vector2 getPosition() const { return position; }

  // Velocity
  Vector2 getVelocity() const { return velocity; }
  void setVelocity(float vx, float vy) { velocity = Vector2(vx, vy); }

  // State
  bool isActive() const { return active; }
  void setActive(bool a) { active = a; }

  // Size
  float getWidth() const { return width; }
  float getHeight() const { return height; }

protected:
  Vector2 position;
  Vector2 velocity;
  float width;
  float height;
  bool active;
  SDL_Color color;
};

#endif // ENTITY_H
