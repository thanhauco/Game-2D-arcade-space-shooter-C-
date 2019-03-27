#include "Game.h"
#include "Bullet.h"
#include "Enemy.h"
#include "HUD.h"
#include "Particle.h"
#include "Player.h"
#include "Starfield.h"
#include <iostream>

Game::Game()
    : window(nullptr), renderer(nullptr), running(false),
      state(GameState::Menu), score(0), combo(0), comboTimer(0.0f),
      enemySpawnTimer(0.0f), difficulty(1.0f), keyState(nullptr) {

  // Seed random number generator
  std::random_device rd;
  rng.seed(rd());
}

Game::~Game() { cleanup(); }

bool Game::init() {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize! Error: " << SDL_GetError()
              << std::endl;
    return false;
  }

  // Create window
  window = SDL_CreateWindow("Stellar Fury", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,
                            SDL_WINDOW_SHOWN);

  if (!window) {
    std::cerr << "Window could not be created! Error: " << SDL_GetError()
              << std::endl;
    return false;
  }

  // Create renderer with VSync
  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (!renderer) {
    std::cerr << "Renderer could not be created! Error: " << SDL_GetError()
              << std::endl;
    return false;
  }

  // Enable alpha blending
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  // Initialize systems
  starfield = std::make_unique<Starfield>(SCREEN_WIDTH, SCREEN_HEIGHT);
  hud = std::make_unique<HUD>();

  running = true;
  return true;
}

void Game::run() {
  Uint64 lastTime = SDL_GetPerformanceCounter();
  const Uint64 frequency = SDL_GetPerformanceFrequency();

  while (running) {
    // Calculate delta time
    Uint64 currentTime = SDL_GetPerformanceCounter();
    float deltaTime = static_cast<float>(currentTime - lastTime) / frequency;
    lastTime = currentTime;

    // Cap delta time to prevent spiral of death
    if (deltaTime > 0.05f) {
      deltaTime = 0.05f;
    }

    handleEvents();
    update(deltaTime);
    render();
  }
}

void Game::cleanup() {
  // Clear entities
  player.reset();
  enemies.clear();
  playerBullets.clear();
  enemyBullets.clear();
  particles.clear();
  starfield.reset();
  hud.reset();

  // Destroy SDL resources
  if (renderer) {
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
  }

  if (window) {
    SDL_DestroyWindow(window);
    window = nullptr;
  }

  SDL_Quit();
}

void Game::handleEvents() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      running = false;
      break;

    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        if (state == GameState::Playing) {
          state = GameState::Paused;
        } else if (state == GameState::Paused) {
          state = GameState::Playing;
        } else {
          running = false;
        }
      }

      if (event.key.keysym.sym == SDLK_RETURN) {
        if (state == GameState::Menu || state == GameState::GameOver) {
          startGame();
        }
      }
      break;
    }
  }

  // Get keyboard state for continuous input
  keyState = SDL_GetKeyboardState(nullptr);
}

void Game::update(float deltaTime) {
  // Always update starfield
  starfield->update(deltaTime);

  switch (state) {
  case GameState::Menu:
    updateMenu(deltaTime);
    break;
  case GameState::Playing:
    updatePlaying(deltaTime);
    break;
  case GameState::Paused:
    // Do nothing while paused
    break;
  case GameState::GameOver:
    updateGameOver(deltaTime);
    break;
  }
}

void Game::updateMenu(float deltaTime) {
  (void)deltaTime; // Unused for now
}

void Game::updatePlaying(float deltaTime) {
  // Update player
  if (player) {
    player->update(deltaTime, keyState, *this);

    // Check if player is dead
    if (player->getHealth() <= 0) {
      endGame();
      return;
    }
  }

  // Spawn enemies
  enemySpawnTimer -= deltaTime;
  if (enemySpawnTimer <= 0) {
    spawnEnemy();
    // Base spawn rate of 3 seconds, decreasing with difficulty
    enemySpawnTimer = 3.0f / difficulty;
  }

  // Update enemies
  for (auto &enemy : enemies) {
    enemy->update(deltaTime, *this);
  }

  // Update player bullets
  for (auto &bullet : playerBullets) {
    bullet->update(deltaTime);
  }

  // Update enemy bullets
  for (auto &bullet : enemyBullets) {
    bullet->update(deltaTime);
  }

  // Update particles
  for (auto &particle : particles) {
    particle->update(deltaTime);
  }

  // Check collisions - player bullets vs enemies
  for (auto &bullet : playerBullets) {
    if (!bullet->isActive())
      continue;

    for (auto &enemy : enemies) {
      if (!enemy->isActive())
        continue;

      if (bullet->collidesWith(*enemy)) {
        bullet->setActive(false);
        enemy->takeDamage(1);

        if (!enemy->isActive()) {
          // Enemy destroyed
          addScore(enemy->getScoreValue());
          createExplosion(enemy->getX(), enemy->getY(), 20,
                          {255, 150, 50, 255});
        }
      }
    }
  }

  // Check collisions - enemy bullets vs player
  if (player) {
    for (auto &bullet : enemyBullets) {
      if (!bullet->isActive())
        continue;

      if (bullet->collidesWith(*player)) {
        bullet->setActive(false);
        player->takeDamage(1);
        createExplosion(player->getX(), player->getY(), 10,
                        {255, 100, 100, 255});
      }
    }

    // Check collisions - enemies vs player
    for (auto &enemy : enemies) {
      if (!enemy->isActive())
        continue;

      if (enemy->collidesWith(*player)) {
        enemy->setActive(false);
        player->takeDamage(2);
        createExplosion(enemy->getX(), enemy->getY(), 25, {255, 200, 50, 255});
      }
    }
  }

  // Update combo timer
  if (combo > 0) {
    comboTimer -= deltaTime;
    if (comboTimer <= 0) {
      combo = 0;
    }
  }

  // Remove inactive entities
  playerBullets.erase(
      std::remove_if(playerBullets.begin(), playerBullets.end(),
                     [](const auto &b) { return !b->isActive(); }),
      playerBullets.end());

  enemyBullets.erase(
      std::remove_if(enemyBullets.begin(), enemyBullets.end(),
                     [](const auto &b) { return !b->isActive(); }),
      enemyBullets.end());

  enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                               [](const auto &e) { return !e->isActive(); }),
                enemies.end());

  particles.erase(std::remove_if(particles.begin(), particles.end(),
                                 [](const auto &p) { return !p->isActive(); }),
                  particles.end());

  // Increase difficulty over time
  difficulty += deltaTime * 0.01f;
  if (difficulty > 5.0f)
    difficulty = 5.0f;
}

void Game::updateGameOver(float deltaTime) {
  (void)deltaTime; // Unused for now
}

void Game::render() {
  // Clear screen with dark background
  SDL_SetRenderDrawColor(renderer, 10, 10, 20, 255);
  SDL_RenderClear(renderer);

  // Render starfield (always visible)
  starfield->render(renderer);

  switch (state) {
  case GameState::Menu:
    renderMenu();
    break;
  case GameState::Playing:
  case GameState::Paused:
    renderPlaying();
    if (state == GameState::Paused) {
      // Draw pause overlay
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
      SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
      SDL_RenderFillRect(renderer, &overlay);
    }
    break;
  case GameState::GameOver:
    renderPlaying(); // Show last frame
    renderGameOver();
    break;
  }

  SDL_RenderPresent(renderer);
}

void Game::renderMenu() {
  // Draw title (simple rectangle placeholder)
  SDL_SetRenderDrawColor(renderer, 0, 200, 255, 255);
  SDL_Rect titleRect = {SCREEN_WIDTH / 2 - 150, 150, 300, 60};
  SDL_RenderFillRect(renderer, &titleRect);

  // Draw "Press ENTER" indicator
  SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
  SDL_Rect startRect = {SCREEN_WIDTH / 2 - 100, 350, 200, 30};
  SDL_RenderFillRect(renderer, &startRect);
}

void Game::renderPlaying() {
  // Render particles (behind everything)
  for (auto &particle : particles) {
    particle->render(renderer);
  }

  // Render bullets
  for (auto &bullet : playerBullets) {
    bullet->render(renderer);
  }
  for (auto &bullet : enemyBullets) {
    bullet->render(renderer);
  }

  // Render enemies
  for (auto &enemy : enemies) {
    enemy->render(renderer);
  }

  // Render player
  if (player) {
    player->render(renderer);
  }

  // Render HUD
  if (hud && player) {
    hud->render(renderer, score, combo, player->getHealth(),
                player->getMaxHealth());
  }
}

void Game::renderGameOver() {
  // Semi-transparent overlay
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
  SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  SDL_RenderFillRect(renderer, &overlay);

  // Game Over text (placeholder rectangle)
  SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
  SDL_Rect gameOverRect = {SCREEN_WIDTH / 2 - 120, 200, 240, 50};
  SDL_RenderFillRect(renderer, &gameOverRect);

  // Score display (placeholder)
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_Rect scoreRect = {SCREEN_WIDTH / 2 - 80, 280, 160, 30};
  SDL_RenderFillRect(renderer, &scoreRect);

  // Restart prompt
  SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
  SDL_Rect restartRect = {SCREEN_WIDTH / 2 - 100, 350, 200, 25};
  SDL_RenderFillRect(renderer, &restartRect);
}

void Game::startGame() {
  // Reset game state
  score = 0;
  combo = 0;
  comboTimer = 0.0f;
  enemySpawnTimer = 1.0f;
  difficulty = 1.0f;

  // Clear entities
  enemies.clear();
  playerBullets.clear();
  enemyBullets.clear();
  particles.clear();

  // Create player
  player = std::make_unique<Player>(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 80.0f);

  state = GameState::Playing;
}

void Game::endGame() {
  state = GameState::GameOver;
  createExplosion(player->getX(), player->getY(), 50, {255, 200, 100, 255});
}

void Game::addScore(int points) {
  combo++;
  comboTimer = 2.0f; // Reset combo timer
  score += points * combo;
}

void Game::spawnEnemy() {
  float x = randomFloat(50, SCREEN_WIDTH - 50);
  float y = -50;

  // Choose enemy type based on difficulty
  int type = randomInt(0, 2);

  auto enemy = std::make_unique<Enemy>(x, y, static_cast<EnemyType>(type));
  enemies.push_back(std::move(enemy));
}

void Game::addBullet(std::unique_ptr<Bullet> bullet) {
  if (bullet->isPlayerBullet()) {
    playerBullets.push_back(std::move(bullet));
  } else {
    enemyBullets.push_back(std::move(bullet));
  }
}

void Game::addParticle(std::unique_ptr<Particle> particle) {
  particles.push_back(std::move(particle));
}

void Game::createExplosion(float x, float y, int count, SDL_Color color) {
  for (int i = 0; i < count; i++) {
    float angle = randomFloat(0, 2.0f * 3.14159f);
    float speed = randomFloat(50, 200);
    float vx = std::cos(angle) * speed;
    float vy = std::sin(angle) * speed;
    float lifetime = randomFloat(0.3f, 0.8f);
    float size = randomFloat(2, 6);

    auto particle =
        std::make_unique<Particle>(x, y, vx, vy, lifetime, size, color);
    particles.push_back(std::move(particle));
  }
}

float Game::randomFloat(float min, float max) {
  std::uniform_real_distribution<float> dist(min, max);
  return dist(rng);
}

int Game::randomInt(int min, int max) {
  std::uniform_int_distribution<int> dist(min, max);
  return dist(rng);
}
