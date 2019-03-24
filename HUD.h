#ifndef HUD_H
#define HUD_H

#include <SDL2/SDL.h>

class HUD {
public:
  HUD();

  void render(SDL_Renderer *renderer, int score, int combo, int health,
              int maxHealth);

private:
  void renderHealthBar(SDL_Renderer *renderer, int health, int maxHealth);
  void renderScore(SDL_Renderer *renderer, int score);
  void renderCombo(SDL_Renderer *renderer, int combo);

  // Animation
  int displayedScore;
  float scoreAnimTimer;
};

#endif // HUD_H
