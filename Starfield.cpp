#include "Starfield.h"
#include <random>

Starfield::Starfield(int width, int height)
    : screenWidth(width), screenHeight(height) {

  stars.resize(NUM_STARS);

  // Initialize stars at random positions
  for (auto &star : stars) {
    spawnStar(star, true);
  }
}

void Starfield::spawnStar(Star &star, bool randomY) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_real_distribution<float> xDist(0, 1);
  static std::uniform_real_distribution<float> yDist(0, 1);
  static std::uniform_real_distribution<float> speedDist(20, 150);
  static std::uniform_int_distribution<int> brightDist(80, 255);
  static std::uniform_int_distribution<int> sizeDist(1, 3);

  star.x = xDist(gen) * screenWidth;
  star.y = randomY ? yDist(gen) * screenHeight : -5;
  star.speed = speedDist(gen);
  star.brightness = brightDist(gen);
  star.size = sizeDist(gen);

  // Slower stars are dimmer (parallax effect)
  star.brightness = static_cast<int>(star.brightness * (star.speed / 150.0f));
}

void Starfield::update(float deltaTime) {
  for (auto &star : stars) {
    star.y += star.speed * deltaTime;

    // Respawn at top when off screen
    if (star.y > screenHeight + 10) {
      spawnStar(star, false);
    }
  }
}

void Starfield::render(SDL_Renderer *renderer) {
  for (const auto &star : stars) {
    // Tint based on brightness (slight blue tint for distant stars)
    int r = star.brightness;
    int g = star.brightness;
    int b = static_cast<int>(star.brightness * 1.1f);
    if (b > 255)
      b = 255;

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);

    if (star.size == 1) {
      // Single pixel for small stars
      SDL_RenderDrawPoint(renderer, static_cast<int>(star.x),
                          static_cast<int>(star.y));
    } else {
      // Rectangle for larger stars
      SDL_Rect rect = {static_cast<int>(star.x), static_cast<int>(star.y),
                       star.size, star.size};
      SDL_RenderFillRect(renderer, &rect);
    }
  }
}
