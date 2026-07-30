# Space War

A space shooter game where you pilot a spaceship against CPU-controlled enemy ships.
Built in C as a way to explore game development closer to the metal.

## Description

Space War is a 2D space combat game built with [raylib](https://www.raylib.com/) and C23.
The player controls a Viper-class spaceship, navigating the screen and firing pulse
cannons while staying within the battlefield boundaries.

Built entirely in C without a game engine, the project focuses on understanding game
loop fundamentals, frame-independent movement, and manual resource management through
a simple, modular codebase.

### Current features

- Player movement with arrow keys
- Shooting mechanic (space key)
- Boundary collision for player and bullets
- Frame-independent movement using delta time

### Planned features

- CPU-controlled enemy ships with basic AI
- Bullet-to-enemy collision detection
- Scoring system
- Multiple spaceship types

## Build

### Dependencies

- [raylib](https://www.raylib.com/) (tested with raylib 5.x)
- GCC (or any C23-compatible compiler)
- [just](https://github.com/casey/just) (command runner)

### Compiling and running

```sh
just build
just run
```

## License

Space War is open-sourced software licensed under the [MIT license](LICENSE).
