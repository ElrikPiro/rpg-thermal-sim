//============================================================================
// Name        : rpg-thermal-sim.cpp
// Author      : David Baselga, <emperador.albino@gmail.com>
// Version     : 0.0
// Copyright   : Attribution only
// Description : A simple RPG thermal simulator
//============================================================================

#include <iostream>
#include <map>
#include <string>
#include "room.hpp"
#include "building.hpp"
#ifdef _WIN64
	#include <windows.h>
#endif

int main(int argc, char ** argv) {
	#ifdef _WIN64
		SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE),0x0001|0x0002|0x0004);
	#endif
	building build;
	if(argc==1){
		build.loop();
	}else if(argc==2){
		build=building(std::string(argv[1]));
		build.loop();
	}else{
		puts("usage:\nrpg-thermal-sim [filename]");
		return -1;
	}
	return 0;
}
