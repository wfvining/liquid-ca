#ifndef _MOTION_CA_MODEL_HPP
#define _MOTION_CA_MODEL_HPP

#include <vector>
#include <random>
#include <functional>
#include <memory>

#include "Agent.hpp"
#include "Network.hpp"
#include "Rule.hpp"

/**
 * Statistics about a model including current timestep, current
 * density, density at each previous timestep, and whether or not the
 * classification is correct.
 */
class ModelStats
{
private:
   Network             _network;
   std::vector<double> _ca_density;
   std::vector<double> _network_density;

   bool _network_summary_only = false;

   NetworkSnapshot _aggregate_network;

public:
   ModelStats(int num_agents);
   ~ModelStats();

   /**
    * Record the ca density and the density of the interaction network
    * at the next timestep.
    */
   void PushState(double density, std::shared_ptr<NetworkSnapshot> snapshot);

   /**
    * Don't save the network snapshots, only save the density of each
    * snapshot.
    */
   void NetworkSummaryOnly();

   /**
    * Get the sequence of densities up to this time.
    */
   const std::vector<double>& GetDensityHistory() const;

   /**
    * Get the network up to this time.
    */
   const Network& GetNetwork() const;

   /**
    * Returns a vector of the density of the aggregate network up to
    * the current time step.
    */
   std::vector<double> AggregateDensityHistory() const;

   /**
    * Return true if the density was classified correctly.
    */
   bool IsCorrect() const;

   /**
    * Return true if the model is synchronized.
    */
   bool IsSynchronized() const;

   /**
    * Return the number of timesteps that the model has run for.
    */
   unsigned int ElapsedTime() const;
};

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
   std::function<double(std::mt19937_64&)> _turn_distribution;
   std::function<int(std::mt19937_64&)>    _step_distribution;
   std::bernoulli_distribution             _noise;

   double _communication_range;

   std::shared_ptr<NetworkSnapshot> CurrentNetwork() const;
   int Noise(int i);

public:
   Model(double arena_size, int num_agents, double communication_range,
         int seed, double initial_density, double agent_speed = 1.0);
   ~Model();

   /**
    * Get the current density of the model (ie. proportion of black
    * states to white states).
    */
   double CurrentDensity() const;

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
    * Set the movement rule.
    */
   void SetMovementRule(const MovementRule& rule);

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
   void Step(Rule* rule);

   /**
    * Set the communication range of the agents.
    */
   void SetCommunicationRange(double range);
};

#endif // _MOTION_CA_MODEL_HPP