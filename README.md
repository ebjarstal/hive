# Hive: C++ Board Game

A C++ implementation of the **Hive** board game, developed as a university project for the UTC LO21 course (autumn 2024). Includes a console interface and a Qt GUI with a hexagonal board.

## Features

- **8 piece types**: Queen (Reine), Grasshopper (Sauterelle), Spider (Araignée), Ant (Fourmi), Beetle (Scarabée), Ladybug (Coccinelle), Pill Bug (Cloporte), Mosquito (Moustique)
- **2 optional extensions**: Ladybug and Mosquito (disabled by default, enabled at game creation)
- **Two interfaces**: console (text) and Qt GUI (hexagonal board rendered with `QGraphicsScene`)
- **Two player modes**: Human vs Human, Human vs AI (minimax with alpha-beta pruning, depth 2)
- **Undo system**: each player has a configurable number of undo moves
- **Save / Load**: game state persisted to text files under `sauvegardes/`

## Prerequisites

| Tool | Version |
|------|---------|
| CMake | ≥ 3.16 |
| Qt | 5 (`qtbase5-dev`) or Qt 6 |
| C++ compiler | C++17 (GCC, Clang, or MSVC) |

## Build

### CMake (cross-platform)

```bash
cmake -B build -S .
cmake --build build
./build/Hive
```

With AddressSanitizer (to verify no memory errors):

```bash
cmake -B build-asan -S . -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined"
cmake --build build-asan
./build-asan/Hive
```

### MSBuild (Windows / Visual Studio 2022)

```
msbuild Hive.sln /p:Configuration=Debug /p:Platform=x64
x64\Debug\HiveDebut.exe
```

## Usage

On launch the program asks:

```
Interface console (0) ou interface Qt (1) ?
```

- **0: Console mode**: text prompts guide you through the game turn by turn.
- **1: Qt mode**: a graphical window opens; click pieces to place or move them.

In both modes you will be asked whether to load an existing save or start a new game, and how many undo moves each player gets.

## Architecture

| File / Class | Role |
|---|---|
| `partie.h/.cpp` | `Partie` — game state (board, players, history, turn counter) |
| `plateau.h/.cpp` | `Plateau` — 3-D grid `vector<vector<vector<Pion*>>>` (15×15×5) |
| `pions.h/.cpp` | `Pion` abstract base + 8 concrete subclasses; each implements `deplacementsPossibles()` |
| `joueur.h/.cpp` | `Joueur` abstract base; `JoueurHumain` (console); `JoueurIA` (minimax α-β) |
| `mouvement.h/.cpp` | `Mouvement` — piece + destination + optional callbacks |
| `command.h/.cpp` | Command pattern: `Command` base, `MouvementCommand` concrete |
| `gestionnaireCommand.h/.cpp` | Static helpers: `executeCommand`, `undoCommand` |
| `gestionnaireMouvements.h/.cpp` | Move validation, hive-connectivity check (`cassageRuche`), move generation |
| `gestionnairePions.h/.cpp` | Piece placement helpers (`setPion`, `deletePion`, `movePion`) |
| `gestionnaireVoisins.h/.cpp` | Hex-grid neighbour computation |
| `gestionnaireSauvegarde.h/.cpp` | Save / load game to `sauvegardes/<name>.txt` |
| `usine.h/.cpp` | `UsineDePions` factory — creates `Pion` instances by type string |
| `Constantes.h` | Board dimensions, piece counts, UI page indices |
| `FenetrePrincipale.h/.cpp` | `QMainWindow` with `QStackedWidget` (menu, game setup, in-game pages) |
| `Controleur.h/.cpp` | MVC controller connecting Qt signals to game logic |
| `VuePion.h/.cpp` | `QGraphicsPolygonItem` for one hexagonal piece |
| `VuePlateau.h/.cpp` | Qt board view |

## Game rules summary

1. Players alternate placing or moving pieces. White goes first.
2. The **Queen Bee must be placed by turn 4** (no other piece may move until she is placed).
3. The **hive must stay connected** at all times — you cannot move a piece if doing so splits it.
4. A player **loses when their Queen is completely surrounded** (6 occupied neighbours).
5. If both Queens are surrounded on the same move it is a draw.

Each piece type moves differently:
- **Queen**: slides 1 space
- **Beetle**: slides 1 space *or* climbs on top of other pieces
- **Grasshopper**: jumps over a straight line of pieces
- **Spider**: slides exactly 3 spaces along the hive
- **Ant**: slides any number of spaces around the hive
- **Ladybug** (extension): moves 3 spaces: 2 on top, 1 down
- **Pill Bug** (extension): slides 1 space; can move an adjacent piece on top and then off
- **Mosquito** (extension): copies the movement of any piece it touches

## Extensions

Two optional extensions can be activated at game creation:

| Code | Piece | Effect |
|------|-------|--------|
| `X` | Coccinelle (Ladybug) | Moves 2 on top then 1 down |
| `M` | Moustique (Mosquito) | Copies an adjacent piece's move type |

Both are **disabled by default**. In console mode you are prompted for each one. In Qt mode checkboxes appear on the game-creation screen.