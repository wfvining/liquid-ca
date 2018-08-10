#include "OneDLattice.hpp"

#include <iostream>

#define NUM_REPLICAS 100

bool evaluate_ca(OneDLattice& lattice)
{
   double initial_density = lattice.GetDensity();
   for(int i = 0; i < 5000; i++)
   {
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

int main(int argc, char** argv)
{
   if(argc != 4) return -1;
   int num_agents = atoi(argv[1]);
   double density_step = atof(argv[2]);
   int seed = atoi(argv[3]);

   for(int radius = 1; radius <= num_agents/2; radius++)
   {
      OneDLattice lattice(num_agents, radius);
      lattice.Seed(seed);

      for(double density = 0.0; density <= 1.01; density += density_step)
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
         std::cout << radius << " "
                   << density << " "
                   << (double)correct / (double)NUM_REPLICAS << std::endl;
      }
   }
   return 0;
}
