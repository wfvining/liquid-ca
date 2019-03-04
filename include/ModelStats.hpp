#ifndef _MODEL_STATS_HPP
#define _MODEL_STATS_HPP

#include <vector>

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

   double AverageAggregateDegree() const;
   double AggregateDegreeStdDev() const;

   /**
    * Get the median aggregate degree over all nodes.
    */
   double MedianAggregateDegree() const;
};

#endif // _MODEL_STATS_HPP
