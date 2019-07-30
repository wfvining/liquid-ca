#ifndef _LCA_GA_HPP
#define _LCA_GA_HPP

#include <vector>

#include "Transition.hpp"

struct Genotype {
   std::vector<Transition> one_transitions;
   std::vector<Transition> zero_transitions;
};

#endif // _LCA_GA_HPP
