#include "HUD.h"

HUD::HUD() : displayedScore(0), scoreAnimTimer(0.0f) {}

void HUD::render(SDL_Renderer *renderer, int score, int combo, int health,
                 int maxHealth) {
  renderHealthBar(renderer, health, maxHealth);
  renderScore(renderer, score);

  if (combo > 1) {
    renderCombo(renderer, combo);
  }

  // Animate score counting up
  if (displayedScore < score) {
    int diff = score - displayedScore;
    displayedScore += (diff / 10) + 1;
    if (displayedScore > score)
      displayedScore = score;
  }
}

void HUD::renderHealthBar(SDL_Renderer *renderer, int health, int maxHealth) {
  int barWidth = 150;
  int barHeight = 16;
  int x = 20;
  int y = 20;

  // Background
  SDL_SetRenderDrawColor(renderer, 50, 50, 50, 200);
  SDL_Rect bg = {x - 2, y - 2, barWidth + 4, barHeight + 4};
  SDL_RenderFillRect(renderer, &bg);

  // Health bar background (dark red)
  SDL_SetRenderDrawColor(renderer, 100, 30, 30, 255);
  SDL_Rect healthBg = {x, y, barWidth, barHeight};
  SDL_RenderFillRect(renderer, &healthBg);

  // Health bar fill
  float healthPercent = static_cast<float>(health) / maxHealth;
  int fillWidth = static_cast<int>(barWidth * healthPercent);

  // Color based on health level
  if (healthPercent > 0.6f) {
    SDL_SetRenderDrawColor(renderer, 50, 200, 100, 255); // Green
  } else if (healthPercent > 0.3f) {
    SDL_SetRenderDrawColor(renderer, 255, 200, 50, 255); // Yellow
  } else {
    SDL_SetRenderDrawColor(renderer, 255, 80, 80, 255); // Red
  }

  SDL_Rect healthFill = {x, y, fillWidth, barHeight};
  SDL_RenderFillRect(renderer, &healthFill);

  // Health bar shine
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 50);
  SDL_Rect shine = {x, y, fillWidth, barHeight / 3};
  SDL_RenderFillRect(renderer, &shine);

  // Health segments
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
  for (int i = 1; i < maxHealth; i++) {
    int segX = x + (barWidth * i / maxHealth);
    SDL_RenderDrawLine(renderer, segX, y, segX, y + barHeight);
  }

  // Border
  SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
  SDL_Rect border = {x - 1, y - 1, barWidth + 2, barHeight + 2};
  SDL_RenderDrawRect(renderer, &border);
}

void HUD::renderScore(SDL_Renderer *renderer, int score) {
  (void)score; // Use displayedScore instead for animation

  // Score display area (top right)
  int x = 800 - 180;
  int y = 20;

  // Background
  SDL_SetRenderDrawColor(renderer, 30, 30, 50, 200);
  SDL_Rect bg = {x, y, 160, 35};
  SDL_RenderFillRect(renderer, &bg);

  // Border
  SDL_SetRenderDrawColor(renderer, 100, 150, 255, 255);
  SDL_RenderDrawRect(renderer, &bg);

  // Score visualization (bar representing score magnitude)
  // Each segment = 1000 points
  int segments = displayedScore / 1000;
  if (segments > 14)
    segments = 14;

  SDL_SetRenderDrawColor(renderer, 0, 200, 255, 255);
  for (int i = 0; i < segments; i++) {
    SDL_Rect seg = {x + 8 + i * 10, y + 8, 8, 18};
    SDL_RenderFillRect(renderer, &seg);
  }

  // Partial segment for remainder
  int remainder = (displayedScore % 1000) / 100;
  if (segments < 14 && remainder > 0) {
    SDL_SetRenderDrawColor(renderer, 0, 200, 255,
                           static_cast<Uint8>(150 + remainder * 10));
    SDL_Rect partSeg = {x + 8 + segments * 10, y + 8, 8, 18};
    SDL_RenderFillRect(renderer, &partSeg);
  }
}

void HUD::renderCombo(SDL_Renderer *renderer, int combo) {
  // Combo display (center top)
  int x = 400;
  int y = 25;

  // Combo glow based on combo level
  int glowSize = 30 + combo * 5;
  if (glowSize > 80)
    glowSize = 80;

  SDL_SetRenderDrawColor(renderer, 255, 200, 50, 50);
  SDL_Rect glow = {x - glowSize / 2, y - 10, glowSize, 30};
  SDL_RenderFillRect(renderer, &glow);

  // Combo indicator boxes
  int numBoxes = combo;
  if (numBoxes > 10)
    numBoxes = 10;

  int boxWidth = 12;
  int totalWidth = numBoxes * (boxWidth + 2);
  int startX = x - totalWidth / 2;

  for (int i = 0; i < numBoxes; i++) {
    // Rainbow-ish colors for high combos
    Uint8 r = static_cast<Uint8>(255 - i * 15);
    Uint8 g = static_cast<Uint8>(150 + i * 10);
    Uint8 b = static_cast<Uint8>(50 + i * 20);

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_Rect box = {startX + i * (boxWidth + 2), y, boxWidth, 15};
    SDL_RenderFillRect(renderer, &box);
  }

  // "x" multiplier indicator
  if (combo > 1) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
    // Simple "x" shape
    SDL_RenderDrawLine(renderer, x + totalWidth / 2 + 8, y + 2,
                       x + totalWidth / 2 + 18, y + 12);
    SDL_RenderDrawLine(renderer, x + totalWidth / 2 + 18, y + 2,
                       x + totalWidth / 2 + 8, y + 12);
  }
}
