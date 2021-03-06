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
#include <iostream>
#include <sstream>

class cell;
bool comparePair(std::pair<int,cell*> first, std::pair<int,cell*> second){
	return first.first < second.first;
}

class cell{
	int flame;
	int ignition;
	int temp_counters;
	int spreadable; //boolean

	int aux_counters = 0;

	std::list<cell*> neightbours;

	public:
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

	int getIgnition(){
		return this->ignition;
	}

	int getTemp(){
		return this->temp_counters;
	}

	void addCounters(int add){
		this->aux_counters += add;
	}

	bool isSpreadable(){
		return this->spreadable?true:false;
	}

	void addNeightbour(cell* newcomer){
		if(newcomer != this) this->neightbours.push_back(newcomer);
	}

	//Actions

	/*void _spread(){
			int colder = 0;
			int accumulate = temp_counters;
			//int avg = 0;
			int giving = 0;
			std::list<std::pair<int,cell*>> stop;
			for(auto it = this->neightbours.begin();it!=this->neightbours.end();it++){
				cell* iterator = *it;
				if(iterator->isSpreadable()) accumulate += iterator->temp_counters;
			}

			//avg = accumulate/(neightbours.size()+1);

			for(auto it = this->neightbours.begin();it!=this->neightbours.end();it++){
				cell* iterator = *it;
				if(iterator->isSpreadable() && iterator->temp_counters < temp_counters ) {
					colder++;
					stop.push_back(std::pair<int,cell*>(iterator->temp_counters,iterator));
				}
			}

			if(colder<1) return;
			giving = temp_counters-1;

			while(giving>0){
				stop.sort(comparePair);
				auto it = stop.begin();
				std::pair<int,cell*> _it = *it;
				cell* iterator = _it.second;
				stop.remove(_it);
				if(_it.first>=giving) return;
				iterator->addCounters(1);
				this->addCounters(-1);
				giving--;
				_it.first+=1;
				stop.push_front(std::pair<int,cell*>(_it.first,_it.second));
			}
		}*/

	void spread(){
				if(!isSpreadable()) return;
				int accumulate = temp_counters;
				int avg = 0;
				int flanders = 1;
				for(auto it = this->neightbours.begin();it!=this->neightbours.end();it++){
					cell* iterator = *it;
					if(iterator->isSpreadable()) {
						accumulate += iterator->temp_counters;
						flanders++;
					}
				}

				avg = accumulate/flanders;
				addCounters(avg-temp_counters);
			}

	void commitStatus(){
			if(aux_counters==0 && this->temp_counters>0) aux_counters--;
			this->temp_counters += aux_counters;
			this->aux_counters = 0;
	}

	void checkFlashpoint(){
		if((this->temp_counters > this->ignition*100) && (this->ignition > 0)) {
			this->flame = 1;
			this->ignition *= -10;
		}
		else if(this->ignition <= -1){
			if(++this->ignition==0) this->flame = 0;
		}

		if(this->flame==1) this->temp_counters += 750;
	}

	void dissipateHeat(){
		int flanders = 0;
		for(auto it = this->neightbours.begin();it!=this->neightbours.end();it++){
			cell* iterator = *it;
			if(iterator->isSpreadable()) flanders++;
		}
		int count = 0;
		if(!this->flame && this->temp_counters>=10) this->temp_counters -= this->temp_counters/10;
		for(auto it = this->neightbours.begin();it!=this->neightbours.end();it++){
			cell* iterator = *it;
			if(this->temp_counters <= 0) break;
			if(iterator->isSpreadable() && iterator->temp_counters == 0) {
				++count;
				if(count>=flanders/2){
					this->temp_counters--;
					break;
				}
			}
		}
	}

	//linkRooms(cell*,cell*) to add temperature transfer between rooms
	void linkRooms(cell* destination){
		this->addNeightbour(destination);
		destination->addNeightbour(this);
	}

	void ignite(){
		cell* c = this;

		if(c->getIgnition() > 0){
			c->setStatus(1,c->getIgnition()*-10,c->getTemp());
		}else if(c->getFlame()==1){
			c->setStatus(1,c->getIgnition()-1,c->getTemp());
		}else{
			c->setStatus(1,-2,c->getTemp());
		}
	}

	void deflagrate(int r){
		std::list<cell*> nhood = this->neightbours;
		for(auto it = nhood.begin();it!=nhood.end();it++){
			cell* iterator = *it;
			if(iterator->isSpreadable()) iterator->ignite();
			if(iterator->isSpreadable() && r>1) iterator->deflagrate(r-1);
		}
	}

	std::string toString(){
		char RESET[] = {0x1b,'[','3','9',';','4','9','m','\0'};
		char FIRE[] = {0x1b,'[','4','1','m','\0'};
		char HEAT[] = {0x1b,'[','1','0','0','m','\0'};
		char INFLAMMABLE[] = {0x1b,'[','4','4','m','\0'};

		std::ostringstream oss;
		oss << '[';
		if(this->flame==1)
			oss << FIRE << " * ";
		else if(!this->spreadable)
			oss << "###";
		else if(this->temp_counters>20){//until 1000 it works properly, I assume we'll never reach ten hundred degrees.
			oss << HEAT;
			if(this->temp_counters < 50) oss << "   ";
			else if(this->temp_counters < 100) oss << " " << this->temp_counters;
			else if(this->temp_counters < 1000) oss << this->temp_counters;
			else if(this->temp_counters < 10000) oss << " " << this->temp_counters/1000 << "k";
			else if(this->temp_counters < 100000) oss << this->temp_counters/1000 << "k";
			else if(this->temp_counters < 1000000) oss << "." << this->temp_counters/100000 << "M";
			else oss << "***";
		}else if(this->ignition>0){
			oss << INFLAMMABLE;
			if(this->ignition < 10) oss << " " << this->ignition << " ";
			else if(this->ignition < 100) oss << " " << this->ignition;
			else oss << this->ignition;
		}else{
			oss << "   ";
		}


		oss<<RESET<<']';
		return oss.str();
	}

	//forgetMe(), setUnreachable()
	void forgetMe(){
		for(auto it = this->neightbours.begin();it!=this->neightbours.end();it++){
			cell* iterator = *it;
			iterator->neightbours.remove(this);
		}
	}

	void setUnreachable(){
		this->spreadable = 0;
	}

	void setReachable(){
		this->spreadable = 1;
	}

	int getFlame(){
		return this->flame;
	}

	std::list<cell*> getNeightbourhood(){
		return neightbours;
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
		this->desc = rn;

		for(int i = 0;i<w*h;i++) {
			cell* cellofgrid = new cell("0,0,0,1");
			layout.insert(std::pair<int,cell*>(i,cellofgrid));
		}

		//set neighbours
		setNeigthbours();

	}

	~room(){
		for(int i = 0;i<w*h;i++) delete layout[i];
	}

	void iterate(int n=1){
		while(n--){
			for(int i = 0;i<w*h;i++) layout[i]->spread();
			for(int i = 0;i<w*h;i++) layout[i]->commitStatus();

			for(int i = 0;i<w*h;i++) layout[i]->checkFlashpoint();

			for(int i = 0;i<w*h;i++) layout[i]->dissipateHeat();
		}
	}


	//getCellXY(int,int) to get and edit cell properties
	cell* getCellXY(int x,int y){
		if(x>=w || y>=h || x < 0 || y < 0) {
			std::cout << "The cell you especified does not exist. please, introduce a valid combination:\nX: ";
			std::string in;
			std::getline(std::cin,in);
			x = std::atoi(in.c_str())-1;
			std::cout << "Y: ";
			std::getline(std::cin,in);
			y = std::atoi(in.c_str())-1;
			return getCellXY(x,y);
		}
		return layout[w*y+x];
	}

	std::string getDesc(){
		return this->desc;
	}



	//toString() to show counters and maybe colors
	std::string toString(){
		std::ostringstream oss;
		oss << this->desc << std::endl;
		for(int ih = this->h-1;ih>=0;ih--){
			for(int jw = 0;jw<this->w;jw++){
				oss << this->layout[ih*this->w+jw]->toString();
			}
			oss << std::endl;
		}
		return oss.str();
	}

	int getWidth(){
		return w;
	}

	int getHeight(){
		return h;
	}

};



#endif /* ROOM_HPP_ */
