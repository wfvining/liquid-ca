#ifndef _LCA_FACTORY_HPP
#define _LCA_FACTORY_HPP

#include <memory>
#include <mutex>

#include "Rule.hpp"
#include "MovementRule.hpp"
#include "Model.hpp"
#include "LCA.hpp"

/**
 * A factory for building LCA experiment instances
 */
class LCAFactory
{
private:
   std::mutex new_lca_mutex_; // mutex used when constructing new LCA instances

   int                                num_agents_;
   double                             communication_range_;
   int                                arena_size_;
   int                                seed_;
   double                             speed_;
   int                                max_time_; /* max number of time steps to run */
   std::shared_ptr<MovementRule>      movement_rule_;
   std::shared_ptr<Rule>              rule_; /* CA rule */
   std::default_random_engine         random_engine_;
   std::uniform_int_distribution<int> seed_distribution_;

   enum InitializationMethod {
      Uniform,    // initialize states at random
      ByPosition, // initialize stated by x-coordinate
      Center,     // initialize a cluster of agents in the center to
                  // the non-quiescent state.
   } init_;

   void ParseRule(std::string rule_spec);

public:

   LCAFactory();
   ~LCAFactory() {}

   /**
    * Initialize the factory based on command line arguments.
    * @return the first index of any remaining args in argv
    */
   int Init(int argc, char** argv);

   /**
    * Make a new LCA instance from the current factory settings. This
    * operation is thread safe.
    * @param initial_density initial fraction of 'ones'
    * @return A new LCA instance
    */
   std::unique_ptr<LCA> Create(double initial_density);

   /**
    * Set the maximum time the simulation will run.
    * @param t maximum number of time steps.
    */
   void SetMaxTime(int t);

   /**
    * Set the speed of the agents.
    * @param s agent speed.
    */
   void SetSpeed(double s);

   /**
    * Set the LCA update rule.
    */
   void SetRule(std::shared_ptr<Rule> r);

   /**
    * Set whether or not to record agent state history.
    *
    * @param record if true then save the agent state vector at each
    * time step.
    */
   void RecordStateHistory(bool record);

   /**
    * Get the arena size used by the factory.
    */
   double ArenaSize() const;
};

#endif // _LCA_FACTORY_HPP
