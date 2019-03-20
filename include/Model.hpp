#ifndef _MOTION_CA_MODEL_HPP
#define _MOTION_CA_MODEL_HPP

#include <vector>
#include <random>
#include <functional>
#include <memory>

#include "Agent.hpp"
#include "Network.hpp"
#include "Rule.hpp"
#include "ModelStats.hpp"

/**
 * The model of moving agents.
 */
class Model
{
private:
   ModelStats         _stats;

   std::vector<Agent> _agents;
   std::vector<int>   _agent_states;
   int                _steps;

   std::mt19937_64 _rng;
   std::bernoulli_distribution             _noise;
   double                                  _noise_probability;

   double _communication_range;

   int Noise(int i);

public:
   Model(int num_agents, double communication_range,
         int seed, double initial_density, double agent_speed = 1.0,
         std::unique_ptr<Surface> surface);
   ~Model();

   /**
    * Reinitialize the model with states set according to x-coordinate
    * of each agent. Any agent to the left of x = 0 -
    * initial_density*(arena_size/2) will be in state 1; all agents to
    * the right will be in state 0.
    */
   void SetPositionalState(double initial_density);

   /**
    * Get the current density of the model (ie. proportion of black
    * states to white states).
    */
   double CurrentDensity() const;

   std::shared_ptr<NetworkSnapshot> CurrentNetwork() const;

   /**
    * Get the density of the communication network.
    */
   double NetworkDensity() const;

   /**
    * Save the network density only.
    */
   void RecordNetworkDensityOnly();

   /**
    * Get statistics about the model.
    */
   const ModelStats& GetStats() const;

   /**
    * Get the agents from the model.
    */
   const std::vector<Agent>& GetAgents() const;

   /**
    * Get the current states of the agents
    */
   const std::vector<int>& GetStates() const;

   /**
    * Set the movement rule.
    */
   void SetMovementRule(std::shared_ptr<MovementRule> rule);

   /**
    * Set the turn distribution.
    */
   void SetTurnDistribution(std::function<double(std::mt19937_64&)> turn_distribution);

   /**
    * Set the step distribution.
    */
   void SetStepDistribution(std::function<int(std::mt19937_64&)> step_distribution);

   /**
    * Set the amount of noise. p is a real number in [0,1].
    */
   void SetNoise(double p);

   /**
    * Evaluate the model for one time-step.
    */
   void Step(const Rule* rule);

   /**
    * Set the communication range of the agents.
    */
   void SetCommunicationRange(double range);
};

#endif // _MOTION_CA_MODEL_HPP
