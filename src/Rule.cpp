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

/**
 * Utility function to compute the density in the neighborhood
 * including self.
 */
double density(int self, const std::vector<int>& neighbors)
{
   return ((double)std::accumulate(neighbors.begin(),
                                   neighbors.end(),
                                   0) + self)
      / (1.0 + (double)neighbors.size());
}

SingleThreshold::SingleThreshold(double threshold, int above) :
   threshold(threshold),
   above(above)
{}

int SingleThreshold::Apply(int self, const std::vector<int>& neighbors) const
{
   double local_density = density(self, neighbors);
   return local_density < threshold ? (1 - above) : above;
}

DoubleThreshold::DoubleThreshold(double low, double high, int below, int between, int above) :
   low(low),
   high(high),
   above(above),
   between(between),
   below(below)
{}

int DoubleThreshold::Apply(int self, const std::vector<int>& neighbors) const
{
   double local_density = density(self, neighbors);
   if(local_density < low)
   {
      return below;
   }
   else if(local_density > high)
   {
      return above;
   }
   else
   {
      return between;
   }
}
