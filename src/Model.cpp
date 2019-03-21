#include "Model.hpp"

#include <numeric>   // std::accumulate
#include <algorithm> // std::for_each

Model::Model(double arena_size,
             int num_agents,
             double communication_range,
             int seed,
             double initial_density,
             double agent_speed) :
   _communication_range(communication_range),
   _rng(seed),
   _stats(num_agents),
   _noise(0.0),
   _arena_size(arena_size)
{
   std::uniform_real_distribution<double> coordinate_distribution(-arena_size/2, arena_size/2);
   std::uniform_real_distribution<double> heading_distribution(0, 2*M_PI);
   std::bernoulli_distribution state_distribution(initial_density);
   std::uniform_int_distribution<int> seed_distribution;

   for(int i = 0; i < num_agents; i++)
   {
      Point initial_position(coordinate_distribution(_rng), coordinate_distribution(_rng));
      Heading initial_heading(heading_distribution(_rng));
      Agent a(initial_position, initial_heading, agent_speed, arena_size, seed_distribution(_rng));
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
   _turn_distribution = heading_distribution;
   _step_distribution = std::uniform_int_distribution<int>(1,1);
   _stats.PushState(CurrentDensity(), CurrentNetwork());
}

Model::~Model() {}

void Model::SetPositionalState(double initial_density)
{
   double x_threshold = (_arena_size / 2.0) - (_arena_size * (1.0 - initial_density));
   _stats = ModelStats(_agents.size());
   for(int i = 0; i < _agents.size(); i++)
   {
      if(_agents[i].Position().GetX() <= x_threshold)
      {
         _agent_states[i] = 1;
      }
      else
      {
         _agent_states[i] = 0;
      }
   }
   _stats.PushState(CurrentDensity(), CurrentNetwork());
}

void Model::RecordNetworkDensityOnly()
{
   _stats.NetworkSummaryOnly();
}

double Model::CurrentDensity() const
{
   return std::accumulate(_agent_states.begin(), _agent_states.end(), 0.0) / _agent_states.size();
}

std::shared_ptr<NetworkSnapshot> Model::CurrentNetwork() const
{
   std::shared_ptr<NetworkSnapshot> snapshot = std::make_shared<NetworkSnapshot>(_agents.size());
   for(int i = 0; i < _agents.size(); i++)
   {
      for(int j = i+1; j < _agents.size(); j++)
      {
         if(_agents[i].Position().Within(_communication_range, _agents[j].Position()))
         {
            snapshot->AddEdge(i, j);
         }
      }
   }
   return snapshot;
}

const ModelStats& Model::GetStats() const
{
   return _stats;
}

const std::vector<Agent>& Model::GetAgents() const
{
   return _agents;
}

const std::vector<int>& Model::GetStates() const
{
   return _agent_states;
}

void Model::SetMovementRule(std::shared_ptr<MovementRule> rule)
{
   for(auto& agent : _agents)
   {
      agent.SetMovementRule(rule->Clone());
   }
}

void Model::SetNoise(double p)
{
   _noise_probability = p;
   _noise = std::bernoulli_distribution(fabs(p));
}

int Model::Noise(int i)
{
   if(_noise(_rng))
   {
      return 1 - i;
   }
   else
   {
      return i;
   }
}

void Model::Step(const std::shared_ptr<Rule> rule)
{
   for(Agent& agent : _agents)
   {
      agent.Step();
   }

   std::shared_ptr<NetworkSnapshot> current_network = CurrentNetwork();
   std::vector<int> new_states(_agents.size());
   for(int a = 0; a < _agent_states.size(); a++)
   {
      auto neighbors = current_network->GetNeighbors(a);
      std::vector<int> neighbor_states;
      std::vector<Point> neighbor_positions;
      for(int n : neighbors)
      {
         if(_noise_probability < 0.0) {
            if(!_noise(_rng))
            {
               neighbor_states.push_back(_agent_states[n]);
               neighbor_positions.push_back(_agents[n].Position());
            }
         }
         else
         {
            neighbor_states.push_back(Noise(_agent_states[n]));
            neighbor_positions.push_back(_agents[n].Position());
         }
      }
      new_states[a] = rule->Apply(_agent_states[a], neighbor_states);
   }
   _agent_states = new_states;
   _stats.PushState(CurrentDensity(), current_network);
}
