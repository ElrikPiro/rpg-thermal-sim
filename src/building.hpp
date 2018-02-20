/*
 * building.hpp
 *
 *  Created on: 20 feb. 2018
 *      Author: dabama1
 */

#ifndef BUILDING_HPP_
#define BUILDING_HPP_

#include <list>
#include <map>
#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include "room.hpp"

class building{
	std::map<std::string, room*> buildingLayout;
	std::list<std::string> links;

	public:
	building(){
		puts("EMPTY BUILDING CREATED.");
	}

	building(std::map<std::string, room*> rooms){
		buildingLayout = rooms;
	}

	building(std::string file){
		//TODO: files will be opened and appended with every single command
	}

	//TODO: commands
	void refresh(){
		for(auto it = buildingLayout.begin();it!=buildingLayout.end();it++){
			std::pair<std::string, room*> iterator = *it;
			std::cout << "CODE: <" << iterator.first << ">" << std::endl << iterator.second->toString();
		}
		std::cout << "command> ";
	}

	void iterate(int n=1){
		for(int i = 0;i<n;i++){
			for(auto it = buildingLayout.begin();it!=buildingLayout.end();it++){
				std::pair<std::string, room*> iterator = *it;
				iterator.second->iterate(1);
			}
		}
	}

	void newRoom(std::string ID,int w,int h,std::string desc){
		room* r = new room(w,h,"desc");
		buildingLayout[ID] = r;
	}
	//TODO: Command parser


};



#endif /* BUILDING_HPP_ */
