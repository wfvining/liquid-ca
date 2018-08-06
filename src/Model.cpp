#include "Model.hpp"

#include <numeric>   // std::accumulate
#include <algorithm> // std::for_each

/// ModelStats functions

ModelStats::ModelStats() {}
ModelStats::~ModelStats() {}

void ModelStats::PushState(double density, const NetworkSnapshot& snapshot)
{
   _network.AppendSnapshot(snapshot);
   _ca_density.push_back(density);
}

/// Model functions

Model::Model(double arena_size,
             int num_agents,
             double communication_range,
             int seed,
             double initial_density) :
   _communication_range(communication_range),
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

   _stats.PushState(CurrentDensity(), CurrentNetwork());
}

Model::~Model() {}

double Model::CurrentDensity() const
{
   return std::accumulate(_agent_states.begin(), _agent_states.end(), 0.0) / _agent_states.size();
}

NetworkSnapshot Model::CurrentNetwork() const
{
   NetworkSnapshot snapshot(_agents.size());
   for(int i = 0; i < _agents.size(); i++)
   {
      for(int j = i+1; j < _agents.size(); j++)
      {
         if(_agents[i].Position().Within(_communication_range, _agents[j].Position()))
         {
            snapshot.AddEdge(i, j);
         }
      }
   }
   return snapshot;
}

const ModelStats& Model::GetStats() const
{
   return _stats;
}

void Model::Step()
{
   std::for_each(_agents.begin(), _agents.end(),
                 [](Agent& agent) { agent.Step(); });
   _stats.PushState(CurrentDensity(), CurrentNetwork());
}
