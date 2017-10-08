# life-sdl
A simple implementation of Conway's Game of Life made using SDL2.


## Screenshots
(coming soon)

## Dependencies
- [SDL2](https://www.libsdl.org/download-2.0.php)
- Some C++ compiler. (The included makefile uses g++.)
- Linux
    * There is a windows version of SDL2 so a port wouldn't take more than like 20 minutes. You'd just have to change a few things then compile with mingw as I use the getopt library. Also take out the color formatting stuff otherwise you won't get terminal output in CMD.

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
* g
    * Toggles between rendering shadows under live nodes and displaying a grid.
* s
    * This will save the game state to a config file.
* r
    * This resets the board to the preset you ran the game with if there is one.

## Config Files
Config files can set pretty much every aspect of how the game runs.
Here is the basic structure:
```ini
[variables]
# some positive int
rows = 50
cols = 50
node_width = 10
init_ups = 10
# must be bools (either 0 or 1)
grid_on = 0
verbose = 0



# hexadecimal rgba (i.e. 0xRRGGBBAA)
[colors]
node_fg    = 0x5c719eff
cursor_fg  = 0x748bb1ff
grid       = 0x313c4eff
shadow     = 0x313c4eff
default_bg = 0x2F343Fff
paused_bg  = 0x252932ff



# this bit is just a grid of 0's and 1's

# you should avoid putting text after [board] as im not sure what it will do
# but it will probably just treat any character other than 0 as 1 - I havn't tested it yet

# if the line is shorter than the width of the board then it will just fill the empty space with 0's
# so you can save yourself some trouble and just use newlines instead of rows with just 0's in it.

# this is a square. dont let monospace fonts trick you
[board]
11111111111111
10000000000001
10000000000001
10000000000001
10000000000001
10000000000001
10000000000001
10000000000001
10000000000001
10000000000001
10000000000001
10000000000001
10000000000001
11111111111111
```