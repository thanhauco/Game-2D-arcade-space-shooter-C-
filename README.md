# Stellar Fury 🚀

A fast-paced 2D arcade space shooter built with C++17 and SDL2.

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![SDL2](https://img.shields.io/badge/SDL-2.0-green.svg)
![License](https://img.shields.io/badge/License-MIT-yellow.svg)

## Features

- **Retro Neon Aesthetic** - Vibrant colors and glow effects
- **Smooth 60 FPS Gameplay** - Delta-time based physics
- **Multiple Enemy Types** - Drifters, Hunters, and Bombers
- **Particle Effects** - Explosions, engine trails, bullet impacts
- **Score System** - With combo multipliers
- **Parallax Starfield** - Multi-layer depth background

## Screenshots

_Coming soon_

## Requirements

- C++17 compatible compiler (clang++ or g++)
- SDL2 library

### Installing SDL2

**macOS:**

```bash
brew install sdl2
```

**Ubuntu/Debian:**

```bash
sudo apt-get install libsdl2-dev
```

**Windows:**
Download from [SDL2 releases](https://github.com/libsdl-org/SDL/releases)

## Building

```bash
make clean && make
```

## Running

```bash
./stellar_fury
```

## Controls

| Key   | Action        |
| ----- | ------------- |
| W / ↑ | Move Up       |
| S / ↓ | Move Down     |
| A / ← | Move Left     |
| D / → | Move Right    |
| Space | Shoot         |
| ESC   | Pause/Quit    |
| Enter | Start/Restart |

## Game Mechanics

### Enemy Types

1. **Drifter** - Floats down slowly, fires occasionally
2. **Hunter** - Actively tracks and chases the player
3. **Bomber** - Large enemy that drops cluster bombs

### Scoring

- Drifter: 100 points
- Hunter: 200 points
- Bomber: 500 points
- Combo multiplier increases with consecutive kills

## Project Structure

```
stellar_fury/
├── main.cpp          # Entry point
├── Game.h/cpp        # Core game loop and state management
├── Vector2.h         # 2D vector math
├── Entity.h/cpp      # Base entity class
├── Player.h/cpp      # Player ship
├── Enemy.h/cpp       # Enemy types
├── Bullet.h/cpp      # Projectile system
├── Particle.h/cpp    # Particle effects
├── Starfield.h/cpp   # Background starfield
├── HUD.h/cpp         # Heads-up display
└── Makefile          # Build configuration
```

## License

MIT License - See LICENSE file for details.

## Author

Thanh Vu (thanhauco@gmail.com)

---

_Built with ❤️ in 2019_
