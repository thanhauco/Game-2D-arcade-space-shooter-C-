# Stellar Fury - Implementation Plan

## Overview

A 2D space shooter game built with C++17 and SDL2.

## Phases

### Phase 1: Foundation ✅

- [x] Project setup (git, Makefile)
- [x] Core game loop (Game class)
- [x] Vector2 math utilities

### Phase 2: Core Entities ✅

- [x] Entity base class
- [x] Player ship implementation
- [x] Bullet/projectile system

### Phase 3: Enemies & Combat ✅

- [x] Enemy base class
- [x] Drifter enemy type
- [x] Hunter enemy type
- [x] Bomber enemy type
- [x] Collision detection

### Phase 4: Effects & Polish ✅

- [x] Particle system
- [x] Explosions
- [x] Starfield background

### Phase 5: UI & Game States ✅

- [x] HUD (score, health)
- [x] Menu state
- [x] Game over state

### Phase 6: Bug Fixes ✅

- [x] Issue #1: Player boundary clipping
- [x] Issue #2: Spawn rate too aggressive
- [x] Issue #3: Combo timer too short
- [x] Issue #4: Difficulty scaling
- [x] Issue #5: Hunter tracking
- [x] Issue #6: Particle limit
- [x] Issue #7: Score display
- [x] Issue #8: Enemy bullet speed

### Phase 7: Power-ups (In Progress)

- [ ] PowerUp Entity class
- [ ] Drop system (enemies drop items)
- [ ] Weapon upgrades (Spread Shot)
- [ ] Health restoration items

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
