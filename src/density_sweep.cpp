#include <random>
#include <iostream>
#include <cstdlib> // atoi
#include <functional>
#include <cmath>
#include <thread> // hardware_concurrency()
#include <mutex>
#include <chrono>
#include <utility>
#include <map>
#include <fstream>

#include <getopt.h>

#include "Model.hpp"

struct config {
   int    num_agents;
   double communication_range;
   double speed;
   int    seed;
   int    num_iterations;
} model_config;

std::mutex density_lock;
double next_density = 0.05;
const double DENSITY_STEP = 0.05;
const double MAX_DENSITY  = 4.0;

std::mutex output_lock;

double evaluate_ca(int num_iterations, double speed, double initial_density, int arena_size)
{
   std::uniform_real_distribution<double> heading_distribution(0, 2*M_PI);
   int num_correct = 0;
   for(int iteration = 0; iteration < num_iterations; iteration++)
   {
      Model m(arena_size,
              model_config.num_agents,
              model_config.communication_range,
              model_config.seed+iteration,
              initial_density,
              speed);

      m.SetMovementRule(std::make_shared<RandomWalk>());
      m.RecordNetworkDensityOnly();

      for(int step = 0; step < 2500; step++)
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

void evaluate_density(double agent_density)
{
   double arena_size = sqrt(model_config.num_agents / agent_density);
   std::map<double, double> results;
   for(double state_density = 0.0; state_density <= 1.001; state_density += 0.02)
   {
      double proportion_correct = evaluate_ca(model_config.num_iterations, model_config.speed,
                                              state_density, arena_size);
      results.emplace(state_density, proportion_correct);
   }

   output_lock.lock(); // lock to avoid interleaving output :/
   for(auto result : results)
   {
      std::cout << agent_density << " "
                << result.first  << " "
                << result.second
                << std::endl;
   }
   std::cout << std::endl;
   output_lock.unlock();
}

void thread_main()
{
   while(true)
   {
      density_lock.lock();

      double density = next_density;
      next_density += DENSITY_STEP;

      density_lock.unlock();

      if(density <= MAX_DENSITY)
      {
         evaluate_density(density);
      }
      else
      {
         return;
      }
   }
}

int main(int argc, char** argv)
{
   int    opt_char;
   int    sweep_density   = 0;
   double density_step    = 0.01;
   double initial_density = 0.0;
   int    save_state      = 0;

   model_config.communication_range = 1;
   model_config.num_agents          = 100;
   model_config.num_iterations      = 100;
   model_config.seed                = 1234;

   static struct option long_options[] =
      {
         {"num-agents",          required_argument, 0,            'n'},
         {"arena-size",          required_argument, 0,            'a'},
         {"seed",                required_argument, 0,            's'},
         {"iterations",          required_argument, 0,            'i'},
         {0,0,0,0}
      };

   int option_index = 0;

   while((opt_char = getopt_long(argc, argv, "m:d:n:s:",
                                 long_options, &option_index)) != -1)
   {
      switch(opt_char)
      {

      case 'n':
         model_config.num_agents = atoi(optarg);
         break;

      case 's':
         model_config.seed = atoi(optarg);
         break;

      case 'i':
         model_config.num_iterations = atoi(optarg);
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

   int num_threads = std::thread::hardware_concurrency();

   std::vector<std::thread> threads;

   for(int i = 0; i < num_threads; i++)
   {
      threads.push_back(std::thread(thread_main));
   }

   for(auto& t : threads)
   {
      t.join();
   }

   return 0;
}
