# rpg-thermal-sim
A thermal simulator to control the spreading of temperature and fire on a RPG setup. Personal project.

## Instalation

### Windows
Download [the Windows 64 bits executable](../../raw/master/x86_64%20win64/rpg-thermal-sim.exe) and launch it, it's compiled and tested on a fresh instalation of windows 10 but must work on any Windows 8 or higher with a x86_64 architecture (almost every common computer on market)

Alternatively you can compile the code by yourself with _Cygwin_, _MinGW_ or _cl (Visual Studio)_

### Linux
Clone or [download](https://github.com/ElrikPiro/rpg-thermal-sim/archive/master.zip) the repository and compile the contents of [the src directory](https://github.com/ElrikPiro/rpg-thermal-sim/tree/master/src) with the command:
```
g++ -std=c++11 -o rpg-thermal-sim rpg-thermal-sim.cpp
``` 
then you can run the file with `./rpg-thermal-sim`

### Other Systems
The application is implemented with pure C++11 standart and ANSI escape sequences so it should compile on any system supporting C++11 and draw colors correctly on any console with escape sequences compatibility.

If your system is not compatible, let me know in order to update this file or to add compatibility.

## Usage
TBA
