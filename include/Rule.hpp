#ifndef _MOTION_CA_RULE_HPP
#define _MOTION_CA_RULE_HPP

#include <vector>

typedef int Rule(int, const std::vector<int>&);

int identity_rule(int self, const std::vector<int>& neighbors);

#endif // _MOTION_CA_RULE_HPP
