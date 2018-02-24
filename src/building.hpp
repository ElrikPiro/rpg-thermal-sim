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
#include <istream>
#include <sstream>
#include "room.hpp"

const char CLEAR[] = {0x1b,'[','2','J'};

typedef std::map<std::string, room*> layout;

class building{
	std::map<std::string, room*> buildingLayout;
	layout ref;
	std::list<std::string> links;
	int iteration = 0;

	public:
	building(){
		puts("EMPTY BUILDING CREATED.");
	}

	building(std::map<std::string, room*> rooms){
		buildingLayout = rooms;
		ref = rooms;
	}

	building(std::string file){
		//TODO: files will be opened and appended with every single command
	}

	//TODO: commands //FIXME: input problems must be prevented
	void refresh(layout buildingLayout){
		std::cout << CLEAR;
		std::cout << "Iteration: " << this->iteration << "\n";
		for(auto it = buildingLayout.begin();it!=buildingLayout.end();it++){
			std::pair<std::string, room*> iterator = *it;
			std::cout << "CODE: <" << iterator.first << ">" << std::endl << iterator.second->toString();
		}
	}

	void iterate(int n=1){
		for(int i = 0;i<n;i++){
			this->iteration++;
			for(auto it = buildingLayout.begin();it!=buildingLayout.end();it++){
				std::pair<std::string, room*> iterator = *it;
				iterator.second->iterate(1);
			}
		}
	}

	void newRoom(std::string ID,int w,int h,std::string desc){
		room* r = new room(w,h,desc);
		buildingLayout[ID] = r;
	}

	void setCell(std::string ID,int x,int y,int flame=0,int ignition=0, int temp=0){
		buildingLayout[ID]->getCellXY(x,y)->setStatus(flame,ignition,temp);
	}

	void linkCells(std::string ID1,int x1,int y1,std::string ID2,int x2, int y2){
		buildingLayout[ID1]->getCellXY(x1,y1)->linkRooms(buildingLayout[ID2]->getCellXY(x2,y2));
	}

	//check: lets you check a room exclusively
	void checkRoom(std::string ID){
		std::cout << CLEAR;
		std::cout << "Iteration: " << ++this->iteration << "\n";
		std::cout << "CODE: <" << ID << ">" << std::endl << buildingLayout[ID]->toString();
	}

	//list: lists all rooms without rendering
	void listRooms(){
		for(auto it = buildingLayout.begin();it!=buildingLayout.end();it++){
			std::pair<std::string, room*> iterator = *it;
			std::cout << "<" << iterator.first << ">: " << std::endl << iterator.second->getDesc() << std::endl;
		}
	}

	//ignite: ignites a cell
	void ignite(std::string ID,int x,int y){
		cell* c = buildingLayout[ID]->getCellXY(x,y);
		_ignite(c);
	}

	void _ignite(cell* c){//FIXME: crap like this should be moved to the cell class
		if(c->getIgnition() > 0){
			c->setStatus(1,c->getIgnition()*-10,c->getTemp());
		}else if(c->getFlame()==1){
			c->setStatus(1,c->getIgnition()-1,c->getTemp());
		}else{
			c->setStatus(1,-2,c->getTemp());
		}
	}

	//deflagrate(int): ignites a cell and its neightbours on n levels
	void deflagrate(std::string ID,int x,int y,int r = 1){
		cell* c = buildingLayout[ID]->getCellXY(x,y);
		_ignite(c);
		_deflagrate(c,r);
	}

	void _deflagrate(cell* c,int r=1){//FIXME: crap like this should be moved to the cell class
		std::list<cell*> nhood = c->getNeightbourhood();
		for(auto it = nhood.begin();it!=nhood.end();it++){
			cell* iterator = *it;
			if(iterator->isSpreadable()) _ignite(iterator);
			if(iterator->isSpreadable() && r>1) _deflagrate(iterator,r-1);
		}
	}

	void block(std::string ID,int x,int y){
		cell* c = buildingLayout[ID]->getCellXY(x,y);
		c->setUnreachable();
	}

	void unblock(std::string ID,int x,int y){
			cell* c = buildingLayout[ID]->getCellXY(x,y);
			c->setReachable();
	}
	//help: list all commands
	void help(){//TODO: make it prettier
		std::cout <<
				"refresh [roomID [roomID [(...)}]] - cleans the screen and shows all the rooms on buildings "
				"if a list of room IDs is provided, it will only show those rooms.\n"
				"\n"
				"iterate [n] - calculates the next n iterations, n defaults to 1\n"
				"\n"
				"build roomID w h [description] - builds a new empty room\n"
				"\n"
				"set roomID x y flame ignition temperature - sets a new status to the selected"
				"cell \n"
				"\n"
				"link roomID x y roomID x y - link two cells, intended to connect cells between rooms\n"
				"\n"
				"list - shows all room names and descriptions\n"
				"\n"
				"ignite - sets a cell on fire\n"
				"\n"
				"deflagrate roomID x y [r] - set a cell and it's neightbours on fire, "
				"if r is set higher than 1, it will do it recursively r times\n"
				"\n"
				"block roomID x y - makes a cell unspreadable\n"
				"\n"
				"unblock roomID x y - makes a cell spreadable\n"
				"\n"
				"Note that blank spaces will act as a separator.\n"<<std::endl;
	}


	int command(){
		std::string input, command;
		std::cout << "command> ";
		std::getline(std::cin,input);
		std::istringstream args(input);
		std::getline(args,command,' ');

		if(command==""){iterate();refresh(ref);return 0;}

		//TODO: the whole parsing
		if(command=="refresh"){

			ref.clear();

			while(std::getline(args,command,' ')){
				if( this->buildingLayout.find(command) == this->buildingLayout.end() ) return 2;
				ref[command] = this->buildingLayout[command];
			}

			if(ref.empty()) ref = this->buildingLayout;
			refresh(ref);

			return 0;
		}else if(command=="iterate"){
			bool a = (bool) std::getline(args,command,' ');
			if(a&&std::atoi(command.c_str())>1) iterate(std::atoi(command.c_str()));
			else iterate();
			refresh(ref);
			return 0;
		}else if(command=="build"){//void newRoom(std::string ID,int w,int h,std::string desc)
			std::string ID,desc;
			int w,h;
			if(std::getline(args,command,' ')){
				ID = command;
				if( this->buildingLayout.find(command) != this->buildingLayout.end() ) return 3;
			}else return 1;

			if(std::getline(args,command,' ')){
				w = std::atoi(command.c_str());
				if( w<1 ) return 2;//FIXME: ojo! those input must be checked in order ti avoid referencing outbounds
			}else return 1;

			if(std::getline(args,command,' ')){
				h = std::atoi(command.c_str());
				if( h<1 ) return 2;
			}else return 1;

			if(std::getline(args,desc)){

			}else desc = "Room without description";

			newRoom(ID,w,h,desc);
			ref[ID] = this->buildingLayout[ID];
			return 0;
		}else if(command=="set"){
			std::string ID;
			int w,h;
			int flame=0,ignition=0,temperature=0;

			if(std::getline(args,command,' ')){
				ID = command;
				if( this->buildingLayout.find(command) == this->buildingLayout.end() ) return 4;
			}else return 1;

			if(std::getline(args,command,' ')){
				w = std::atoi(command.c_str())-1;
				if( w<0 ) return 2;
			}else return 1;

			if(std::getline(args,command,' ')){
				h = std::atoi(command.c_str())-1;
				if( h<0 ) return 2;
			}else return 1;

			if(std::getline(args,command,' ')){
				flame = std::atoi(command.c_str());
				if( flame<0 || flame>1 ) return 2;
			}

			if(std::getline(args,command,' ')){
				ignition = std::atoi(command.c_str());
				if( flame > 0 && ignition > 0 ) return 2;
			}

			if(std::getline(args,command,' ')){
				temperature = std::atoi(command.c_str());
				if( temperature < 0 ) return 2;
			}

			setCell(ID,w,h,flame,ignition,temperature);
			return 0;
		}else if(command=="exit") return -1;
		else if(command=="link"){
			std::string ID1,ID2;
			int w1,h1,w2,h2;

			if(std::getline(args,command,' ')){
				ID1 = command;
				if( this->buildingLayout.find(command) == this->buildingLayout.end() ) return 3;
			}else return 1;

			if(std::getline(args,command,' ')){
				w1 = std::atoi(command.c_str())-1;
				if( w1<0 ) return 2;
			}else return 1;

			if(std::getline(args,command,' ')){
				h1 = std::atoi(command.c_str())-1;
				if( h1<0 ) return 2;
			}else return 1;

			if(std::getline(args,command,' ')){
				ID2 = command;
				if( this->buildingLayout.find(command) == this->buildingLayout.end() ) return 3;
			}else return 1;

			if(std::getline(args,command,' ')){
				w2 = std::atoi(command.c_str())-1;
				if( w2<0 ) return 2;
			}else return 1;

			if(std::getline(args,command,' ')){
				h2 = std::atoi(command.c_str())-1;
				if( h2<0 ) return 2;
			}else return 1;

			linkCells(ID1,w1,h1,ID2,w2,h2);
			return 0;
		}else if(command=="list"){
			for(auto it = this->buildingLayout.begin();it!=this->buildingLayout.end();it++){
				listRooms();
				return -2;
			}
		}else if(command=="ignite"){
			std::string ID1;
			int x,y;

			if(std::getline(args,command,' ')){
				ID1 = command;
				if( this->buildingLayout.find(command) == this->buildingLayout.end() ) return 3;
			}else return 1;

			if(std::getline(args,command,' ')){
				x = std::atoi(command.c_str())-1;
				if( x<0 ) return 2;
			}else return 1;

			if(std::getline(args,command,' ')){
				y = std::atoi(command.c_str())-1;
				if( y<0 ) return 2;
			}else return 1;

			ignite(ID1,x,y);
			return 0;
		}else if(command=="deflagrate"){
			std::string ID1;
			int x,y,r;
				if(std::getline(args,command,' ')){
				ID1 = command;
					if( this->buildingLayout.find(command) == this->buildingLayout.end() ) return 3;
				}else return 1;

				if(std::getline(args,command,' ')){
					x = std::atoi(command.c_str())-1;
					if( x<0 ) return 2;
				}else return 1;

				if(std::getline(args,command,' ')){
					y = std::atoi(command.c_str())-1;
					if( y<0 ) return 2;
				}else return 1;

				if(std::getline(args,command,' ')){
					r = std::atoi(command.c_str());
					if( r<1 ) return 2;
				}else r=1;

				deflagrate(ID1,x,y,r);
				return 0;
		}else if(command=="block"){
			std::string ID1;
			int x,y;

			if(std::getline(args,command,' ')){
				ID1 = command;
				if( this->buildingLayout.find(command) == this->buildingLayout.end() ) return 3;
			}else return 1;

			if(std::getline(args,command,' ')){
				x = std::atoi(command.c_str())-1;
				if( x<0 ) return 2;
			}else return 1;

			if(std::getline(args,command,' ')){
				y = std::atoi(command.c_str())-1;
				if( y<0 ) return 2;
			}else return 1;

			block(ID1,x,y);
			return 0;
		}else if(command=="unblock"){
			std::string ID1;
			int x,y;

			if(std::getline(args,command,' ')){
				ID1 = command;
				if( this->buildingLayout.find(command) == this->buildingLayout.end() ) return 3;
			}else return 1;

			if(std::getline(args,command,' ')){
				x = std::atoi(command.c_str())-1;
				if( x<0 ) return 2;
			}else return 1;

			if(std::getline(args,command,' ')){
				y = std::atoi(command.c_str())-1;
				if( y<0 ) return 2;
			}else return 1;

			unblock(ID1,x,y);
			return 0;
		}

		return 1;
	}

	void loop(){
		int h = 0;
		while(h!=-1){
			h=command();
			if(h==0) refresh(ref);
			if(h==1) help();
			else if(h>1) std::cerr << "Error code: " << h << std::endl;
			if(h==2) std::cerr << "wrong input" << std::endl;
			if(h==3) std::cerr << "object already exist" << std::endl;
			if(h==4) std::cerr << "room does not exist" << std::endl;
		}
	}

};



#endif /* BUILDING_HPP_ */
