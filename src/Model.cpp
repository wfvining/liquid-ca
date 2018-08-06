#include "Model.hpp"

#include <numeric> // std::accumulate

ModelStats::ModelStats() {}
ModelStats::~ModelStats() {}

Model::Model(int seed,
             double arena_size,
             int num_agents,
             double initial_density) :
   _rng(seed)
{
   std::uniform_real_distribution<double> coordinate_distribution(-arena_size/2, arena_size/2);
   std::uniform_real_distribution<double> heading_distribution(0, 2*M_PI);
   std::bernoulli_distribution state_distribution(initial_density);

   for(int i = 0; i < num_agents; i++)
   {
      Point initial_position(coordinate_distribution(_rng), coordinate_distribution(_rng));
      Heading initial_heading(heading_distribution(_rng));
      Agent a(initial_position, initial_heading, 1.0, arena_size);
      _agents.push_back(a);
      if(state_distribution(_rng))
      {
         _agent_states.push_back(1);
      }
      else
      {
         _agent_states.push_back(0);
      }
   }
}

Model::~Model() {}

double Model::CurrentDensity() const
{
   return std::accumulate(_agent_states.begin(), _agent_states.end(), 0.0) / _agent_states.size();
}
