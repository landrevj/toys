# life-sdl
A simple implementation of Conway's Game of Life made using SDL2.


## Screenshots
(coming soon)

## Dependencies
- [SDL2](https://www.libsdl.org/download-2.0.php)
- Some C++ compiler. (The included makefile uses g++.)

## Keybinds
* q
    * Quits the game.
* Space
    * Pauses/Unpauses the game.
    * If hit when the game updates per second is zero it will reset the ups to the default.
* Click and/or drag, t, and Enter
    *  They will all toggle the life of a cell. The first is based off the mouse position and the last two off of the in-game cursor's position.
    * Clicking will move the cursor to the clicked cell.
* Up, Down, Left, and Right keys
    * These will move the in-game cursor around.
* Backspace
    * This clears the board of living cells.
* [ and ]
    * These will respectively decrease and increase the number of times the game updates the board per second.
    * If you decrease to and from zero the game will pause/unpause.
* \
    * This will reset the updates per second to whatever you started the game with, be it as a command line argument or in a config file.
* .
    * When paused this will step the simulation by one update.
* p
    * This will enable paint mode, which will toggle cell life under the cursor as it moves.
* s
    * This will save the game state to a config file.
* r
    * This resets the board to the preset you ran the game with if there is one.