# Stellar Fury - Implementation Plan

## Overview

A 2D space shooter game built with C++17 and SDL2.

## Phases

### Phase 1: Foundation

- [x] Project setup (git, Makefile)
- [ ] Core game loop (Game class)
- [ ] Vector2 math utilities

### Phase 2: Core Entities

- [ ] Entity base class
- [ ] Player ship implementation
- [ ] Bullet/projectile system

### Phase 3: Enemies & Combat

- [ ] Enemy base class
- [ ] Drifter enemy type
- [ ] Hunter enemy type
- [ ] Bomber enemy type
- [ ] Collision detection

### Phase 4: Effects & Polish

- [ ] Particle system
- [ ] Explosions
- [ ] Starfield background

### Phase 5: UI & Game States

- [ ] HUD (score, health)
- [ ] Menu state
- [ ] Game over state

## Technical Specs

| Spec       | Value   |
| ---------- | ------- |
| Language   | C++17   |
| Graphics   | SDL2    |
| Resolution | 800x600 |
| Target FPS | 60      |

## Dependencies

- SDL2 core only (no SDL2_image, SDL2_ttf, SDL2_mixer)
- All graphics procedurally drawn
