#include <iostream>
#include <getopt.h>

#include "Model.hpp"

struct model_config
{
   int    num_agents;
   double communication_range;
   int    arena_size;
   int    seed;
   double mu;
   double speed;
   double num_iterations;
   std::shared_ptr<MovementRule> movement_rule;
} model_config;

void density_history()
{
   Model m(model_config.arena_size,
           model_config.num_agents,
           model_config.communication_range,
           model_config.seed,
           0.5,
           model_config.speed);
   m.SetMovementRule(model_config.movement_rule);

   for(int i = 0l; i < 5000; i++)
   {
      m.Step(majority_rule);
      auto agg_degree = m.GetStats().AverageAggregateDegree();
      auto agg_stddev = m.GetStats().AggregateDegreeStdDev();
      std::cout << agg_degree << " " << agg_stddev << std::endl;
   }

   // for(auto d : m.GetStats().AggregateDensityHistory())
   // {
   //    std::cout << d << std::endl;
   // }
}

int main(int argc, char **argv)
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
   model_config.movement_rule       = std::make_shared<RandomWalk>();

   static struct option long_options[] =
      {
         {"communication-range", required_argument, 0,            'r'},
         {"num-agents",          required_argument, 0,            'n'},
         {"arena-size",          required_argument, 0,            'a'},
         {"seed",                required_argument, 0,            's'},
         {"correlated",          required_argument, 0,            'c'},
         {0,0,0,0}
      };

   int option_index = 0;

   while((opt_char = getopt_long(argc, argv, "m:d:r:n:a:s:i:",
                                 long_options, &option_index)) != -1)
   {
      switch(opt_char)
      {
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

      case 'c':
         model_config.movement_rule = std::make_shared<CorrelatedRandomWalk>(atof(optarg));
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
      exit(-1);
   }

   model_config.speed = atof(argv[optind]);
   density_history();
   return 0;
}
