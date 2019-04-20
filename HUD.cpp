#include "HUD.h"
#include <string>

HUD::HUD() : displayedScore(0), scoreAnimTimer(0.0f) {}

void HUD::render(SDL_Renderer *renderer, int score, int highScore, int combo,
                 int health, int maxHealth, int bossHealth, int maxBossHealth) {
  // Animate score
  if (displayedScore < score) {
    int diff = score - displayedScore;
    displayedScore += (diff / 10) + 1;
  }

  renderScore(renderer, displayedScore);
  renderHighScore(renderer, highScore);
  renderCombo(renderer, combo);
  renderHealthBar(renderer, health, maxHealth);

  if (bossHealth > 0 && maxBossHealth > 0) {
    renderBossHealth(renderer, bossHealth, maxBossHealth);
  }
}

void HUD::renderScore(SDL_Renderer *renderer, int score) {
  // Draw score bar (simulate text)
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_Rect scoreRect = {20, 20, score / 100, 20}; // Width based on score
  if (scoreRect.w > 200)
    scoreRect.w = 200; // Cap width
  SDL_RenderFillRect(renderer, &scoreRect);
}

void HUD::renderHighScore(SDL_Renderer *renderer, int highScore) {
  if (highScore <= 0)
    return;

  // Draw high score marker/bar
  SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255); // Gold
  SDL_Rect highScoreRect = {20, 45, highScore / 100, 5};
  if (highScoreRect.w > 200)
    highScoreRect.w = 200;
  SDL_RenderFillRect(renderer, &highScoreRect);
}

void HUD::renderCombo(SDL_Renderer *renderer, int combo) {
  if (combo > 1) {
    SDL_SetRenderDrawColor(renderer, 255, 200, 50, 255);
    for (int i = 0; i < combo; i++) {
      SDL_Rect comboRect = {20 + i * 15, 50, 10, 10};
      SDL_RenderFillRect(renderer, &comboRect);
    }
  }
}

void HUD::renderHealthBar(SDL_Renderer *renderer, int health, int maxHealth) {
  int barWidth = 200;
  int barHeight = 20;
  int x = 800 - barWidth - 20; // Right aligned
  int y = 20;

  // Background
  SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
  SDL_Rect bgRect = {x, y, barWidth, barHeight};
  SDL_RenderFillRect(renderer, &bgRect);

  // Health
  float healthPercent = (float)health / maxHealth;
  int currentWidth = (int)(barWidth * healthPercent);

  SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255); // Red
  SDL_Rect healthRect = {x, y, currentWidth, barHeight};
  SDL_RenderFillRect(renderer, &healthRect);
}

void HUD::renderBossHealth(SDL_Renderer *renderer, int health, int maxHealth) {
  int barWidth = 600;
  int barHeight = 20;
  int x = (800 - barWidth) / 2; // Center aligned
  int y = 50;                   // Below standard HUD

  // Background
  SDL_SetRenderDrawColor(renderer, 50, 0, 0, 255); // Dark red bg
  SDL_Rect bgRect = {x, y, barWidth, barHeight};
  SDL_RenderFillRect(renderer, &bgRect);

  // Health
  float healthPercent = (float)health / maxHealth;
  int currentWidth = (int)(barWidth * healthPercent);

  SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255); // Orange/Red
  SDL_Rect healthRect = {x, y, currentWidth, barHeight};
  SDL_RenderFillRect(renderer, &healthRect);
}
