#include "Rule.hpp"

int identity_rule(int self, const std::set<int>& neighbors)
{
   return self;
}

int always_one(int self, const std::set<int>& neighbors)
{
   return 1;
}

int always_zero(int self, const std::set<int>& neighbors)
{
   return 0;
}
