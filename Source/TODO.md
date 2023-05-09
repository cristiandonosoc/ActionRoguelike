# Action Roguelike

## TODOs

### Attributes

- On Killed calculation is repeated twice, both in the server and in the OnRep.
    - We should centralize that calculation.

### AI Minion

- Minion doesn't ragdoll when dying.
- Minion still has collision when dead.

### Console Manager

- Needs to create interim actor to be able to function as a "channel".

### Actions

- Auto start doesn't wait until the action is replicated before sending the start RPC.
- Effect attempts to damage the player when it's already deleted, causing an assert.

### GameMode

- Player Character doesn't unspawn when reviving (keeps there).
- UI doesn't correctly reset when reviving (likely tied to the character).

### Messaging

- Do Channel/NetChannel binding, to ensure the correct underlying UChannel is correctly bound to the
  appropiate MessageChannel.
- Make Channel send the message back to their owner.
- Add UChannel name to DefaultEngine.ini, so we can have our messaging type.

### Tom Looman

- Go back to watch the lessons.

## Possible Systems

### Statechart

Some code generation to embed state machines into the game.
This is one is bigger as it needs code generation and some work.

### Subsystem Dependency/Initialization Enforcement

Make sure that subsystems only start at the correct order with the given dependencies.

