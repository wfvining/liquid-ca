#include "Model.hpp"

#include <random>
#include <iostream>
#include <cstdlib>
#include <functional>
#include <cmath>

int levy_flight_step(double mu, int max_step, std::mt19937_64& gen)
{
   std::uniform_real_distribution<double> u(0,1);
   return (int)round(pow((pow(max_step, mu + 1) - 1)*u(gen) + 1, 1.0/(mu + 1)));
}

int main(int argc, char** argv)
{
   if(argc != 8)
   {
      std::cout << "Wrong number of arguments: "
                << std::endl
                << argv[0] << " <alpha> <max-step> <comm-range> <num-agents> <arena-size> <seed> <initial-density>"
                << std::endl;
      return -1;
   }

   double alpha    = atof(argv[1]);
   double mu       = alpha + 1.0;
   int    max_step = atoi(argv[2]);
   int    communication_range = atoi(argv[3]);
   int    num_agents          = atoi(argv[4]);
   int    arena_size          = atoi(argv[5]);
   int    seed                = atoi(argv[6]);
   double initial_density     = atof(argv[7]);

   std::uniform_real_distribution<double> heading_distribution(0, 2*M_PI);
   Model m(arena_size, num_agents, communication_range, seed, initial_density);
   m.SetTurnDistribution(heading_distribution);
   m.SetStepDistribution(std::bind(levy_flight_step, mu, max_step, std::placeholders::_1));

   for(int i = 0; i < 5000; i++)
   {
      m.Step(majority_rule);
      std::cout << m.CurrentDensity() << std::endl;
      if(m.CurrentDensity() == 0 || m.CurrentDensity() == 1)
      {
         break; // done. no need to keep evaluating.
      }
   }
}
