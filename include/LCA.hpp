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

   const ModelStats& GetStats() const;
};

#endif // _LCA_HPP
