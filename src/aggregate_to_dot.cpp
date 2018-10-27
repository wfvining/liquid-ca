#include <random>
#include <iostream>
#include <cstdlib>
#include <functional>
#include <algorithm> // std::fill()
#include <cmath>
#include <utility>
#include <map>
#include <fstream>

#include <getopt.h>

#include "Model.hpp"

struct model_config
{
   int    num_agents;
   double communication_range;
   int    arena_size;
   int    seed;
   double mu;
} model_config;

void network_statistics(int num_iterations, double speed)
{
   std::uniform_real_distribution<double> heading_distribution(0, 2*M_PI);
   std::vector<double> degree_distribution(model_config.num_agents);
   std::fill(degree_distribution.begin(), degree_distribution.end(), 0.0);
   Model m(model_config.arena_size,
           model_config.num_agents,
           model_config.communication_range,
           model_config.seed,
           0.5,
           speed);

   m.SetMovementRule(std::make_shared<LevyWalk>(model_config.mu, model_config.arena_size/speed));

   for(int step = 0; step < 5000; step++)
   {
      m.Step(majority_rule);
   }

   NetworkSnapshot aggregate = m.GetStats().GetNetwork().Aggregate();

   std::cout << aggregate << std::endl;
}

int main(int argc, char** argv)
{
   int    opt_char;
   int    sweep_density   = 0;
   double density_step    = 0.01;
   double initial_density = 0.0;
   int    num_iterations  = 1;
   int    save_state      = 0;

   model_config.communication_range = 5;
   model_config.num_agents          = 100;
   model_config.arena_size          = 100;
   model_config.seed                = 1234;
   model_config.mu                  = 1.2;

   static struct option long_options[] =
      {
         {"initial-density",     required_argument, 0,            'd'},
         {"communication-range", required_argument, 0,            'r'},
         {"num-agents",          required_argument, 0,            'n'},
         {"arena-size",          required_argument, 0,            'a'},
         {"seed",                required_argument, 0,            's'},
         {"iterations",          required_argument, 0,            'i'},
         {"mu",                  required_argument, 0,            'm'},
         {0,0,0,0}
      };

   int option_index = 0;

   while((opt_char = getopt_long(argc, argv, "m:d:r:n:a:s:i:",
                                 long_options, &option_index)) != -1)
   {
      switch(opt_char)
      {
      case 'd':
         initial_density = atof(optarg);
         break;

      case 'r':
         model_config.communication_range = atof(optarg);
         break;

      case 'n':
         model_config.num_agents = atoi(optarg);
         break;

      case 'a':
         model_config.arena_size = atof(optarg);
         break;

      case 's':
         model_config.seed = atoi(optarg);
         break;

      case 'i':
         num_iterations = atoi(optarg);
         break;

      case 'm':
         model_config.mu = atof(optarg);
         break;

      case ':':
         std::cout << "option " << long_options[option_index].name << "requires an argument" << std::endl;
         exit(-1);
         break;

      case '?':
         std::cout << "unrecognized option" << std::endl;
         exit(-1);
      }
   }

   if(optind >= argc)
   {
      std::cout << "missing required argument <agent-speed>" << std::endl;
   }

   double speed = atof(argv[optind]);

   network_statistics(num_iterations, speed);
}
