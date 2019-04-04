#ifndef _LCA_HPP
#define _LCA_HPP

#include <memory>
#include <functional>

#include "Model.hpp"

/**
 * An LCA experiment.
 */
class LCA
{
private:
   int                    max_time_;
   std::shared_ptr<Rule>  update_rule_;
   std::unique_ptr<Model> model_;
public:
   LCA(const Model& m, std::shared_ptr<Rule> update_rule, int max_time);
   ~LCA();

   /**
    * Run the LCA Simulation for 'max_time_' time steps
    */
   void Run();

   /**
    * Run the LCA Simulation for 'max_time_' or until the early_stop
    * predicate returns true.
    * @param early_stop early termination predicate.
    */
   void Run(std::function<bool(const ModelStats&)> early_stop);

   /**
    * Run the LCA simulation for the given number of time steps.
    * @param k the number of steps to run.
    */
   void Run(int k);

   const ModelStats& GetStats() const;

   const std::vector<Agent>& GetAgents() const;
   const std::vector<int>&   GetStates() const;
   std::shared_ptr<NetworkSnapshot> CurrentNetwork() const;
   double CurrentDensity() const;
   void MinimizeMemory();
};

#endif // _LCA_HPP
