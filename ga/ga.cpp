#include <random>
#include <memory>
#include <vector>
#include <utility>
#include <set>

#include "ga.hpp"
#include "TotalisticRule.hpp"
#include "LCAFactory.hpp"

#define NUM_SAMPLES 40

std::default_random_engine rng;

std::shared_ptr<TotalisticRule> genotype_to_phenotype(Genotype g)
{
   std::vector<Transition> full_table = g.one_transitions;
   full_table.insert(full_table.end(),
                     g.zero_transitions.begin(),
                     g.zero_transitions.end());
   return std::make_shared<TotalisticRule>(full_table);
}

std::vector<Transition> split_at(double split,
                                 std::vector<Transition>& transitions,
                                 bool left,
                                 int new_state,
                                 double new_heading)
{
   std::vector<Transition> new_transitions;
   for(Transition& t : transitions)
   {
      if(t.range.ExcludeMax().ExcludeMin().Contains(split))
      {
         Range range_l(t.range.Min(), split);
         Range range_r(split, t.range.Max());
         Transition l = t;
         Transition r = t;

         if(left) {
            l.result_state = new_state;
            l.heading_change = new_heading;
         }
         else {
            r.result_state = new_state;
            r.heading_change = new_heading;
         }
         l.range = Range(t.range.Min(), split);
         r.range = Range(split, t.range.Max());

         new_transitions.push_back(l);
         new_transitions.push_back(r);
      }
      else
      {
         new_transitions.push_back(t);
      }
   }
   return new_transitions;
}

std::vector<Genotype> initialize_population(int pop_size)
{
   std::uniform_int_distribution<int> transition_count(0,10);
   std::uniform_real_distribution<double> partition_distribution(0,1);
   std::uniform_real_distribution<double> heading_distribution(0, 2.0*M_PI);
   std::uniform_int_distribution<int> state_distribution(0,1);
   std::bernoulli_distribution coin(0.5);

   std::vector<Genotype> pop;
   for(int i = 0; i < pop_size; i++)
   {
      // XXX: start simple
      int zero_transitions = 1; // transition_count(rng);
      int one_transitions  = 1; // transition_count(rng);

      Genotype g;

      // Set up the transition on the interval [0,1]
      Transition t1;
      t1.include_self = true;
      t1.pre_state = 1;
      t1.heading_change = heading_distribution(rng);
      t1.result_state = state_distribution(rng);
      g.one_transitions.push_back(t1);

      Transition t0;
      t1.include_self = true;
      t0.pre_state = 0;
      t0.heading_change = heading_distribution(rng);
      t0.result_state = state_distribution(rng);
      g.zero_transitions.push_back(t0);

      for(int t = 1; t < zero_transitions-1; t++)
      {
         g.zero_transitions = split_at(partition_distribution(rng),
                                       g.zero_transitions,
                                       coin(rng),
                                       state_distribution(rng),
                                       heading_distribution(rng));
      }

      for(int t = 1; t < one_transitions-1; t++)
      {
         g.one_transitions = split_at(partition_distribution(rng),
                                      g.one_transitions,
                                      coin(rng),
                                      state_distribution(rng),
                                      heading_distribution(rng));
      }
      pop.push_back(g);
   }
   return pop;
}

double evaluate_rule(LCAFactory& factory, int samples)
{
   std::uniform_real_distribution<double> density_distribution(0.0,1.0);
   int num_correct = 0;

   for(int i = 0; i < samples; i++)
   {
      std::unique_ptr<LCA> lca = factory.Create(density_distribution(rng));
      lca->MinimizeMemory();
      lca->Run([](const ModelStats& s) { return (s.CurrentCADensity() == 0.0 || s.CurrentCADensity() == 1.0); });

      if(lca->GetStats().IsCorrect())
      {
         num_correct++;
      }
   }

   return (double)num_correct / (double)samples;
}

Genotype tournament(int k, std::vector<std::pair<Genotype, double>>& population)
{
   std::pair<Genotype, double> parent = std::make_pair(Genotype(), -1);
   std::uniform_int_distribution<int> index_distribution(0, population.size()-1);
   // select the first parent
   std::set<int> selections;
   for(int i = 0; i < k; i++)
   {
      int s;

      do
      {
         s = index_distribution(rng);
      } while(selections.find(s) != selections.end());

      selections.emplace(s);
      auto selection = population[s];

      if(selection.second > parent.second)
      {
         parent = selection;
      }
   }
   return parent.first;
}

std::pair<Genotype, Genotype> select(std::vector<std::pair<Genotype, double>>& population)
{
   Genotype parent_a = tournament(4, population);
   Genotype parent_b = tournament(4, population);
   return std::make_pair(parent_a, parent_b);
}

std::vector<Transition> merge_at(double x,
                                 std::vector<Transition> left,
                                 std::vector<Transition> right)
{
   std::vector<Transition> new_transitions;
   bool has_left = false;
   bool has_right = false;
   Transition left_overlap;
   Transition right_overlap;
   for(Transition& t: left)
   {
      if(t.range.ExcludeMax().ExcludeMin().Contains(x))
      {
         left_overlap = t;
         has_left = true;
      }
      else if(t.range.Max() <= x)
      {
         new_transitions.push_back(t);
      }
   }
   for(Transition& t: right)
   {
      if(t.range.ExcludeMax().ExcludeMin().Contains(x))
      {
         right_overlap = t;
         has_right = true;
      }
      else if(t.range.Min() >= x)
      {
         new_transitions.push_back(t);
      }
   }

   // combine the overlapping transitions.
   if(has_left && has_right)
   {
      left_overlap.range = Range(left_overlap.range.Min(), x);
      right_overlap.range = Range(x, right_overlap.range.Max());
      new_transitions.push_back(left_overlap);
      new_transitions.push_back(right_overlap);
   }
   else if(has_left)
   {
      left_overlap.range = Range(left_overlap.range.Min(), x);
      new_transitions.push_back(left_overlap);
   }
   else if(has_right)
   {
      right_overlap.range = Range(x, right_overlap.range.Max());
      new_transitions.push_back(right_overlap);
   }

   return new_transitions;
}

Genotype xover(std::pair<Genotype, Genotype> parents)
{
   std::uniform_real_distribution<double> xover_point_distribution(0,1);
   Genotype child;

   child.zero_transitions = merge_at(xover_point_distribution(rng),
                                     parents.first.zero_transitions,
                                     parents.second.zero_transitions);
   child.one_transitions = merge_at(xover_point_distribution(rng),
                                    parents.first.one_transitions,
                                    parents.second.one_transitions);

   return child;
}

// TODO
Genotype mutate(Genotype g)
{
   // - The xover process will likely create many small intervals (so
   //   small that they are rarely, if ever, triggered). There needs to
   //   be a mechanism by which these sectors can be merged back in to
   //   other sectors.
   //
   //   This could be addressed by
   //
   //   1. a "DELETE" mutation which removes a transition (growing the
   //   adjacent transitions to fill the gap)
   //
   //   2. a "RESIZE" mutation which incrementally grows/shrinks the
   //   transition interval (taking range away from adjacent intervals)
   //
   //   3. a "MERGE" mutation which combines two adjacent transitions
   //   with the same result state and combines them taking the average
   //   heading change of the two original transitions.

   std::uniform_real_distribution<double> p(0,1);
   std::uniform_int_distribution<int>
      index_distribution(0, g.zero_transitions.size() + g.one_transitions.size() - 1);
   std::normal_distribution<double> heading_change(0, 1.0);

   // with a very small probability flip the result state of a transition
   if(p(rng) < 0.01)
   {
      int i = index_distribution(rng);
      if(i < g.zero_transitions.size())
      {
         g.zero_transitions[i].result_state = 1-g.zero_transitions[i].result_state;
      }
      else
      {
         i = i - g.zero_transitions.size();
         g.one_transitions[i].result_state = 1-g.one_transitions[i].result_state;
      }
   }

   // with a less small probability select a heading change and add a small value to it.
   if(p(rng) < 0.05)
   {
      int i = index_distribution(rng);
      if(i < g.zero_transitions.size())
      {
         g.zero_transitions[i].heading_change += heading_change(rng);
      }
      else
      {
         i = i - g.zero_transitions.size();
         g.one_transitions[i].heading_change += heading_change(rng);
      }
   }

   return g;
}

std::vector<Genotype> run_ga(LCAFactory& factory,
                             std::vector<Genotype> initial_population,
                             int num_generations)
{
   std::vector<Genotype> current_population = initial_population;
   for(int generation = 0; generation < num_generations; generation++)
   {
      std::vector<std::pair<Genotype, double>> population_fitness;

      // 1. evaluate each member of the population K times on K different
      // initial conditions.
      double best_fitness = 0;
      Genotype best_genotype;
      for(Genotype g : current_population)
      {
         factory.SetRule(genotype_to_phenotype(g));
         double fitness = evaluate_rule(factory, NUM_SAMPLES);
         population_fitness.push_back(std::make_pair(g, fitness));
         if(fitness > best_fitness)
         {
            best_fitness = fitness;
            best_genotype = g;
         }
      }
      std::cout << "generation: " << generation << "[" << best_fitness << "]" << std::endl;

      current_population.clear();

      // 2. Select the best individual to be held over (elitism)
      current_population.push_back(best_genotype);

      // 3. selection, recombination, and mutation
      while(current_population.size() < population_fitness.size())
      {
         std::pair<Genotype,Genotype> parents = select(population_fitness);
         Genotype child = mutate(xover(parents));
         current_population.push_back(child);
      }
   }

   return current_population;
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

   LCAFactory factory;
   factory.Init(argc, argv);

   std::vector<Genotype> initial_population = initialize_population(population_size);

   std::vector<Genotype> final_population = run_ga(factory, initial_population, max_generations);

   for(Genotype g : final_population)
   {
      std::cout << "---" << std::endl;
      std::cout << *genotype_to_phenotype(g) << std::endl;
   }
   // TODO: Save the final population in a collection of rule files.
}
