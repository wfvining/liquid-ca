#ifndef _LCA_GA_HPP
#define _LCA_GA_HPP

#include <vector>

#include "Transition.hpp"

struct Genotype {
   std::vector<Transition> from_one;
   std::vector<Transition> from_zero;
};

#endif // _LCA_GA_HPP
