# Tragic: A Card Game in C

A turn-based card game for two players, written in C and played from the
terminal. Players build a deck, summon creatures, cast effect cards, and
battle until one wizard's life points reach zero.

## About

Tragic is a lightweight, Magic-inspired card game built as a standalone C
program. The focus of the project was game-state management in plain C: a reusable game library (`gamelib`) separated from the
program entry point (`main`), with no external dependencies.

## Gameplay & design

- **Deck building.** Each player assembles a deck of 6 to 60 cards. The two
  players don't need equal deck sizes.
- **Tiered card generation.** Card health is assigned by draw position so the
  early game and late game feel different: the last cards drawn (created
  first) have 7–8 HP, the first cards drawn have 1–3 HP, and everything in
  between has 4–6 HP. A player's wizard class can then raise the HP of certain
  card types above these baselines.
- **Combat.** A player may attack once per turn, regardless of how many
  creatures they have in play.
- **Healing rules.** A "heal" card cannot take a player above the starting
  20 life points, and a creature cannot exceed 12 HP.
- **Persistent effect cards.** Every non-creature card whose effect isn't
  fully consumed returns to the player's hand instead of being discarded.

## Project structure

```
.
├── main.c        # Entry point and game loop
├── gamelib.c     # Game logic: deck generation, combat, card effects
├── gamelib.h     # Public interface of the game library
└── .gitignore
```

## Build & run

```bash
gcc -Wall -o tragic main.c gamelib.c
./tragic
```

