/*
 * Implementation for Chromosome class
 */

#include <algorithm>
#include <cassert>
#include <random>

#include "chromosome.hh"
#include "cities.hh"

//////////////////////////////////////////////////////////////////////////////
// Generate a completely random permutation from a list of cities
Chromosome::Chromosome(const Cities* cities_ptr)
  : cities_ptr_(cities_ptr),
    order_(random_permutation(cities_ptr->size())),
    generator_(rand())
{
  assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// Clean up as necessary
Chromosome::~Chromosome()
{
  assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// Perform a single mutation on this chromosome
void
Chromosome::mutate()
{
  // get two indices to switch
  int index1 = generator_() % order_.size();
  int index2 = generator_() % order_.size();

  //make sure indices aren't the same
  while (index2 == index1)
  {
    index2 = generator_() % order_.size();
  }
  //swap 'em
  std::swap(order_[index1], order_[index2]);

  assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// Return a pair of offsprings by recombining with another chromosome
// Note: this method allocates memory for the new offsprings
std::pair<Chromosome*, Chromosome*>
Chromosome::recombine(const Chromosome* other)
{
  assert(is_valid());
  assert(other->is_valid());

  // choose a beginning and an end, and make sure beginning <= end
  int finish = generator_() % order_.size();
  int start;
  if (finish == 0)
  {
    start = 0;
  }
  else start = generator_() % finish;

  // create two childen based on beginning and end
  std::pair<Chromosome*, Chromosome*> children;
  children.first = Chromosome::create_crossover_child(this, other, start, finish);
  children.second = Chromosome::create_crossover_child(other, this, start, finish);

  return children;

}

//////////////////////////////////////////////////////////////////////////////
// For an ordered set of parents, return a child using the ordered crossover.
// The child will have the same values as p1 in the range [b,e),
// and all the other values in the same order as in p2.
Chromosome*
Chromosome::create_crossover_child(const Chromosome* p1, const Chromosome* p2,
                                   unsigned b, unsigned e) const
{
  Chromosome* child = p1->clone();

  // We iterate over both parents separately, copying from parent1 if the
  // value is within [b,e) and from parent2 otherwise
  unsigned i = 0, j = 0;

  for ( ; i < p1->order_.size() && j < p2->order_.size(); ++i) {
    if (i >= b and i < e) {
      child->order_[i] = p1->order_[i];
    }
    else { // Increment j as long as its value is in the [b,e) range of p1
      while (p1->is_in_range(p2->order_[j], b, e)) {
        ++j;
      }
      assert(j < p2->order_.size());
      child->order_[i] = p2->order_[j];
      j++;
    }
  }

  assert(child->is_valid());
  return child;
}

// Return a positive fitness value, with higher numbers representing
// fitter solutions (shorter total-city traversal path).
double
Chromosome::get_fitness() const
{
  // fitness in just 1 over the distance, so shorter distances have higher fitness
  return 1.0 / cities_ptr_->total_path_distance(order_);
}

// A chromsome is valid if it has no repeated values in its permutation,
// as well as no indices above the range (length) of the chromosome.
// We implement this check with a sort, which is a bit inefficient, but simple
bool
Chromosome::is_valid() const
{
  // copy vector and sort it, then make sure that each values is the same as the
  // index at which it is located
  Cities::permutation_t test_order = order_;
  std::sort(test_order.begin(), test_order.end());
  for (unsigned i = 0; i < test_order.size(); i++)
  {
    if (i != test_order[i])
    {
      return false;
    }
  }
  return true;
}

// Find whether a certain value appears in a given range of the chromosome.
// Returns true if value is within the specified the range specified
// [begin, end) and false otherwise.
bool
Chromosome::is_in_range(unsigned value, unsigned begin, unsigned end) const
{
  // linear search through order_ to see if value is in range
  for (unsigned i = begin; i != end; i++)
  {
    if (order_[i] == value)
      {
        return true;
      }
  }
  return false;
}
