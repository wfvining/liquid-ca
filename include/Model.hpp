#ifndef _MOTION_CA_MODEL_HPP
#define _MOTION_CA_MODEL_HPP

#include <vector>
#include <random>

#include "Agent.hpp"

/**
 * Statistics about a model including current timestep, current
 * density, density at each previous timestep, and whether or not the
 * classification is correct.
 */
class ModelStats
{
private:
   std::vector<double> _network_density;
   std::vector<double> _ca_density;

public:
   ModelStats();
   ~ModelStats();

   /**
    * Record the ca density and the density of the interaction network
    * at the next timestep.
    */
   void PushState(double density, double network_density);

   const std::vector<double>& GetDensityHistory() const;

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
   int ElapsedTime() const;
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

public:
   Model(int seed, double arena_size, int num_agents, double initial_density);
   ~Model();

   /**
    * Get the current density of the model (ie. proportion of black
    * states to white states).
    */
   double CurrentDensity() const;

   /**
    * Get statistics about the model.
    */
   const ModelStats& GetStats() const;

   void Run();
   void Run(int max_steps);
};

#endif // _MOTION_CA_MODEL_HPP