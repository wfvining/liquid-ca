#ifndef _TRANSITION_HPP
#define _TRANSITION_HPP

#include "Range.hpp"
#include <string>

struct Transition
{
   bool   any_state;
   bool   include_self;
   int    pre_state;
   int    result_state;
   double heading_change;
   bool   result_self;
   Range  range;

   Transition() :
      any_state(false),
      include_self(false),
      result_self(false),
      range(0.0, 1.0),
      heading_change(0.0)
      {}
};

namespace parser
{
   class ParseException : public std::exception
   {
   public:
      std::string message;
      ParseException(std::string m) : message(m) {}
   };

   Transition parse_transition(std::string& str);
}

#endif // _TRANSITION_HPP
