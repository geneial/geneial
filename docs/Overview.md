GeneiAL      {#mainpage}
========

GeneiAL is an open source extensible [genetic algorithm](https://en.wikipedia.org/wiki/Genetic_algorithm) library. It provides a framework to solve optimization problems by maximizing fitness value of the chromosomes in a population over the course of generations using a custom fitness function.
The library offers the user high flexibility and extensibility while simultaneously featuring good scalability for the core functionality.

## Installation and Documentation
 * Installation guide: @subpage readme.
 * Additional information on the delivered demos: @subpage demos.
 * Documentation guidelines: @subpage guidelines.
 * Coding guidelines: @subpage coding.
 * Licence: @subpage licence.
 
## Implemented Features
 * support for user-defined chromosome types (e.g., the optimization of sequences of genes)
 * generation of new chromosomes using random values within a user-specified interval
 * generation of new chromosomes by partially/fully reusing population from a previous optimization
 * multi-threading support for the evaluation of the fitness function
 * diagnostic tools to analyze the genetic algorithm's behavior (i.e., runtime measurement, analysis of genetic operators, etc.)
 * extendible by user-specified analysis tools with customized pre- and post-processing logic

### Genetic Operators
 * roulette-wheel selection
 * N-point crossover, smoothed variant of N-point crossover
 * uniform mutation, smoothed variant of uniform mutation
 * extendible by customized genetic operators

### Stopping Criteria
 * basic stopping criteria (_fixed-point criterion_,_maximal number of generations_, etc.)
 * extendible by user-defined stopping criteria
 * support for a combinations (AND, OR, XOR, Negation) of multiple stopping criteria
