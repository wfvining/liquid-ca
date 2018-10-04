#ifndef _MOTION_CA_RULE_HPP
#define _MOTION_CA_RULE_HPP

#include <vector>
#include "Point.hpp"

typedef int Rule(int, const Point&, const std::vector<int>&, const std::vector<Point>&);

// int identity_rule(int self, const std::vector<int>& neighbors);
// int always_one(int self, const std::vector<int>& neighbors);
// int always_zero(int self, const std::vector<int>& neighbors);
// int majority_rule(int self, const std::vector<int>& neighbors);
// int contrarian_rule(int self, const std::vector<int>& neighbors);

Rule identity_rule;
Rule always_one;
Rule always_zero;
Rule majority_rule;
Rule contrarian_rule;
Rule gkl2d_strict;
Rule gkl2d_lax;

#endif // _MOTION_CA_RULE_HPP
