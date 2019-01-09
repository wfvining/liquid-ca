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

struct result
{
   int t;
   double avg_degree;
   double std_dev;
};

std::mutex results_lock;
std::vector<result> results;

std::mutex seed_lock;
int seed = 0;

int next_seed()
{
   int s;
   seed_lock.lock();
   s = seed;
   seed++;
   seed_lock.unlock();
   return s;
}

void record_result(result result)
{
   results_lock.lock();
   results.push_back(result);
   results_lock.unlock();
}

result evaluate_ca(int num_iterations, double speed, double initial_density)
{
   int num_correct = 0;

   Model m(model_config.arena_size,
           model_config.num_agents,
           model_config.communication_range,
           model_config.seed+next_seed(),
           initial_density,
           speed);

   m.SetMovementRule(model_config.movement_rule);
   m.RecordNetworkDensityOnly();

   int step;
   for(step = 0; step < model_config.max_time; step++)
   {
      m.Step(model_config.rule);
      if(m.CurrentDensity() == 0 || m.CurrentDensity() == 1)
      {
         break; // done. no need to keep evaluating.
      }
   }

   if(step < model_config.max_time)
   {
      auto stats = m.GetStats();
      return result { step,
            stats.AverageAggregateDegree(),
            stats.AggregateDegreeStdDev()
            };
   }
   else return result {-1, 0, 0};
}

void thread_main()
{
   int total_iterations = 0;
   while(total_iterations < 10) // XXX: assumes there are exactly 10 threads
   {
      result r = evaluate_ca(model_config.num_iterations,
                             model_config.speed,
                             0.5);
      if(r.t >= 0)
      {
         record_result(r);
      }
      total_iterations++;
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

   //int max_threads = std::thread::hardware_concurrency();
   int max_threads = 10;
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
      std::cout << result.t << " " << result.avg_degree << " " << result.std_dev << std::endl;
   }
}
