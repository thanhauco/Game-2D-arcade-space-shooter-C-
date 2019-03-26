# Issue #1: Player can partially exit screen boundaries

## Description

The player ship can partially move outside the visible screen area when moving quickly near the edges. The boundary check doesn't account for the ship's full dimensions.

## Steps to Reproduce

1. Move player ship to the left edge at full speed
2. Notice the ship partially clips outside the screen

## Expected Behavior

Player should be fully contained within screen boundaries.

## Priority

Medium

## Status

Closed - Fixed in commit 097d9c1
