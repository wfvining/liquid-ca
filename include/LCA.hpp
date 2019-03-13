#ifndef _LCA_HPP
#define _LCA_HPP

#include <memory>

#include "Model.hpp"

/**
 * An LCA experiment.
 */
class LCA
{
private:
   int   max_time_;
   std::uniqe_ptr<Model> model_;
public:
   LCA(std::unique_ptr<Model> m, int max_time);
   ~LCA();

   /**
    * Run the LCA Simulation
    */
   void Run();
};

#endif // _LCA_HPP
