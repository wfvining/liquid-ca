#ifndef _MOTION_CA_MODEL_HPP
#define _MOTION_CA_MODEL_HPP

#include <vector>
#include <random>

#include "Agent.hpp"
#include "Network.hpp"

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

public:
   ModelStats();
   ~ModelStats();

   /**
    * Record the ca density and the density of the interaction network
    * at the next timestep.
    */
   void PushState(double density, const NetworkSnapshot& snapshot);

   /**
    * Get the sequence of densities up to this time.
    */
   const std::vector<double>& GetDensityHistory() const;

   /**
    * Get the network up to this time.
    */
   const Network& GetNetwork() const;

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

   std::mt19937_64 _rng;

   double _communication_range;

   NetworkSnapshot CurrentNetwork() const;

public:
   Model(double arena_size, int num_agents, double communication_range,
         int seed, double initial_density);
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
    * Get statistics about the model.
    */
   const ModelStats& GetStats() const;

   /**
    * Evaluate the model for one time-step.
    */
   void Step();

   /**
    * Set the communication range of the agents.
    */
   void SetCommunicationRange(double range);
};

#endif // _MOTION_CA_MODEL_HPP