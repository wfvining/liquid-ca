#include <random>
#include <iostream>
#include <cstdlib>
#include <functional>
#include <cmath>
#include <utility>
#include <map>
#include <fstream>
#include <thread>

#include <getopt.h>

#include "Model.hpp"
#include "Rule.hpp"

struct model_config
{
   int    num_agents;
   double communication_range;
   int    arena_size;
   int    seed;
   double mu;
   int    num_iterations;
} model_config;

std::mutex density_lock;
double init_density = 0.0;

std::mutex results_lock;
std::map<double, double> results;

double next_density()
{
   double d;
   
   density_lock.lock();
   d = init_density;
   init_density += 0.01;
   density_lock.unlock();

   return d;
}

void store_result(double initial_density, double proportion_correct)
{
   results_lock.lock();
   results.emplace(initial_density, proportion_correct);
   results_lock.unlock();
}

std::shared_ptr<NetworkSnapshot> make_spatial(int seed)
{
   Model m(model_config.arena_size, model_config.num_agents,
           model_config.communication_range,
           seed, 0.0);
   return m.GetStats().GetNetwork().GetSnapshot(0);
}

std::vector<int> eval_network(Rule* rule,
                              std::shared_ptr<NetworkSnapshot> n,
                              const std::vector<int>& states)
{
   std::vector<int> new_states(states.size());
   for(int a = 0; a < states.size(); a++)
   {
      auto neighbors = n->GetNeighbors(a);
      // XXX: wouldn't it be nice if I could just do Rule.Apply(neighbors) here?
      std::vector<int> neighbor_states;
      for(int n : neighbors)
      {
         neighbor_states.push_back(states[n]);
      }
      new_states[a] = rule(states[a], neighbor_states);
   }
   return new_states;
}

double density(std::vector<int>& states)
{
   return (double)std::accumulate(states.begin(), states.end(), 0)/(double)states.size();
}

std::vector<int> initialize_state(double initial_density, std::mt19937_64& gen)
{
   std::uniform_real_distribution<double> u(0,1);
   std::vector<int> states;
   for(int i = 0; i < model_config.num_agents; i++)
   {
      if(u(gen) < initial_density) states.push_back(1);
      else states.push_back(0);
   }
   return states;
}

bool evaluate_ca(int seed, double initial_density)
{
   std::uniform_int_distribution<int> u;
   std::mt19937_64 gen(seed);
   std::vector<int> agent_states = initialize_state(initial_density, gen);
   double actual_density = density(agent_states);
   for(int i = 0; i < 5000; i++)
   {
      auto network = make_spatial(u(gen));
      agent_states = eval_network(majority_rule, network, agent_states);
      double current_density = density(agent_states);
      if(current_density == 1 || current_density == 0)
      {
         break;
      }
   }
   return density(agent_states) == round(actual_density);
}

void run_sweep()
{
   double d;
   while((d = next_density()) < 1.001)
   {
      int correct = 0;
      for(int replica = 0; replica < model_config.num_iterations; replica++)
      {
         if(evaluate_ca(model_config.seed+replica, d))
         {
            correct++;
         }
      }
      store_result(d, (double)correct / (double)model_config.num_iterations);
   }
}

int main(int argc, char** argv)
{
   int    opt_char;
   int    sweep_density   = 0;
   double density_step    = 0.01;
   double initial_density = 0.0;
   int    num_iterations  = 100;
   int    save_state      = 0;

   model_config.communication_range = 5;
   model_config.num_agents          = 100;
   model_config.arena_size          = 100;
   model_config.seed                = 1234;
   model_config.mu                  = 1.2;
   model_config.num_iterations      = 100;

   static struct option long_options[] =
      {
         {"initial-density",     required_argument, 0,            'd'},
         {"communication-range", required_argument, 0,            'r'},
         {"num-agents",          required_argument, 0,            'n'},
         {"arena-size",          required_argument, 0,            'a'},
         {"seed",                required_argument, 0,            's'},
         {"iterations",          required_argument, 0,            'i'},
         {0,0,0,0}
      };

   int option_index = 0;

   while((opt_char = getopt_long(argc, argv, "d:r:n:a:s:i:",
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

      case ':':
         std::cout << "option " << long_options[option_index].name << "requires an argument" << std::endl;
         exit(-1);
         break;

      case '?':
         std::cout << "unrecognized option" << std::endl;
         exit(-1);
      }
   }

   int num_threads = std::thread::hardware_concurrency();
   std::vector<std::thread> threads;
   for(int i = 0; i < num_threads; i++)
   {
      threads.push_back(std::thread(run_sweep));
   }

   for(auto& thread : threads)
   {
      thread.join();
   }

   for(auto result : results)
   {
      std::cout << result.first << " " << result.second << std::endl;
   }
   
   return 0;
}
