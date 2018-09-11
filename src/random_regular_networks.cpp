#include "OneDLattice.hpp"

#include <iostream>
#include <future>
#include <map>
#include <utility>
#include <functional> // std::bind

#define NUM_REPLICAS 100

int num_agents;
double density_step;
int seed;

bool evaluate_ca(OneDLattice& lattice)
{
   double initial_density = lattice.GetDensity();
   for(int i = 0; i < 5000; i++)
   {
      lattice.Shuffle();
      lattice.Step(majority_rule);
      if(!lattice.IsChanging())
      {
         break; // stop evaluate if the ca state stops changing.
      }
   }
   
   if(initial_density < 0.5)
   {
      return lattice.GetDensity() == 0.0;
   }
   else
   {
      return lattice.GetDensity() == 1.0;
   }
}

std::map<double,double> evaluate_radius(int radius)
{
   OneDLattice lattice(num_agents, radius);
   lattice.Seed(seed);

   std::map<double, double> results;
   for(double density = 0.0; density <= 1.001; density += density_step)
   {
      int correct = 0;
      for(int i = 0; i < NUM_REPLICAS; i++)
      {
         lattice.SetDensity(density);
         if(evaluate_ca(lattice))
         {
            correct++;
         }
      }
      results.emplace(std::make_pair(density, (double)correct / (double)NUM_REPLICAS));
   }
   return results;
}

int main(int argc, char** argv)
{
   if(argc != 4) return -1;
   num_agents = atoi(argv[1]);
   density_step = atof(argv[2]);
   seed = atoi(argv[3]);

   std::map<int,std::future<std::map<double,double>>> futures;
   for(int radius = 1; radius <= num_agents/2; radius++)
   {
      futures.emplace(std::make_pair(radius,std::async(std::launch::async | std::launch::deferred,
                                                       std::bind(evaluate_radius, radius))));
   }

   for(auto& future : futures)
   {
      for(auto& result : future.second.get())
      {
         std::cout << future.first << " " << result.first << " " << result.second << std::endl;
      }
   }
   return 0;
}
