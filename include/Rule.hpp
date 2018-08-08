#ifndef _MOTION_CA_RULE_HPP
#define _MOTION_CA_RULE_HPP

#include <set>

typedef int Rule(int, const std::set<int>&);

int identity_rule(int self, const std::set<int>& neighbors);
int always_one(int self, const std::set<int>& neighbors);
int always_zero(int self, const std::set<int>& neighbors);

#endif // _MOTION_CA_RULE_HPP
