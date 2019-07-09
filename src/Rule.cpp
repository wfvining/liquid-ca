#include "Rule.hpp"

#include <numeric> // std::accumulate

Identity::Identity() {}
Identity::~Identity() {}

std::pair<int, double> Identity::Apply(int self, const std::vector<int>& neighbors) const
{
   return std::make_pair(self, 0);
}

MajorityRule::MajorityRule() {}
MajorityRule::MajorityRule(bool f) : flip(f) {}
MajorityRule::~MajorityRule() {}

std::pair<int, double> MajorityRule::Apply(int self, const std::vector<int>& neighbors) const
{
   int n = std::accumulate(neighbors.begin(), neighbors.end(), 0) + self;
   if((double)n > ((double)neighbors.size()+1) / 2.0)
   {
      return std::make_pair(1, 0);
   }
   else if((double)n == (double)(neighbors.size()+1) / 2.0)
   {
      return std::make_pair(flip ? 1 - self : self, 0);
   }
   else
   {
      return std::make_pair(0, 0);
   }
}

Constant::Constant(int c) : state(c) {}
Constant::~Constant() {}

std::pair<int, double> Constant::Apply(int self, const std::vector<int>& neighbors) const
{
   return std::make_pair(state, 0);
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

#if 0

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

Quorum::Quorum(double t) :
   threshold(t)
{}

int Quorum::Apply(int self, const std::vector<int>& neighbors) const
{
   double local_density = density(self, neighbors);
   if(local_density < (1 - threshold))
   {
      return 0;
   }
   else if(local_density > threshold)
   {
      return 1;
   }
   else
   {
      return self;
   }
}

#endif // 0
