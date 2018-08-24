#include <random>
#include <iostream>
#include <cstdlib>
#include <functional>
#include <cmath>
#include <future>
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

int levy_flight_step(double mu, int max_step, std::mt19937_64& gen)
{
   std::uniform_real_distribution<double> u(0,1);
   double pmin = powf(1.0, -mu+1);
   double pmax = powf((double)max_step, -mu+1);
   double z    = powf((pmax - pmin)*u(gen) + pmin, 1.0/(-mu+1));
   int x = floor(z);
   return x;
}

double evaluate_ca(int num_iterations, double speed, double initial_density)
{
   std::uniform_real_distribution<double> heading_distribution(0, 2*M_PI);
   int num_correct = 0;
   for(int iteration = 0; iteration < num_iterations; iteration++)
   {
      Model m(model_config.arena_size,
              model_config.num_agents,
              model_config.communication_range,
              model_config.seed+iteration,
              initial_density,
              speed);

      m.SetTurnDistribution(heading_distribution);
      m.SetStepDistribution(std::bind(levy_flight_step,
                                      model_config.mu,
                                      model_config.arena_size/speed, // this means an agent can travel
                                                                     // at furthest from one end of the
                                                                     // arena to another before
                                                                     // turning.
                                      std::placeholders::_1));

      for(int step = 0; step < 5000; step++)
      {
         m.Step(majority_rule);
         if(m.CurrentDensity() == 0 || m.CurrentDensity() == 1)
         {
            break; // done. no need to keep evaluating.
         }
      }

      if(m.GetStats().IsCorrect())
      {
         num_correct++;
      }
   }

   return (double) num_correct / num_iterations;
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
   model_config.mu                  = 1.2;

   static struct option long_options[] =
      {
         {"sweep-density",       optional_argument, &sweep_density, 1},
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
      case 0:
         initial_density = 0.0;
         if(optarg)
         {
            density_step = atof(optarg);
         }
         break;

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

   std::map<double, std::future<double>> results;

   while(initial_density <= 1.001)
   {
      results.emplace(initial_density, std::async(std::launch::async | std::launch::deferred,
                                                  std::bind(evaluate_ca, num_iterations, speed, initial_density)));

      if(sweep_density == 1)
      {
         initial_density += density_step;
      }
      else
      {
         break; // exit the loop if a sweep has not been requested.
      }
   }

   for(auto& result : results)
   {
      std::cout << result.first << " " << result.second.get() << std::endl;
   }
}
