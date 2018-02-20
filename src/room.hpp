/*
 * room.hpp
 *
 *  Created on: 19 feb. 2018
 *      Author: dabama1
 */

#ifndef ROOM_HPP_
#define ROOM_HPP_

#include <list>
#include <map>
#include <string>
#include <cstdlib>

class cell{
	int flame;
	int ignition;
	int temp_counters;
	int spreadable; //boolean

	int aux_counters = 0;

	std::list<cell*> neightbours;

	public:
	//TODO: GETTERS AND SETTERS
	cell(std::string args){//format: (...;)flame,ignition,temp_counters,spreadable(;...)
		std::string aux = args;
		flame = std::atoi(aux.substr(0,aux.find(",")).c_str());
		aux = aux.substr(aux.find(",")+1);
		ignition = std::atoi(aux.substr(0,aux.find(",")).c_str());
		aux = aux.substr(aux.find(",")+1);
		temp_counters = std::atoi(aux.substr(0,aux.find(",")).c_str());
		aux = aux.substr(aux.find(",")+1);
		spreadable = std::atoi(aux.c_str());
		aux_counters = 0;
	}

	cell(char arg){
		if(arg=='.'){
			cell("0,0,0,1");
		}else if(arg=='#'){
			cell("0,0,0,0");
		}else if(arg=='*'){
			cell("1,-20,0,1");
		}else{
			cell("0,0,0,1");
		}
	}

	void setStatus(int f,int i, int t){
		this->flame = f;
		this->ignition = i;
		this->temp_counters = t;
	}

	void addCounters(int add){
		this->aux_counters += add;
	}

	bool isSpreadable(){
		return this->spreadable?true:false;
	}

	void addNeightbour(cell* newcomer){
		if(newcomer->isSpreadable() && newcomer != this) this->neightbours.push_back(newcomer);
	}

	//Actions

	void spread(){
		int colder = 0;
		for(auto it = this->neightbours.begin();it!=this->neightbours.end();it++){
			cell* iterator = *it;
			if(iterator->spreadable && (iterator->temp_counters < this->temp_counters)) colder++;//FIXME: Segmentation fault
		}

		if(colder==0) return;
		else if(colder==1) this->aux_counters -= this->temp_counters;
		else this->aux_counters -= (this->temp_counters-this->temp_counters%colder);

		int giving = this->temp_counters/colder;
		for(auto it = this->neightbours.begin();it!=this->neightbours.end();it++){
			cell* iterator = *it;
			if(iterator->temp_counters < this->temp_counters) iterator->addCounters(giving);
		}
	}

	void commitStatus(){
			this->temp_counters += aux_counters;
			this->aux_counters = 0;
	}

	void checkFlashpoint(){
		if((this->temp_counters > this->ignition) && (this->ignition > 0)) {
			this->flame = 1;
			this->ignition *= -10;
		}
		else if(this->ignition <= -1){
			if(++this->ignition==0) this->flame = 0;
		}

		if(this->flame==1) this->temp_counters += 10;
	}

	void dissipateHeat(){
		for(auto it = this->neightbours.begin();it!=this->neightbours.end();it++){
			cell* iterator = *it;
			if(this->temp_counters == 0) break;
			if(iterator->temp_counters == 0) {
				this->temp_counters--;
				break;
			}
		}
	}

};

class room{
	std::map<int,cell*> layout;
	int w,h;
	std::string desc;

	void setNeigthbours(){
		for(int i = 0;i<w*h;i++){
					int jw = (i%w);
					int jh = (i/w);

					jh--;jw--;//upper line
					if((jw)>=0 && (jw<w) && jh>=0 && jh<h) layout[i]->addNeightbour(layout[jw+jh*w]);
					jw++;
					if((jw)>=0 && (jw<w) && jh>=0 && jh<h) layout[i]->addNeightbour(layout[jw+jh*w]);
					jw++;
					if((jw)>=0 && (jw<w) && jh>=0 && jh<h) layout[i]->addNeightbour(layout[jw+jh*w]);

					jh++;//middle line
					if((jw)>=0 && (jw<w) && jh>=0 && jh<h) layout[i]->addNeightbour(layout[jw+jh*w]);
					jw-=2;
					if((jw)>=0 && (jw<w) && jh>=0 && jh<h) layout[i]->addNeightbour(layout[jw+jh*w]);

					jh++;//lower line
					if((jw)>=0 && (jw<w) && jh>=0 && jh<h) layout[i]->addNeightbour(layout[jw+jh*w]);
					jw++;
					if((jw)>=0 && (jw<w) && jh>=0 && jh<h) layout[i]->addNeightbour(layout[jw+jh*w]);
					jw++;
					if((jw)>=0 && (jw<w) && jh>=0 && jh<h) layout[i]->addNeightbour(layout[jw+jh*w]);
				}
	}

	public:
	room(int w, int h, std::string rn = "Generic room"){
		this->w = w;
		this->h = h;

		for(int i = 0;i<w*h;i++) {
			cell* cellofgrid = new cell("0,0,0,1");
			layout.insert(std::pair<int,cell*>(i,cellofgrid));
		}

		//set neighbours
		setNeigthbours();

	}

	void iterate(int n=1){
		while(n--){
			for(int i = 0;i<w*h;i++) layout[i]->spread();
			for(int i = 0;i<w*h;i++) layout[i]->commitStatus();
			for(int i = 0;i<w*h;i++) layout[i]->spread();
			for(int i = 0;i<w*h;i++) layout[i]->commitStatus();

			for(int i = 0;i<w*h;i++) layout[i]->checkFlashpoint();

			for(int i = 0;i<w*h;i++) layout[i]->dissipateHeat();
		}
	}


	//TODO: getCellXY(int,int) to get and edit cell properties
	//TODO: toString() to show counters and maybe colors

};

//TODO: linkRooms(cell*,cell*) to add temperature transfer between rooms

#endif /* ROOM_HPP_ */
