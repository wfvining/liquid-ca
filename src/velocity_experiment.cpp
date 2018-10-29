#include <random>
#include <iostream>
#include <cstdlib>
#include <functional>
#include <cmath>
#include <future>
#include <thread> // hardware_concurrency()
#include <chrono>
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
   double speed;
   double num_iterations;
   Rule*  rule;
   std::shared_ptr<MovementRule> movement_rule;
   int    max_time;
} model_config;

std::mutex density_lock;
double density = 0.0;

std::mutex results_lock;
std::map<double, double> results;

double next_density()
{
   double d;

   density_lock.lock();

   d = density;
   density += 0.01;

   density_lock.unlock();

   return d;
}

void record_result(double initial_density, double proportion_correct)
{
   results_lock.lock();
   results.emplace(initial_density, proportion_correct);
   results_lock.unlock();
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

      // m.SetMovementRule(LevyWalk(model_config.mu, model_config.arena_size/speed));
      m.SetMovementRule(model_config.movement_rule);
      m.RecordNetworkDensityOnly();

      for(int step = 0; step < model_config.max_time; step++)
      {
         m.Step(model_config.rule);
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

void thread_main()
{
   double initial_density;
   while((initial_density = next_density()) < 1.001)
   {
      double proportion_correct = evaluate_ca(model_config.num_iterations,
                                              model_config.speed,
                                              initial_density);
      record_result(initial_density, proportion_correct);
   }
}

int main(int argc, char** argv)
{
   int    opt_char;
   int    sweep_density   = 0;
   double density_step    = 0.01;
   double initial_density = 0.0;
   int    save_state      = 0;

   model_config.communication_range = 5;
   model_config.num_agents          = 100;
   model_config.arena_size          = 100;
   model_config.seed                = 1234;
   model_config.mu                  = 1.2;
   model_config.num_iterations      = 100;
   model_config.movement_rule       = std::make_shared<RandomWalk>();
   model_config.rule                = majority_rule;
   model_config.max_time            = 5000;

   static struct option long_options[] =
      {
         {"initial-density",     required_argument, 0,            'd'},
         {"communication-range", required_argument, 0,            'r'},
         {"num-agents",          required_argument, 0,            'n'},
         {"arena-size",          required_argument, 0,            'a'},
         {"seed",                required_argument, 0,            's'},
         {"iterations",          required_argument, 0,            'i'},
         {"mu",                  required_argument, 0,            'm'},
         {"rule",                required_argument, 0,            'R'},
         {"correlated",          required_argument, 0,            'c'},
         {"max-time",            required_argument, 0,            'T'},
         {0,0,0,0}
      };

   int option_index = 0;

   while((opt_char = getopt_long(argc, argv, "m:d:r:n:a:s:i:c:R:T:",
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
         model_config.num_iterations = atoi(optarg);
         break;

      case 'm':
         model_config.mu = atof(optarg);
         break;

      case 'T':
         model_config.max_time = atoi(optarg);
         break;

      case 'R':
         if(std::string(optarg) == "gkl")
         {
            model_config.rule = gkl2d_strict;
         }
         else if(std::string(optarg) == "gkl-lax")
         {
            model_config.rule = gkl2d_lax;
         }
         else if(std::string(optarg) == "majority")
         {
            model_config.rule = majority_rule;
         }
         else if(std::string(optarg) == "gkl-mode")
         {
            model_config.rule = gkl2d_mode;
         }
         else
         {
            std::cout << "invalid rule (" << std::string(optarg) << ")" << std::endl;
            exit(-1);
         }
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
   }

   model_config.speed = atof(argv[optind]);

   int max_threads = std::thread::hardware_concurrency();
   std::vector<std::thread> threads;

   for(int i = 0; i < max_threads; i++)
   {
      threads.push_back(std::thread(thread_main));
   }

   for(auto& thread : threads)
   {
      thread.join();
   }

   // print the results
   for(auto result : results)
   {
      std::cout << result.first << " " << result.second << std::endl;
   }
}
