# Stellar Fury - Test Plan

## Overview

This document outlines the testing strategy for Stellar Fury, covering unit tests, integration tests, and manual gameplay testing.

---

## 1. Unit Tests

### 1.1 Vector2 Math Tests

| Test Case       | Input         | Expected Output |
| --------------- | ------------- | --------------- |
| Addition        | (1,2) + (3,4) | (4,6)           |
| Subtraction     | (5,5) - (2,3) | (3,2)           |
| Scalar Multiply | (2,3) \* 2    | (4,6)           |
| Magnitude       | (3,4)         | 5.0             |
| Normalize       | (3,4)         | (0.6, 0.8)      |
| Dot Product     | (1,0) · (0,1) | 0               |

### 1.2 Entity Tests

| Test Case       | Description                                   | Expected               |
| --------------- | --------------------------------------------- | ---------------------- |
| Position Update | Entity at (0,0) with velocity (10,10), dt=1.0 | Position = (10,10)     |
| Boundary Box    | Entity at (100,100), size (20,30)             | Rect = (90,85,20,30)   |
| Collision True  | Overlapping entities                          | collidesWith() = true  |
| Collision False | Non-overlapping entities                      | collidesWith() = false |

### 1.3 Player Tests

| Test Case      | Description               | Expected              |
| -------------- | ------------------------- | --------------------- |
| Move Right     | D key pressed             | velocity.x = 300      |
| Move Diagonal  | W+D keys pressed          | velocity normalized   |
| Shoot          | Space pressed, cooldown=0 | Bullet created        |
| Shoot Cooldown | Space pressed, cooldown>0 | No bullet             |
| Take Damage    | takeDamage(1)             | health decreases by 1 |
| Death          | health = 0                | isActive check fails  |

### 1.4 Enemy Tests

| Test Case        | Description                | Expected             |
| ---------------- | -------------------------- | -------------------- |
| Drifter Movement | Update with dt=1.0         | Moves downward +80px |
| Hunter Movement  | Update near player         | Tracks toward player |
| Bomber Cluster   | Shoot timer expires        | 3 bullets spawned    |
| Enemy Damage     | takeDamage(1) on 2HP enemy | health = 1           |

---

## 2. Integration Tests

### 2.1 Collision System

```
Test: Player hit by enemy bullet
Setup: Player at (400, 500), Enemy bullet at (400, 500)
Action: Run collision check
Expected:
  - Bullet deactivated
  - Player health reduced
  - Explosion particles created
```

```
Test: Player bullet destroys enemy
Setup: Enemy at (300, 200) with 1 HP, Player bullet at (300, 200)
Action: Run collision check
Expected:
  - Bullet deactivated
  - Enemy deactivated
  - Score increased by enemy value
  - Combo incremented
```

### 2.2 Game State Transitions

```
Test: Menu to Playing
Precondition: state = Menu
Action: Press ENTER
Expected: state = Playing, player initialized

Test: Playing to Paused
Precondition: state = Playing
Action: Press ESC
Expected: state = Paused

Test: Playing to GameOver
Precondition: state = Playing, player.health = 0
Action: Update game
Expected: state = GameOver, explosion at player position
```

### 2.3 Spawn System

```
Test: Enemy spawning over time
Setup: difficulty = 1.0, enemySpawnTimer = 0
Action: Update game
Expected:
  - New enemy added to enemies vector
  - Timer reset to 3.0 seconds
```

---

## 3. Manual Testing Checklist

### 3.1 Startup Tests

- [ ] Game window opens at 800x600
- [ ] Starfield background visible
- [ ] Menu screen displayed
- [ ] No console errors

### 3.2 Menu State

- [ ] Starfield animates in background
- [ ] ENTER key starts game
- [ ] ESC key exits application

### 3.3 Gameplay Tests

#### Controls

- [ ] W / ↑ moves player up
- [ ] S / ↓ moves player down
- [ ] A / ← moves player left
- [ ] D / → moves player right
- [ ] SPACE fires bullets
- [ ] Diagonal movement normalized (same speed)
- [ ] Player confined to screen bounds

#### Combat

- [ ] Bullets travel upward
- [ ] Bullets destroy enemies on contact
- [ ] Enemy bullets damage player
- [ ] Enemy collision damages player
- [ ] Explosions appear on death
- [ ] Score increases when killing enemies
- [ ] Combo system works (multiplier increases)

#### Enemies

- [ ] Drifter floats down with wobble
- [ ] Drifter fires single shots
- [ ] Hunter tracks toward center
- [ ] Hunter fires more frequently
- [ ] Bomber moves slowly
- [ ] Bomber drops 3-bullet spread

#### Visual Effects

- [ ] Player engine flickers
- [ ] Star parallax (distant stars slower)
- [ ] Particle explosions fade over time
- [ ] Health bar updates correctly
- [ ] Score display updates
- [ ] Combo indicator shows on hit

### 3.4 Game Over State

- [ ] Game over screen appears
- [ ] Final score displayed
- [ ] Large explosion on player death
- [ ] ENTER restarts game
- [ ] New game resets score and health

### 3.5 Performance Tests

- [ ] Maintains 60 FPS with 10+ enemies
- [ ] Maintains 60 FPS with 50+ particles
- [ ] No memory leaks after 5+ restarts
- [ ] No visual stuttering

---

## 4. Edge Cases

| Scenario                          | Expected Behavior        |
| --------------------------------- | ------------------------ |
| Rapid fire + many enemies         | Frame rate stable        |
| Player at corner + diagonal input | Player stays in bounds   |
| Multiple bullets hit same enemy   | Only first registers     |
| 0 HP enemy receiving damage       | No crash, stays inactive |
| Very high score (>1M)             | Display handles overflow |

---

## 5. Regression Tests

After any code change, verify:

1. All controls responsive
2. Collision detection accurate
3. Score/combo working
4. No memory leaks
5. 60 FPS maintained

---

## Test Execution Log

| Date       | Tester   | Version | Pass/Fail | Notes           |
| ---------- | -------- | ------- | --------- | --------------- |
| 2019-03-29 | Thanh Vu | v1.0.0  | Pass      | Initial release |
