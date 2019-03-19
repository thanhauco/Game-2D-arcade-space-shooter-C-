#include "Game.h"
#include <iostream>

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  std::cout << "=== Stellar Fury ===" << std::endl;
  std::cout << "A 2D Space Shooter" << std::endl;
  std::cout << std::endl;

  Game game;

  if (!game.init()) {
    std::cerr << "Failed to initialize game!" << std::endl;
    return 1;
  }

  std::cout << "Controls:" << std::endl;
  std::cout << "  WASD/Arrows - Move" << std::endl;
  std::cout << "  Space       - Shoot" << std::endl;
  std::cout << "  ESC         - Pause/Quit" << std::endl;
  std::cout << "  Enter       - Start/Restart" << std::endl;
  std::cout << std::endl;

  game.run();

  std::cout << "Thanks for playing!" << std::endl;

  return 0;
}
