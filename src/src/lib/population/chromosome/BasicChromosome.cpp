/*
 * BasicChromome.cpp
 *
 *  Created on: Nov 26, 2014
 *      Author: bewo
 */

#include <population/chromosome/BasicChromosome.h>
#include <iostream>
#include "utility/random.h"
#include "Config.h"

namespace GeneticLibrary {
namespace Population {
namespace Chromosome {


/**
 * Creates a new Chromosome with random values and a fitness of -1
 */
BasicChromosome::BasicChromosome() {

	double value;
	//Creating One Chromosome
	for (int i = 0; i <= defChromosomeSize; i++){
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

/**
 * Gets the fitness value of a Chromosome.
 * Does not calculate the fitness.
 */
int BasicChromosome::getFitness(){
	return this->fitness;
}

/**
 * Sets Fitness of a chromosome
 */
void BasicChromosome::setFitness(int fit){
	this->fitness = fit;
}

/**
 * Chromosomes contain an array of values, this function is used to get one specific value of that Array
 * @position position in Array (starting at 0)
 */
double BasicChromosome::getValue(int position){
	return this->values[position];
}

/**
 * Chromosomes contain an array of values, this function is used to set one specific value of that Array
 * @position position in Array (starting at 0)
 * @value value to be set at position
 */
void BasicChromosome::setValue(int position, double value){
	this->values[position] = value;
}

/**
 * Used to 'age' a chromosome. Increments the age of a chromosome by one
 */
void BasicChromosome::doAge(){
	this->age++;
}

void BasicChromosome::setAge(unsigned int age){
	this->age = age;
}

unsigned int BasicChromosome::getAge(){
	return this->age;
}


} /* namespace Chromosome */
} /* namespace Population */
} /* namespace GeneticLibrary */
