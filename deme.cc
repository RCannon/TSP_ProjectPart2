/*
 * Declarations for Deme class to evolve a genetic algorithm for the
 * travelling-salesperson problem.  A deme is a population of individuals.
 */
#include <cmath>
#include <random>
#include <cassert>

#include "chromosome.hh"
#include "deme.hh"

// Generate a Deme of the specified size with all-random chromosomes.
// Also receives a mutation rate in the range [0-1].
Deme::Deme(const Cities* cities_ptr, unsigned pop_size, double mut_rate)
    : mut_rate_(mut_rate), generator_(rand())
{
    // add pop_size number of cities to vector
    for (unsigned i = 0; i != pop_size; i++)
    {
        pop_.push_back(new Chromosome(cities_ptr));
    }
}

// Clean up as necessary

Deme::~Deme()
{
    // need to release all the memory taken up by chromosomes in vector
    for (auto v : pop_)
    {
        delete v;
    }
}

// Evolve a single generation of new chromosomes, as follows:
// We select pop_size/2 pairs of chromosomes (using the select() method below).
// Each chromosome in the pair can be randomly selected for mutation, with
// probability mut_rate, in which case it calls the chromosome mutate() method.
// Then, the pair is recombined once (using the recombine() method) to generate
// a new pair of chromosomes, which are stored in the Deme.
// After we've generated pop_size new chromosomes, we delete all the old ones.
void Deme::compute_next_generation()
{
    // make a bunch of babies, add them to a new popolation vector,
    // then delete the older generation and relplace the population vector
    // with the new one
    std::vector<Chromosome*> new_pop; 
    double mutate_val; // stores the value that tells us to mutate or not
    Chromosome* parent1;
    Chromosome* parent2;
    std::pair<Chromosome*, Chromosome*> kids;
    for (unsigned i = 0; i < pop_.size() / 2; i++)
    {
        parent1 = select_parent();
        parent2 = select_parent();

        // make sure parent1 and parent2 are not the same
        while (parent1 == parent2)
        {
            parent2 = select_parent();
        }

        // generate two random values that will tell us whether or not
        // to mutate each parent
        static std::uniform_real_distribution<double> dis(0.0, 1.0);
        mutate_val = dis(generator_);
        if (mutate_val < mut_rate_)
        {
            parent1->mutate();
        }

        mutate_val = dis(generator_);
        if (mutate_val < mut_rate_)
        {
            parent2->mutate();
        }

        kids = parent1->recombine(parent2);
        new_pop.push_back(kids.first);
        new_pop.push_back(kids.second);
    }
    // delete each chromosome in old pop vector
    for (auto v : pop_)
    {
        delete v;
    }
    pop_ = new_pop;
}

// Return a copy of the chromosome with the highest fitness.
const Chromosome* Deme::get_best() const
{

    Chromosome* best = nullptr;

    // dont need a very big number since all fitness values should be less than 1
    // but just to be safe...
    double best_fit = 100000; 

    double fitn;

    // for each chromosome, add its fitness to the running total
    for (auto v : pop_)
    {
        fitn = v->get_fitness();
        if (fitn < best_fit)
        {
            best_fit = fitn;
            best = v;
        }
    }
    assert(best != nullptr);
    return best;
}

// Randomly select a chromosome in the population based on fitness and
// return a pointer to that chromosome.
Chromosome* Deme::select_parent()
{

    double total_fitn = 0;
    // get the total fitness of population
    for (auto v : pop_)
    {
        total_fitn += v->get_fitness();
    }
    // pick a random number [0, total_fitn)
    static std::uniform_real_distribution<double> distri(0.0, total_fitn);
    double start_fitn = distri(generator_);

    // to select parent, iterate through chromosomes, adding their fitness to
    // the random number we generated. Once the number is greater than the
    // total fitness, choose that chromosome as parent
    for (auto v : pop_)
    {
        start_fitn += v->get_fitness();
        if (start_fitn > total_fitn)
        {
            return v;
        }
    }
    // if for some reason the sum never gets larger than the total fitness
    // then return the last chromosome in the vector
    return pop_.back();
}
