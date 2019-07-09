#include "Transition.hpp"

#include <cmath> // M_PI
#include <sstream>
#include <iostream>
#include <utility>

namespace parser {
   std::string trim_leading_space(const std::string& str)
   {
      return str.substr(str.find_first_not_of(" "), str.length());
   }

   std::string pre_condition(std::string& rule)
   {
      int transition_position = rule.find("->");
      if(transition_position == std::string::npos)
         throw ParseException("missing transition (->)");

      return trim_leading_space(rule.substr(0, transition_position));
   }

   void result(std::string& rule, Transition& t)
   {
      std::string result = trim_leading_space(rule.substr(rule.find("->") + 2, rule.length()));
      std::string result_state = result.substr(0,result_state.find_first_of(" ,\t")-1);
      std::cout << result_state << std::endl;

      if(result_state == "@")
      {
         t.result_self = true;
      }
      else
      {
         try
         {
            t.result_state = std::stoi(result);
         }
         catch(std::invalid_argument e)
         {
            throw ParseException("invalid transition result");
         }
      }

      std::string heading_change = trim_leading_space(result.substr(result.find(",") + 1, result.length()));
      std::cout << heading_change << std::endl;
      heading_change = heading_change.substr(0, heading_change.find_first_of(" #\t")-1);
      try
      {
         t.heading_change = std::stod(heading_change) * M_PI/180.0;
      }
      catch(std::invalid_argument e)
      {
         throw ParseException("invalid transition heading change");
      }

      std::cout << "Heading_change: " << t.heading_change << std::endl;
   }

   void pre_state(std::string& pre, Transition& t)
   {
      int pre_state_end = pre.find_first_of(" ");
      if(pre_state_end == std::string::npos)
         throw ParseException("bad precondition state");

      if(pre.at(pre_state_end-1) == '@')
      {
         t.any_state = true;
      }
      else
      {
         std::string p = pre.substr(0, pre_state_end);
         try
         {
            t.pre_state = std::stoi(p);
         }
         catch (std::invalid_argument e)
         {
            throw ParseException("invalid pre state");
         }
      }
   }

   void pre_inclusive(std::string& pre, Transition& t)
   {
      std::string pre_suffix = trim_leading_space(pre.substr(pre.find_first_of(" "), pre.length()));

      if(pre_suffix.front() == '+')
      {
         t.include_self = true;
      }
      else if(pre_suffix.front() == '-')
      {
         t.include_self = false;
      }
      else
      {
         std::ostringstream os;
         os << "invalid inclusive marker in: \"" << pre_suffix << "\"";
         throw ParseException(os.str());
      }
   }

   Range pre_range(std::string& pre)
   {
      int range_start = pre.find_first_of("[(");
      int range_end   = pre.find_first_of("])");

      if(range_start == std::string::npos || range_end == std::string::npos)
      {
         throw ParseException("bad range");
      }

      if(range_end < pre.length())
      {
         if(pre.substr(range_end+1).find_first_not_of(" ") != std::string::npos)
         {
            std::ostringstream os;
            os << "extra characters following range: " << pre.substr(range_end, pre.length());
            throw ParseException(os.str());
         }
      }

      std::string full_range = pre.substr(range_start, (range_end - range_start)+1);
      std::string min_str;
      std::string max_str;
      double min;
      double max;

      std::istringstream iss(pre.substr(range_start+1, (range_end - range_start)-1));
      if(std::getline(iss, min_str, ',') && std::getline(iss, max_str, ','))
      {
         try
         {
            min = std::stod(min_str);
            max = std::stod(max_str);
         }
         catch(std::invalid_argument e)
         {
            throw ParseException("bad number in range");
         }
      }
      else
      {
         std::ostringstream os;
         os << "bad range: \"" << full_range << "\"";
         throw ParseException(os.str());
      }

      Range r(min, max);

      if(full_range.front() == '(')
      {
         r = r.ExcludeMin();
      }

      if(full_range.back() == ')')
      {
         r = r.ExcludeMax();
      }

      return r;
   }

   std::ostream& operator<<(std::ostream& out, const Transition& t)
   {
      return out << (t.any_state ? "@" : std::to_string(t.pre_state)) << " "
                 << (t.include_self ? "+" : "-") << " "
                 << t.range << " -> "
                 << (t.result_self ? "@" : std::to_string(t.result_state))
                 << ", " << t.heading_change
                 << std::endl;
   }

   Transition parse_transition(std::string& str)
   {
      Transition t;
      std::string precondition = pre_condition(str);
      pre_state(precondition, t);
      pre_inclusive(precondition, t);
      t.range = pre_range(precondition);
      result(str, t);

      std::cout << "parsed rule: " << t << std::endl;

      return t;
   }
}
