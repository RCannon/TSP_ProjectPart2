Traveling Salesman Project Part 2 - Reilly Cannon only

Here is my implementation of a genetic algorithm that attempts to approximate a solution to the traveling salesman problem by calculating a locally optimal solution. To do this, I had to devise a notion of "fitness," where higher fitness values correspond to shorter distances for traversing all cities. I chose the relative fitness value to be 1 over the distance between cities in the order given in that "chromosome", which fits the requirements as previously stated.

to run the program in the shell, type:

./tsp <cities_file>.tsv <population_size> <mutation_rate>

the population size determines the number of chromosomes that will be used to find a solution, and the mutation rate determines how likely the chromosomes are to randomly switch cities in their local permutation. Each method is documented in the .cc files.
