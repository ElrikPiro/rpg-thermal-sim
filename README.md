# rpg-thermal-sim
A thermal simulator to control the spreading of temperature and fire on a RPG setup. Personal project. The application is intended to use as an aid for the dungeon master on a tabletop RPG (i.e. Dungeons and Dragons, Pathfinder, Stormbringer, Hawkmoon, ...) accelerating calculations on room temperature, fire spreading and smoke presence on a grid.

## Instalation

### Windows
Download [the Windows 64 bits executable](../../raw/master/x86_64%20win64/rpg-thermal-sim.exe) and launch it, it's compiled and tested on a fresh instalation of windows 10 but must work on any Windows 8 or higher with a x86_64 architecture (almost every common computer on market)

Alternatively you can compile the code by yourself with **Cygwin**, **MinGW** or **cl**_(Visual Studio)_ the only flag needed to make it work in most cases is `-std=c++11`

### Linux
Clone or [download](https://github.com/ElrikPiro/rpg-thermal-sim/archive/master.zip) the repository and compile the contents of [the src directory](https://github.com/ElrikPiro/rpg-thermal-sim/tree/master/src) with the command:
```
g++ -std=c++11 -o rpg-thermal-sim rpg-thermal-sim.cpp
``` 
then you can run the file with `./rpg-thermal-sim`

### Other Systems
The application is implemented with pure [C++11 standart](https://en.wikipedia.org/wiki/C%2B%2B11) and [ANSI escape sequences](https://en.wikipedia.org/wiki/ANSI_escape_code) so it should compile on any system supporting C++11 and draw colors correctly on any console with escape sequences compatibility.

If your system is not compatible, let me know in order to update this file or to add compatibility.

## Usage
### Launch
The application can be launched directly from the file browser by double-clicking the icon.

On linux systems you can launch it like any regular application:
```
PATH/TO/PROGRAM/rpg-thermal-sim [PATH/TO/SAVEFILE]
```
As remarked in the last code you can optionally pass a file as a parameter in order to load a previous map layout

### How to layout your map
Once you start the application the console terminal will output something like this:
```
Iteration: 0
command> 
```
Of course there's not much to do here, but let's assume that we want to model a 5x5 room, then, we would have to call the following command:
```
build roomA 5 5 The first room that we are modeling
```
now, the output will be something like this:
```
Iteration: 0
CODE: <roomA>
The first room that we are modeling
[   ][   ][   ][   ][   ]
[   ][   ][   ][   ][   ]
[   ][   ][   ][   ][   ]
[   ][   ][   ][   ][   ]
[   ][   ][   ][   ][   ]
command> 
```
The command build takes the first word as the name that identifies the room we've just created, the second is the width, the third the height, after that comes, optionally, a description.

Each cell of the room will be identified by the name of the room and it's coordinates, being the coordinates of the first cell (bottom left) `roomA 1 1` and the last cell (top right) `roomA 5 5`.

Now, figure we want this room to have a neighbouring room, let's say, like this:
```
[   ][   ][   ][   ][   ]
[   ][   ][   ][   ][   ]     [   ][   ][   ][   ]     
[   ][   ][   ][   ][   ][   ][   ][   ][   ][   ]
[   ][   ][   ][   ][   ]     [   ][   ][   ][   ]
[   ][   ][   ][   ][   ]
```
in order to achieve this layout we should create two additional rooms:
```
build doorAB 1 1 Door that connects roomA and roomB
build roomB 4 3
```
Let's check what does the output show us:
```
Iteration: 0
CODE: <doorAB>
Door that connects roomA and roomB
[   ]
CODE: <roomA>
The first room that we are modeling
[   ][   ][   ][   ][   ]
[   ][   ][   ][   ][   ]
[   ][   ][   ][   ][   ]
[   ][   ][   ][   ][   ]
[   ][   ][   ][   ][   ]
CODE: <roomB>
Room without description
[   ][   ][   ][   ]
[   ][   ][   ][   ]
[   ][   ][   ][   ]
command> 
```
Note that we didn't specified a description for roomB so it defaults to `Room without description`

Now this, by itself does not represent the scheme I'd drawn before, we must __link__ the cells together. In our rooms, each cell is represented by the brackets and it's contents, being `[   ]` an empty cell (there's nothing but air), cells can be shown in different colors and values that show information about it's state.

The most important state is the Temperature, in order to a cell to be able to calculate it's temperature, it needs to know it's neightbourging cells temperature. Every cell on a new room knows it's neighbouring cells, but if we want cells from different rooms to take into account each other, we'll need to specify it by ourselves, that's what the __link__ command does.

Let's connect the `doorAB 1 1` cell with the `roomA 5 3` and the `roomB 1 2` cells so we can achieve our objective with the next commands:
```
link doorAB 1 1 roomA 5 3
link doorAB 1 1 roomB 1 2
```
Now, our rooms are connected, even if the output does not differ from the last output.

Now It's time to ***put*** some ~~inflamable~~ forniture to decorate our pretty rooms...

![TO BE CONTINUED](https://i.kinja-img.com/gawker-media/image/upload/flgsb6ayx7n8wpk4cg2u.png)
