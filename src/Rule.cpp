#include "Rule.hpp"

#include <numeric> // std::accumulate

int identity_rule(int self, const Point&p, const std::vector<int>& neighbors, const std::vector<Point>& positions)
{
   return self;
}

int always_one(int self, const Point& p, const std::vector<int>& neighbors, const std::vector<Point>& positions)
{
   return 1;
}

int always_zero(int self, const Point& p, const std::vector<int>& neighbors, const std::vector<Point>& positions)
{
   return 0;
}

/**
 * majority rule including self
 */
int majority_rule(int self, const Point& p, const std::vector<int>& neighbors, const std::vector<Point>& positions)
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

int contrarian_rule(int self, const Point& p, const std::vector<int>& neighbors, const std::vector<Point>& positions)
{
   int n = std::accumulate(neighbors.begin(), neighbors.end(), self);
   if((double) n > ((double)neighbors.size()+1) / 2.0)
   {
      return 0;
   }
   else if((double)n == (double)(neighbors.size()+1) / 2.0)
   {
      return 1-self;
   }
   else
   {
      return 1;
   }
}

int gkl2d(int self, const Point& p, const std::vector<int>& neighbors, const std::vector<Point>& positions)
{
   auto position_iter = positions.begin();
   auto neighbor_iter   = neighbors.begin();
   int sum = self;
   int count = 1;

   for( ; position_iter < positions.end(); ++position_iter, ++neighbor_iter)
   {
      if(self == 0 && (position_iter->NorthOf(p) || position_iter->EastOf(p)))
      {
         sum += *neighbor_iter;
         count++;
      }
      else if(self == 1 && (position_iter->SouthOf(p) || position_iter->WestOf(p)))
      {
         sum += *neighbor_iter;
         count++;
      }
   }
   
   double majority = (double) sum / (double) count;
   if(majority > 0.5)
   {
      return 1;
   }
   else if(majority == 0.5)
   {
      return 1 - self; // consistent with the implementation of majority rule above
   }
   else
   {
      return 0;
   }
}
