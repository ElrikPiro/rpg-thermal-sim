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
//using namespace std;

int main(int argc, char ** argv) {
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
