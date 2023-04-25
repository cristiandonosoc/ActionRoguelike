# Action Roguelike

## TODOs

- AI Minion
  - Minion projectiles stack up sometimes against characters.
  - Minion doesn't ragdoll when dying.
  - Minion still has collision when dead.
- Projectiles
  - Character collides against his own (replicated) projectile
- Console Manager
  - Needs to create interim actor to be able to function as a "channel".

## Possible Systems

### Statechart

Some code generation to embed state machines into the game.
This is one is bigger as it needs code generation and some work.

### Messaging System

Some way to be able to send arbitrary structs, mostly within actors, but could be expanded  to
other systems as well:
- Actor Components
- Subsystems

### Subsystem Dependency/Initialization Enforcement

Make sure that subsystems only start at the correct order with the given dependencies.

