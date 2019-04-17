#include "Range.hpp"

Range::Range(double min, double max) :
   min_(min),
   max_(max),
   include_min_(true),
   include_max_(true)
{}

Range Range::ExcludeMin() const
{
   Range r(*this);
   r.include_min_ = false;
   return r;
}

Range Range::ExcludeMax() const
{
   Range r(*this);
   r.include_max_ = false;
   return r;
}

bool Range::Contains(double k) const
{
   if(include_max_ && include_min_)
   {
      return k <= max_ && k >= min_;
   }
   else if(include_max_ && !include_min_)
   {
      return k <= max_ && k > min_;
   }
   else if(!include_max_ && include_min_)
   {
      return k < max_ && k >= min_;
   }
   else // if(!include_max_ && !include_min_)
   {
      return k < max_ && k > min_;
   }
}

std::ostream& operator<<(std::ostream& out, const Range& r)
{
   return out << (r.include_min_ ? "[" : "(") << r.min_ << "," << r.max_ << (r.include_max_ ? "]" : ")");
}
