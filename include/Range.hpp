#ifndef _LCA_RANGE_HPP
#define _LCA_RANGE_HPP

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

   bool Contains(double k) const;
};

#endif // _LCA_RANGE_HPP
