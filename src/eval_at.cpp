#include "Model.hpp"
#include "LCAFactory.hpp"

#include <algorithm>
#include <map>
#include <utility>
#include <vector>

double mean(std::vector<int>& times)
{
   double t_tot = 0;
   for(int t : times)
   {
      t_tot += t;
   }
   return t_tot / (double)times.size();
}

double median(std::vector<int> times)
{
   std::sort(times.begin(), times.end());
   return times[times.size()/2];
}

double variance(std::vector<int>& times)
{
   double mu = mean(times);
   double sum_squared_error = 0;
   for(int t : times)
   {
      sum_squared_error += (t - mu) * (t - mu);
   }
   return sum_squared_error / (double)times.size();
}

int mode(std::vector<int>& times)
{
   std::map<int, int> frequency;

   for(int t : times)
   {
      frequency[t] += t;
   }

   std::pair<int, int> best = std::make_pair(-1, -1);
   for(std::pair<int, int> t : frequency)
   {
      if(t.second > best.second)
         best = t;
   }

   return best.first;
}

int main(int argc, char** argv)
{
   LCAFactory factory;

   int arg_index = factory.Init(argc, argv);
   double initial_density = atof(argv[arg_index]);
   int num_correct = 0;

   std::vector<int> times;

   for(int i = 0; i < 100; i++)
   {
      std::unique_ptr<LCA> lca = factory.Create(initial_density);
      lca->MinimizeMemory();
      int time = lca->Run([](const ModelStats& s) {
                             return (s.CurrentCADensity() == 0.0 || s.CurrentCADensity() == 1.0);
                          });

      times.push_back(time);
      if(lca->GetStats().IsCorrect())
      {
         num_correct++;
      }
   }

   std::cout << initial_density << " "
             << num_correct << " "
             << mean(times) << " "
             << median(times) << " "
             << variance(times) << " "
             << mode(times) << std::endl;
}
