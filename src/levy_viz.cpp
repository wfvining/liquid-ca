#include <random>
#include <iostream>
#include <cstdlib>
#include <functional>
#include <cmath>
#include <map>
#include <fstream>

#include <getopt.h>

#include "Model.hpp"
#include "MovementRule.hpp"

struct model_config
{
   int num_agents;
   int communication_range;
   int arena_size;
   int seed;
   double mu;
   double speed;
} model_config;

double heading(std::mt19937_64& gen)
{
   std::uniform_real_distribution<double> heading_distribution(0, 2*M_PI);
   return heading_distribution(gen);
}

void evaluate_ca(double initial_density)
{
   std::uniform_real_distribution<double> heading_distribution(0, 2*M_PI);
   Model m(model_config.arena_size,
           model_config.num_agents,
           model_config.communication_range,
           model_config.seed,
           initial_density,
           model_config.speed);

//   m.SetMovementRule(LevyWalk(model_config.mu, model_config.arena_size));
   m.SetMovementRule(std::make_shared<CorrelatedRandomWalk>(model_config.mu));
   
   for(int step = 0; step < 2000; step++)
   {
      m.Step(majority_rule);
      for(auto agent : m.GetAgents())
      {
         std::cout << agent.Position().GetX() << " " << agent.Position().GetY() << " ";
      }
      std::cout << std::endl;
   }
}

int main(int argc, char** argv)
{
   int    opt_char;
   int    sweep_density   = 0;
   double density_step    = 0.01;
   double initial_density = 0.5;
   int    num_iterations  = 1;
   int    save_state      = 0;

   model_config.communication_range = 5;
   model_config.num_agents          = 100;
   model_config.arena_size          = 100;
   model_config.seed                = 1234;
   model_config.speed               = 1;

   static struct option long_options[] =
      {
         {"sweep-density",       optional_argument, &sweep_density, 1},
         {"initial-density",     required_argument, 0,            'd'},
         {"communication-range", required_argument, 0,            'r'},
         {"num-agents",          required_argument, 0,            'n'},
         {"arena-size",          required_argument, 0,            'a'},
         {"seed",                required_argument, 0,            's'},
         {"iterations",          required_argument, 0,            'i'},
         {"speed",               required_argument, 0,            'S'},
         {0,0,0,0}
      };

   int option_index = 0;

   while((opt_char = getopt_long(argc, argv, "m:d:r:n:a:s:i:",
                                 long_options, &option_index)) != -1)
   {

      switch(opt_char)
      {
      case 0:
         initial_density = 0.0;
         if(optarg)
         {
            density_step = atof(optarg);
         }
         break;

      case 'S':
         model_config.speed = atof(optarg);
         break;
         
      case 'd':
         initial_density = atof(optarg);
         break;

      case 'r':
         model_config.communication_range = atoi(optarg);
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
      std::cout << "missing required option <alpha>" << std::endl;
   }

   double alpha    = atof(argv[optind]);
   model_config.mu = alpha + 1;

   evaluate_ca(0.5);
}
