#include <random>
#include <memory>
#include <vector>
#include <utility>
#include <set>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <thread>

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
      t0.include_self = true;
      t0.pre_state = 0;
      t0.heading_change = heading_distribution(rng);
      t0.result_state = state_distribution(rng);
      g.zero_transitions.push_back(t0);

      for(double x = 0.125; x < 1.0; x += 0.125)
      {
         g.zero_transitions = split_at(x,
                                       g.zero_transitions,
                                       coin(rng),
                                       state_distribution(rng),
                                       heading_distribution(rng));
      }

      for(double x = 0.125; x < 1.0; x += 0.125)
      {
         g.one_transitions = split_at(x,
                                      g.one_transitions,
                                      coin(rng),
                                      state_distribution(rng),
                                      heading_distribution(rng));
      }
      pop.push_back(g);
   }
   return pop;
}

std::vector<std::unique_ptr<LCA>> lcas;
int lca_i;
std::mutex lcas_lock;

int thread_main()
{
   lcas_lock.lock();
   while(lca_i < lcas.size())
   {
      int i = lca_i++;
      lcas_lock.unlock();
      lcas[i]->MinimizeMemory();
      lcas[i]->Run([](const ModelStats& s) { return (s.CurrentCADensity() == 0.0 || s.CurrentCADensity() == 1.0); });
      lcas_lock.lock();
   }
   lcas_lock.unlock();
   return 0;
}

double evaluate_rule(LCAFactory& factory)
{
   int num_correct = 0;
   lcas_lock.lock();
   lca_i = 0;
   lcas.clear();
   for(double i = 0.1; i < 1.0; i+=0.1)
   {
      if(i == 0.5) continue;
      lcas.push_back(factory.Create(i));
   }
   for(int i = 0; i < 15; i++)
   {
      lcas.push_back(factory.Create(0.45));
      lcas.push_back(factory.Create(0.55));
   }
   lcas_lock.unlock();

   // start all threads
   std::vector<std::thread> threads;
   for(int i = 0; i < std::thread::hardware_concurrency(); i++)
   {
      threads.push_back(std::thread(thread_main));
   }
   // wait for all threads to finish
   for(std::thread& t : threads)
   {
      t.join();
   }

   for(int i = 0; i < lcas.size(); i++)
   {
      if(lcas[i]->GetStats().IsCorrect())
      {
         num_correct++;
      }
   }
   return (double)num_correct / (double)lcas.size();
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
   Genotype parent_a = tournament(10, population);
   Genotype parent_b = tournament(10, population);
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
   std::uniform_int_distribution<int> xover_point_distribution(0,8);
   Genotype child;

   child.zero_transitions = merge_at(xover_point_distribution(rng)*0.125,
                                     parents.first.zero_transitions,
                                     parents.second.zero_transitions);
   child.one_transitions = merge_at(xover_point_distribution(rng)*0.125,
                                    parents.first.one_transitions,
                                    parents.second.one_transitions);

   return child;
}

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
   //   2. a "RESIZE" mutation which incrementally grows/shrinks the               <------
   //   transition interval (taking range away from adjacent intervals)
   //
   //   3. a "MERGE" mutation which combines two adjacent transitions
   //   with the same result state and combines them taking the average
   //   heading change of the two original transitions.

   std::uniform_real_distribution<double> p(0,1);
   std::normal_distribution<double> heading_change(0, 1.0);

   for(int i = 0; i < g.zero_transitions.size(); i++)
   {
      if(p(rng) < 0.025)
      {
         g.zero_transitions[i].result_state = 1-g.zero_transitions[i].result_state;
      }

      if(p(rng) < 0.025)
      {
         g.one_transitions[i].result_state = 1-g.one_transitions[i].result_state;
      }

      if(p(rng) < 0.1)
      {
         g.zero_transitions[i].heading_change += heading_change(rng);
      }

      if(p(rng) < 0.1)
      {
         g.one_transitions[i].heading_change += heading_change(rng);
      }
   }

   return g;
}

Genotype rule_to_genotype(TotalisticRule& r)
{
   Genotype g;
   std::vector<Transition> transitions = r.GetTransitions();
   for(Transition& t : r.GetTransitions())
   {
      if(t.pre_state == 0)
      {
         g.zero_transitions.push_back(t);
      }
      else
      {
         g.one_transitions.push_back(t);
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

      // 0. Save all genotyes into a file
      std::ostringstream dir;
      dir << "generation" << generation;
      mkdir(dir.str().c_str(), 0777);
      for(int i = 0; i < current_population.size(); i++)
      {
         std::ostringstream f;
         f << dir.str() << "/genome_" << i;
         std::ofstream out(f.str(), std::ofstream::out|std::ofstream::trunc);
         out << *genotype_to_phenotype(current_population[i]);
      }

      // 1. evaluate each member of the population K times on K different
      // initial conditions.
      double best_fitness = 0;
      Genotype best_genotype;
      for(Genotype g : current_population)
      {
         factory.SetRule(genotype_to_phenotype(g));
         double fitness = evaluate_rule(factory);
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
