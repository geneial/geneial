#pragma once

#include <geneial/namespaces.h>
#include <geneial/core/population/chromosome/BaseChromosome.h>
#include <geneial/core/population/ContainerTypes.h>
#include <geneial/utility/mixins/Printable.h>

#include <iostream>
#include <map>
#include <vector>
#include <unordered_map>
#include <memory>


geneial_private_namespace(geneial)
{
geneial_private_namespace(population)
{
geneial_private_namespace(management)
{
geneial_export_namespace
{
template<typename FITNESS_TYPE>
    class BaseManager;
}
}
}
}

geneial_private_namespace(geneial)
{
geneial_private_namespace(population)
{
    using ::geneial::utility::Printable;
    using ::geneial::population::management::BaseManager;
geneial_export_namespace
{

template<typename FITNESS_TYPE>
class Population: public Printable
{
private:
    Population();
public:

    friend class BaseManager<FITNESS_TYPE>;

    const static int POPULATION_AGE_INITIAL = 0;

    //TODO(bewo): cleanup this typedef mess
    typedef unsigned int population_age;
    typedef unsigned int population_size;

    //Alias chromomsome container
    typedef typename ContainerTypes<FITNESS_TYPE>::chromosome_container chromosome_container;

    //In our case we do not need to rehash so we redirect
    struct IDHash
    {
       inline size_t operator() (const typename BaseChromosome<FITNESS_TYPE>::chromsome_hash &hash) const
       {
         return hash;
       }
    };

    //A map containing all the chromosomes hash values.
    typedef typename std::unordered_map<typename BaseChromosome<FITNESS_TYPE>::chromsome_hash,
            typename BaseChromosome<FITNESS_TYPE>::ptr> hash_map;


    typedef typename hash_map::value_type hashmap_value_type;
    typedef typename hash_map::key_type hashmap_key_type;

    typedef typename hash_map::const_iterator hashmap_const_it;
    typedef typename hash_map::iterator hashmap_it;

    //A fitness <-> chromsome map holding the actual population, multiple chromosomes might have same fitness
    typedef typename std::multimap<FITNESS_TYPE, typename BaseChromosome<FITNESS_TYPE>::ptr> fitness_map;

    typedef typename fitness_map::value_type fitnessmap_value_type;
    typedef typename fitness_map::key_type fitnessmap_key_type;

    typedef typename fitness_map::const_iterator fitnessmap_const_it;
    typedef typename fitness_map::iterator fitnessmap_it;

//    typedef typename std::unordered_map<typename BaseChromosome<FITNESS_TYPE>::chromsome_hash,FITNESS_TYPE> fitness_cache;

    population_size getSize() const;

    //TODO(bewo) Population(Population &other);

    //TODO(bewo) Population(Population &&other);

    virtual ~Population();

    virtual void print(std::ostream& os) const;

    population_age getAge() const;

    void setAge(population_age age);

    void doAge();

    typename BaseChromosome<FITNESS_TYPE>::ptr getOldestChromosome();

    typename BaseChromosome<FITNESS_TYPE>::ptr getYoungestChromosome();

    const inline fitness_map& getFitnessMap() const
    {
        return _fitnessMap;
    }

    const inline hash_map& getHashMap() const
    {
        return _hashMap;
    }

    bool hashExists(const typename BaseChromosome<FITNESS_TYPE>::chromsome_hash);

    unsigned int removeDuplicates(chromosome_container &toCheck);

    typename BaseChromosome<FITNESS_TYPE>::ptr getChromosomeByHash(
            const typename BaseChromosome<FITNESS_TYPE>::chromsome_hash);

    void replacePopulation(chromosome_container &replacementPopulation);

    unsigned int insertChromosomeContainer(chromosome_container &container);

    bool insertChromosome(const typename BaseChromosome<FITNESS_TYPE>::ptr &chromosome);

    void removeChromosomeContainer(const chromosome_container &container);

    void removeChromosome(const typename BaseChromosome<FITNESS_TYPE>::ptr &chromosome);

    void clearChromosomes();

    BaseManager<FITNESS_TYPE>& getManager() const
    {
        assert(!_manager.expired() && "Manager is already expired. Something was not properly bootstrapped");
        const auto manager = _manager.lock();
        return *manager;
    }

private:

    void _insertChromosome(const typename BaseChromosome<FITNESS_TYPE>::ptr &chromosome,
            typename BaseChromosome<FITNESS_TYPE>::chromsome_hash hashValue);

    fitness_map _fitnessMap;

    hash_map _hashMap;

//    fitness_cache _fitnessCache;

    population_age _age;

    std::weak_ptr<BaseManager<FITNESS_TYPE>>  _manager;

};

} /* geneial_export_namespace */
} /* private namespace population */
} /* private namespace geneial */
#include <geneial/core/population/Population.hpp>

