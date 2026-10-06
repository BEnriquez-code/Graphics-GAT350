**My Game Engine**

# Game Engine

A custom 2D component-based game engine written in **C++20**, built from scratch with a focus on modularity and performance.

## Features

- **Component-Based Architecture** — Actors are composed of reusable components rather than relying on deep inheritance hierarchies, making it easy to mix and match behavior.
- **Audio System** — Powered by [FMOD](https://www.fmod.com/) for sound effects and music playback.
- **Input System** — Unified handling of keyboard, mouse, and/or controller input.
- **Physics System** — Built on [Box2D](https://box2d.org/) for rigid body dynamics and collision detection/response.
- **Rendering System** — Built on [SDL3](https://www.libsdl.org/) for 2D rendering, including sprites, text, and particle effects.
- **Serialization** — Uses [rapidjson](https://rapidjson.org/) to load game data, scenes, and tilemaps from JSON.
- **Tilemap Support** — Loads tilemaps exported from [Tiled](https://www.mapeditor.org/).
- **Demo Game** — A included sample project demonstrating actors, components, tilemaps, and UI in action.

## Tech Stack

| Purpose        | Library     |
|-----------------|-------------|
| Language        | C++20       |
| Rendering       | SDL3        |
| Audio           | FMOD        |
| Physics         | Box2D       |
| Serialization   | rapidjson   |

## Architecture Overview

The engine follows a component-based design:

- **Actors** act as containers that hold a collection of components and represent any object in the game world.
- **Components** implement individual pieces of behavior (rendering, physics, input handling, audio, etc.) and can be attached to any actor.
- **Systems** (audio, input, physics, rendering) operate over components each frame, driving the core engine loop.

## Demo Game

The repository includes a demo game built with the engine, showcasing:

- Actors and component composition
- Tilemap loading and rendering
- UI elements
- Physics and input integration

## Getting Started

### Prerequisites

- A C++20-compatible compiler
- [SDL3](https://www.libsdl.org/)
- [FMOD](https://www.fmod.com/)
- [Box2D](https://box2d.org/)
- [rapidjson](https://rapidjson.org/)

### Building

```bash
# Clone the repository
git clone https://github.com/BEnriquez-code/GameEngine-GAT150.git

# Configure and build (adjust for your build system)
cmake -B build -S .
cmake --build build
```

### Running the Demo

```bash
./build/demo
```

## Project Structure

```
├── engine/          # Core engine source (audio, input, physics, rendering)
├── demo/            # Demo game showcasing engine features
├── assets/          # Tilemaps, textures, audio, and other game assets
└── README.md
```
"# Graphics-GAT350" 
