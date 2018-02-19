/*
 * room.hpp
 *
 *  Created on: 19 feb. 2018
 *      Author: dabama1
 */

#ifndef ROOM_HPP_
#define ROOM_HPP_

#include <list>
#include <string>
#include <cstdlib>

class cell{
	int flame = 0;
	int ignition = 0;
	int temp_counters = 0;
	int spreadable = 0; //boolean

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
		if(newcomer->isSpreadable()) this->neightbours.push_back(newcomer);
		this->neightbours.unique();
	}

	//Actions

	void spread(){
		int colder = 0;
		for(auto it = this->neightbours.begin();it==this->neightbours.end();it++){
			cell* iterator = *it;
			if(iterator->spreadable && (iterator->temp_counters < this->temp_counters)) colder++;
		}

		if(colder==0) return;
		else if(colder==1) this->aux_counters -= this->temp_counters;
		else this->aux_counters -= (this->temp_counters-this->temp_counters%colder);

		int giving = this->temp_counters/colder;
		for(auto it = this->neightbours.begin();it==this->neightbours.end();it++){
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
		for(auto it = this->neightbours.begin();it==this->neightbours.end();it++){
			cell* iterator = *it;
			if(iterator->temp_counters == 0) {
				this->temp_counters--;
				break;
			}
		}
	}

};

class room{
	cell*** layout;

	public:
};



#endif /* ROOM_HPP_ */
