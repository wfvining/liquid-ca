#ifndef _LCA_RANGE_HPP
#define _LCA_RANGE_HPP

#include <iostream>

class Range
{
private:
   bool include_min_;
   bool include_max_;

   double min_;
   double max_;

public:
   Range(double min, double max);
   ~Range() {}

   Range ExcludeMax() const;
   Range ExcludeMin() const;

   double Min() const;
   double Max() const;

   bool Contains(double k) const;

   friend std::ostream& operator<< (std::ostream& out, const Range& r);
};

#endif // _LCA_RANGE_HPP
