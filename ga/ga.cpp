#include "ga.hpp"
#include "TotalisticRule.hpp"

std::default_random_engine rng;

std::shared_ptr<TotalisticRule> genotype_to_phenotype(Genotype g)
{
   std::vector<Transition> full_table = g.from_one;
   full_table.insert(full_table.end(),
                     g.from_zero.begin(),
                     g.from_zero.end());
   return std::make_shared<TotalisticRule>(full_table);
}

int main(int argc, char** argv)
{
// usage: ga <population-size> <generations> <seed> [lca-factory-options]
   int population_size = atoi(argv[1]);
   int max_generations = atoi(argv[2]);
   int seed            = atoi(argv[3]);

   rng = std::default_random_engine(seed);

   argv+=4;
   argc-=4;

   std::vector<Genotype> population = initialize_population();

}
