#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <memory>
#include <random>
#include <vector>

// Forward declarations
class Player;
class Enemy;
class Bullet;
class Particle;
class PowerUp;
class Starfield;
class HUD;

enum class GameState { Menu, Playing, Paused, GameOver };

class Game {
public:
  Game();
  ~Game();

  bool init();
  void run();
  void cleanup();

  // Getters
  SDL_Renderer *getRenderer() const { return renderer; }
  int getWidth() const { return SCREEN_WIDTH; }
  int getHeight() const { return SCREEN_HEIGHT; }
  GameState getState() const { return state; }
  int getScore() const { return score; }
  int getCombo() const { return combo; }

  // Game actions
  void addScore(int points);
  void spawnEnemy();
  void addBullet(std::unique_ptr<Bullet> bullet);
  void addParticle(std::unique_ptr<Particle> particle);
  void spawnPowerUp(float x, float y);
  void createExplosion(float x, float y, int count, SDL_Color color);

  // Random number generation
  float randomFloat(float min, float max);
  int randomInt(int min, int max);

private:
  void handleEvents();
  void update(float deltaTime);
  void render();

  void updateMenu(float deltaTime);
  void updatePlaying(float deltaTime);
  void updateGameOver(float deltaTime);

  void renderMenu();
  void renderPlaying();
  void renderGameOver();

  void startGame();
  void endGame();

  // Constants
  static const int SCREEN_WIDTH = 800;
  static const int SCREEN_HEIGHT = 600;
  static const int TARGET_FPS = 60;

  // SDL
  SDL_Window *window;
  SDL_Renderer *renderer;
  bool running;

  // Game state
  GameState state;
  int score;
  int combo;
  float comboTimer;
  float enemySpawnTimer;
  float difficulty;

  // Entities
  std::unique_ptr<Player> player;
  std::vector<std::unique_ptr<Enemy>> enemies;
  std::vector<std::unique_ptr<Bullet>> playerBullets;
  std::vector<std::unique_ptr<Bullet>> enemyBullets;
  std::vector<std::unique_ptr<PowerUp>> powerups;
  std::vector<std::unique_ptr<Particle>> particles;

  // Systems
  std::unique_ptr<Starfield> starfield;
  std::unique_ptr<HUD> hud;

  // Random
  std::mt19937 rng;

  // Input state
  const Uint8 *keyState;
};

#endif // GAME_H
