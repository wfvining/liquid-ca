#include "Rule.hpp"

#include <algorithm> // std::accumulate

int identity_rule(int self, const std::vector<int>& neighbors)
{
   return self;
}

int always_one(int self, const std::vector<int>& neighbors)
{
   return 1;
}

int always_zero(int self, const std::vector<int>& neighbors)
{
   return 0;
}

/**
 * majority rule including self
 */
int majority_rule(int self, const std::vector<int>& neighbors)
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
