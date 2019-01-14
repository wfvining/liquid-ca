#include "Rule.hpp"

#include <numeric> // std::accumulate

MajorityRule::MajorityRule() {}
MajorityRule::~MajorityRule() {}

int MajorityRule::Apply(int self, const std::vector<int>& neighbors) const
{
   int n = std::accumulate(neighbors.begin(), neighbors.end(), 0) + self;
   if((double)n > ((double)neighbors.size()+1) / 2.0)
   {
      return 1;
   }
   else if((double)n == (double)(neighbors.size()+1) / 2.0)
   {
      return 1 - self;
   }
   else
   {
      return 0;
   }
}

Identity::Identity() {}
Identity::~Identity() {}

int Identity::Apply(int self, const std::vector<int>& neighbors) const
{
   return self;
}

Constant::Constant(int c) : state(c) {}
Constant::~Constant() {}

int Constant::Apply(int self, const std::vector<int>& neighbors) const
{
   return state;
}
