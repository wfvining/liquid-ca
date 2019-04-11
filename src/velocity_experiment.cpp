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
#include "LCAFactory.hpp"

LCAFactory factory;

int num_iterations;

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

double evaluate_ca(int num_iterations, double initial_density)
{
   int num_correct = 0;
   for(int iteration = 0; iteration < num_iterations; iteration++)
   {
      std::unique_ptr<LCA> lca = factory.Create(initial_density);
      lca->MinimizeMemory();
      lca->Run([](const ModelStats& s) { return (s.CurrentCADensity() == 0.0 || s.CurrentCADensity() == 1.0); });

      if(lca->GetStats().IsCorrect())
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
      double proportion_correct = evaluate_ca(num_iterations, initial_density);
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

   int arg_index = factory.Init(argc, argv);
   num_iterations = atoi(argv[arg_index]);

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
