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

int gkl2d_lax(int self, const Point& p, const std::vector<int>& neighbors, const std::vector<Point>& positions)
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

int gkl2d_strict(int self, const Point& p, const std::vector<int>& neighbors, const std::vector<Point>& positions)
{
   auto position_iter = positions.begin();
   auto neighbor_iter   = neighbors.begin();
   int sum = self;
   int count = 1;

   for( ; position_iter < positions.end(); ++position_iter, ++neighbor_iter)
   {
      if(self == 0 && (position_iter->DueNorthOf(p) || position_iter->DueEastOf(p)))
      {
         sum += *neighbor_iter;
         count++;
      }
      else if(self == 1 && (position_iter->DueSouthOf(p) || position_iter->DueWestOf(p)))
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

int mode(std::vector<int>& v)
{
   int zeros = 0;
   int ones  = 0;
   for(int s : v)
   {
      s == 0 ? zeros++ : ones++;
   }

   if(zeros == ones)
   {
      return -1;
   }
   else if(zeros > ones)
   {
      return 0;
   }
   else
   {
      return 1;
   }
}

int majority(int self, int a, int b)
{
   if(a == -1 && b == -1)
   {
      return self;
   }
   else if(a == -1 && b == self)
   {
      return self;
   }
   else if(a == -1 && b != self)
   {
      return 1-self;
   }
   else if(b == -1 && a == self)
   {
      return self;
   }
   else if(b == -1 && a != self)
   {
      return 1-self;
   }
   else // all three sectors have information
   {
      if((a == 1 && b == 1) || (a == 1 && self == 1) || (b == 1 && self == 1))
      {
         return 1;
      }
      else // if((a == 0 && b == 0) || (a == 0 && self == 0) || (b == 0 && self == 0))
      {
         return 0;
      }
   }
}

int gkl2d_mode(int self, const Point& p, const std::vector<int>& neighbors, const std::vector<Point>& positions)
{
   auto position_iter = positions.begin();
   auto neighbor_iter   = neighbors.begin();
   int sum = self;
   int count = 1;
   std::vector<int> neighbors_north;
   std::vector<int> neighbors_south;
   std::vector<int> neighbors_east;
   std::vector<int> neighbors_west;

   // partition the neighbors
   for( ; position_iter < positions.end(); ++position_iter, ++neighbor_iter)
   {
      if(position_iter->DueNorthOf(p))
      {
         neighbors_north.push_back(*neighbor_iter);
      }
      else if(position_iter->DueEastOf(p))
      {
         neighbors_east.push_back(*neighbor_iter);
      }
      else if(position_iter->DueSouthOf(p))
      {
         neighbors_south.push_back(*neighbor_iter);
      }
      else if(position_iter->DueWestOf(p))
      {
         neighbors_west.push_back(*neighbor_iter);
      }
   }

   if(self == 0)
   {
      return majority(self, mode(neighbors_north), mode(neighbors_east));
   }
   else
   {
      return majority(self, mode(neighbors_south), mode(neighbors_west));
   }
}

int gkl2d_closest(int self, const Point& p, const std::vector<int>& neighbors, const std::vector<Point>& positions)
{
   auto position_iter = positions.begin();
   auto neighbor_iter   = neighbors.begin();
   int sum = self;
   int count = 1;
   std::vector<int> neighbors_north;
   std::vector<int> neighbors_south;
   std::vector<int> neighbors_east;
   std::vector<int> neighbors_west;

   double closest_north = 10000;
   double closest_south = 10000;
   double closest_east  = 10000;
   double closest_west  = 10000;

   int closest_north_state = -1;
   int closest_south_state = -1;
   int closest_east_state  = -1;
   int closest_west_state  = -1;
   
   // partition the neighbors
   for( ; position_iter < positions.end(); ++position_iter, ++neighbor_iter)
   {
      if(position_iter->DueNorthOf(p))
      {
         neighbors_north.push_back(*neighbor_iter);
         if(position_iter->Distance(p) < closest_north)
         {
            closest_north = position_iter->Distance(p);
            closest_north_state = *neighbor_iter;
         }
      }
      else if(position_iter->DueEastOf(p))
      {
         neighbors_east.push_back(*neighbor_iter);
         if(position_iter->Distance(p) < closest_east)
         {
            closest_east = position_iter->Distance(p);
            closest_east_state = *neighbor_iter;
         }
      }
      else if(position_iter->DueSouthOf(p))
      {
         neighbors_south.push_back(*neighbor_iter);
         if(position_iter->Distance(p) < closest_south)
         {
            closest_south = position_iter->Distance(p);
            closest_south_state = *neighbor_iter;
         }
      }
      else if(position_iter->DueWestOf(p))
      {
         neighbors_west.push_back(*neighbor_iter);
         if(position_iter->Distance(p) < closest_west)
         {
            closest_west = position_iter->Distance(p);
            closest_west_state = *neighbor_iter;
         }
      }
   }

   if(self == 0)
   {
      return majority(self, closest_north_state, closest_east_state);
   }
   else
   {
      return majority(self, closest_south_state, closest_west_state);
   }
}