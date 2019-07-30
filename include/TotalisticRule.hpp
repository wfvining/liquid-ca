#ifndef _TOTALISTIC_RULE_HPP
#define _TOTALISTIC_RULE_HPP

#include <vector>
#include <sstream>
#include <utility>

#include "Rule.hpp"
#include "Transition.hpp"

class TotalisticRule : public Rule
{
private:

   std::vector<Transition> transition_table_;

public:
   TotalisticRule() {}
   TotalisticRule(std::vector<Transition> transitions);
   ~TotalisticRule() {}

   std::pair<int, double> Apply(int self, const std::vector<int>& neighbors) const override;

   friend std::istream& operator>>(std::istream& str, TotalisticRule& rule);
   friend std::ostream& operator<<(std::ostream& str, const TotalisticRule& rule);
};

#endif // _TOTALISTIC_RULE_HPP
