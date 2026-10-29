# TheDayBefore

Open-world survival FPS extraction game built with Unreal Engine 5.8.

## Project Structure

```
TheDayBefore/
├── Source/                          # C++ source code
│   └── TheDayBefore/
│       ├── Characters/              # Character classes
│       │   ├── TDBCharacter.h/.cpp  # Base FPS character
│       ├── Camera/                  # Camera systems
│       │   ├── TDBFirstPersonCameraComponent.h/.cpp  # ADS camera
│       ├── Tactical/                # Tactical mechanics
│       │   ├── TDBLeanComponent.h/.cpp      # Lean (Q/E)
│       │   └── TDBBreathComponent.h/.cpp    # Hold breath (Shift)
│       └── GameFramework/           # Game framework
│           ├── TDBGameMode.h/.cpp           # Game rules
│           └── TDBPlayerController.h/.cpp   # Input handling
├── Content/                         # UE assets
│   ├── Characters/                  # Character meshes, animations
│   ├── Weapons/                     # Weapon models, sounds
│   ├── Maps/                        # Level files
│   ├── UI/                          # UI widgets, textures
│   ├── Input/                       # Input mappings, actions
│   ├── Audio/                       # Sound effects, music
│   ├── VFX/                         # Particle effects
│   └── DataTables/                  # Game data tables
├── Config/                          # Configuration files
│   ├── DefaultEngine.ini            # Engine settings
│   ├── DefaultGame.ini              # Game settings
│   ├── DefaultInput.ini             # Input configuration
│   └── DefaultEditor.ini            # Editor settings
└── Docs/                            # Documentation
    ├── GDD.md                       # Game Design Document
    └── TechnicalSpec.md             # Technical Specification
```

## Key Features

- **First-Person Perspective**: Full FPS camera system with ADS (Aim Down Sights)
- **Tactical Mechanics**: Lean, breath control, crouch/prone stances
- **Survival Systems**: Health, stamina, hunger, thirst
- **Inventory System**: Grid-based inventory like EFT/Tarkov
- **Open World**: Large-scale world with multiple biomes
- **Multiplayer**: Dedicated server architecture

## Development Setup

### Prerequisites
- Unreal Engine 5.8
- Visual Studio 2022 (Windows)

### Building
1. Open `TheDayBefore.uproject` in Unreal Engine
2. Generate Visual Studio project files
3. Build in Development Editor configuration

### Controls
- **WASD**: Movement
- **Mouse**: Look/Aim
- **Shift**: Hold breath (while aiming)
- **Q/E**: Lean left/right
- **C**: Crouch/Prone
- **Space**: Jump
- **E**: Interact

## Documentation

- [Game Design Document](Docs/GDD.md)
- [Technical Specification](Docs/TechnicalSpec.md)

## Repository

Private repository: https://github.com/zxDOOMxz/The-Day-Before
