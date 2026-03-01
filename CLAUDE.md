# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

A C++ implementation of the **Hive** board game (UTC LO21 course, autumn 2024) with two interfaces:
- Console interface (text-based)
- Qt GUI (hexagonal board rendered with `QGraphicsScene`)

## Build System

The project is built with **Visual Studio 2022** (MSVC v143, C++17) using Qt integration via MSBuild.

**Solution file:** `Hive.sln`
**Project file:** `HiveDebut/HiveDebut.vcxproj`
**Qt modules required:** `core`, `gui`, `widgets`, `designer`

To build from command line (Windows, with VS tools and Qt in PATH):
```
msbuild Hive.sln /p:Configuration=Debug /p:Platform=x64
```

**Note:** The code uses Windows-only headers (`<windows.h>`, `<direct.h>`) and is not cross-platform. The compiled binary is at `x64/Debug/HiveDebut.exe`.

There are no automated tests.

## Architecture

### Layered Structure

**Model layer** (`HiveDebut/`):
- `Plateau` — 3D grid (`vector<vector<vector<Pion*>>>`, dimensions 15×15×5 from `Constantes.h`) representing the board; a third axis (`z`) allows stacking (Scarab mechanic)
- `Pion` — abstract base class for all pieces; subclasses: `Reine`, `Sauterelle`, `Araignee`, `Fourmi`, `Scarabee`, `Coccinelle`, `Cloporte`, `Moustique`. Each implements `deplacementsPossibles()` for its movement rules
- `Partie` — game state: holds references to `Plateau`, two `Joueur*`, the move `historique` stack, and tour counter
- `Joueur` — abstract; subclasses `JoueurHumain` (console interaction) and `JoueurIA` (minimax with alpha-beta pruning)
- `Mouvement` — represents a single move (piece + destination coordinates)

**Factory:** `UsineDePions` — creates `Pion` instances by type string (`"R"`, `"S"`, `"A"`, `"F"`, `"K"`, `"X"`, `"C"`, `"M"`); tracks counts and enabled extensions

**Manager/service classes** (all static methods):
- `GestionnaireMouvements` — move validation, hive-connectivity checks (`cassageRuche`), generates all legal moves
- `GestionnairePions` — piece placement helpers
- `GestionnaireVoisins` — neighbor calculation on the hex grid
- `GestionnaireSauvegarde` — save/load game to text files under `sauvegardes/`; `liste_sauvegardes.txt` is the index
- `GestionnaireCommand` — executes/dispatches `Command` objects

**Command pattern** (`command.h`): `Command` is an abstract base with `execute()`/`undo()`; `MouvementCommand` wraps a `Mouvement`. The `historique` stack on `Partie` enables undo. Each `Joueur` has a `nbUndo` counter limiting undos.

**Qt view layer:**
- `VuePion` — `QObject` + `QGraphicsPolygonItem`; draws a hexagonal piece, emits `pionClique(VuePion*)` on click
- `VuePlateau` — the game board view
- `FenetrePrincipale` — `QMainWindow` with a `QStackedWidget` for pages (menu, new game, vs AI, 2-player, load, in-game). Page indices defined in `Constantes.h`
- `Controleur` — `QObject` acting as MVC controller; connects Qt signals from the view to game logic; owns `piocheJoueur1`/`piocheJoueur2` lists of `VuePion*`

### Key Constants (`Constantes.h`)

| Constant | Value | Meaning |
|---|---|---|
| `TAILLE_PLATEAU` | 15 | Board width and height |
| `NB_COUCHES` | 5 | Max stack height (z-axis) |
| `NB_PIONS_DEBUT` | 5 | Pieces per player at start |
| `SCALE_BY` | 20 | Pixel scale factor for hexagons |

### Extensions

Optional piece types (Coccinelle, Moustique) are disabled by default and activated via `UsineDePions::setExtensionActive()`. The Qt UI exposes checkboxes for these at game creation.

### Game Flow

At startup, `main.cpp` prompts for console (0) or Qt (1) mode.

**Console mode:** `Partie::setup()` → `partie.jouerUnTour(joueur)` loop, saving state each turn via `GestionnaireSauvegarde::sauvegarde()`.

**Qt mode:** `FenetrePrincipale` creates a `Controleur` which drives the game; `Controleur::jouerTour()` slot is the main turn handler; signals like `partieTerminee` propagate results back to the view.
