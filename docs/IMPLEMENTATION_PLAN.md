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

### Phase 7: Power-ups ✅

- [x] PowerUp Entity class
- [x] Drop system (enemies drop items)
- [x] Weapon upgrades (Spread Shot)
- [x] Health restoration items

### Phase 8: Boss Battle ✅

- [x] Boss Entity class (Multi-stage)
- [x] Boss AI patterns (Movement & Attacks)
- [x] Boss health bar (UI)
- [x] Boss spawn trigger (Score/Time based)

### Phase 9: Polish & Persistence ✅

- [x] High Score saving/loading (File I/O)
- [x] Shield Power-up (New type & logic)
- [x] Visual indication for Shield (Player overlay)

### Phase 10: Post-Release Maintenance ✅

- [x] Hotfix: Safe High Score loading
- [x] Polish: Boss UI color tweaks
- [x] Polish: Shield visual improvements

### Phase 11: Advanced Combat ✅

- [x] Screen Shake (Juice)
- [x] Charge Shot (New Mechanic)

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
