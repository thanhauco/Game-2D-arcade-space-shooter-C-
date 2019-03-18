#include "Entity.h"

Entity::Entity(float x, float y, float w, float h)
    : position(x, y), velocity(0, 0), width(w), height(h), active(true),
      color{255, 255, 255, 255} {}

void Entity::update(float deltaTime) { position += velocity * deltaTime; }

void Entity::render(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  SDL_Rect rect = getBoundingBox();
  SDL_RenderFillRect(renderer, &rect);
}

bool Entity::collidesWith(const Entity &other) const {
  if (!active || !other.active)
    return false;

  SDL_Rect a = getBoundingBox();
  SDL_Rect b = other.getBoundingBox();

  return SDL_HasIntersection(&a, &b);
}

SDL_Rect Entity::getBoundingBox() const {
  return SDL_Rect{static_cast<int>(position.x - width / 2),
                  static_cast<int>(position.y - height / 2),
                  static_cast<int>(width), static_cast<int>(height)};
}
