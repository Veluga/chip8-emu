Chip-8 Emulator in C++.

# Unimplemented
* Sound
* Remapping controls
* 0xFX0A opcode (await keypress)

# Known bug
* When playing Pong, collision detection doesn't work reliably and thus a ball may register as not caught / caught when caught / not caught. From what I can see, it doesn't seem to be a bug in the 0xDXYN opcode implementation as that seems to be working just fine for other games.