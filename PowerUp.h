#pragma once

#include "Entity.h"
#include <SDL2/SDL.h>

enum class PowerUpType {
    WeaponUpgrade,
    HealthRestore
};

class PowerUp : public Entity {
public:
    PowerUp(float x, float y, PowerUpType type);
    
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;
    
    PowerUpType getType() const { return type; }
    
private:
    PowerUpType type;
    float blinkTimer;
    SDL_Color color;
};
