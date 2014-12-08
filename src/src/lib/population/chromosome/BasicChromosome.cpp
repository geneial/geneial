/*
 * BasicChromome.cpp
 *
 *  Created on: Nov 26, 2014
 *      Author: bewo
 */

#include <population/chromosome/BasicChromosome.h>
#include <iostream>
#include "utility/random.h"

namespace GeneticLibrary {
namespace Population {
namespace Chromosome {


BasicChromosome::BasicChromosome() {

	double value;
	//Creating One Chromosome
	for (int i = 0; i <= chromosomeSize; i++){
		 value = Utility::random::instance()->generateDouble(0,100);
		 values[i] = value;
	}
	//Setting fitness-flag (-1 = not jet calculated)
	this->age = 0;
	this->fitness = -1;
}
BasicChromosome::~BasicChromosome() {
	// TODO Auto-generated destructor stub
}

int BasicChromosome::getFitness(){
	return this->fitness;
}

void BasicChromosome::setFitness(int fit){
	this->fitness = fit;
}

double BasicChromosome::getValue(int position){
	return this->values[position];
}

void BasicChromosome::setValue(int position, double value){
	this->values[position] = value;
}

void BasicChromosome::incrementAge(){
	this->age++;
}

void BasicChromosome::setAge(int age){
	this->age = age;
}

int BasicChromosome::getAge(){
	return this->age;
}


} /* namespace Chromosome */
} /* namespace Population */
} /* namespace GeneticLibrary */
