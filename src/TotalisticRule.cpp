#include "TotalisticRule.hpp"
#include "Transition.hpp"

#include <numeric>
#include <iostream>
#include <utility>

std::istream& operator>>(std::istream& stream, TotalisticRule& rule)
{
   TotalisticRule temp;

   std::string line;
   int line_number = 1;
   while(std::getline(stream, line))
   {
      if(line.size() == 0 || line.front() == '#') continue; // skip comments

      try {
         temp.transition_table_.push_back(parser::parse_transition(line));
      } catch(parser::ParseException e)
      {
         std::cerr << "Parse failed on line " << line_number << ": " << e.message << std::endl;
         throw e;
      }
      line_number++;
   }

   rule = std::move(temp);
   return stream;
}

double density(const std::vector<int>& neighborhood)
{
   return ((double)std::accumulate(neighborhood.begin(), neighborhood.end(), 0)
           / (double)neighborhood.size());
}

bool matches(Transition& t, int self, const std::vector<int>& neighbors)
{
   if(t.any_state || self == t.pre_state)
   {
      double neighborhood_density;

      std::vector<int> neighborhood(neighbors.size());
      std::copy(neighbors.begin(), neighbors.end(), neighborhood.begin());
      if(t.include_self)
      {
         neighborhood.push_back(self);
      }

      neighborhood_density = density(neighborhood);

      if(t.range.Contains(neighborhood_density))
      {
         return true;
      }
   }
   return false;
}

std::pair<int, double> apply(Transition t, int self)
{
   if(t.result_self) return std::make_pair(self, t.heading_change);
   else return std::make_pair(t.result_state, t.heading_change);
}

std::pair<int, double> TotalisticRule::Apply(int self, const std::vector<int>& neighbors) const
{
   // Look for rules that match the current state
   // Apply the first rule that matches
   for(Transition t : transition_table_)
   {
      if(matches(t, self, neighbors))
      {
         return apply(t, self);
      }
   }

   // If no rule applies then state remains unchanged.
   return std::make_pair(self, 0); // XXX: is this the right thing to do.
}
