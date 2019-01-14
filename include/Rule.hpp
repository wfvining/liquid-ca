#ifndef _MOTION_CA_RULE_HPP
#define _MOTION_CA_RULE_HPP

#include <vector>
#include "Point.hpp"

class Rule {
public:
   Rule() {}
   ~Rule() {}

   /**
    *
    */
   virtual int Apply(int self, const std::vector<int>& neighbors) const = 0;
};

class Identity : public Rule {
public:
   Identity();
   ~Identity();
   int Apply(int self, const std::vector<int>& neighbors) const override;
};

class Constant : public Rule {
public:
   Constant(int c);
   ~Constant();
   int Apply(int self, const std::vector<int>& neighbors) const override;
private:
   int state;
};

class MajorityRule : public Rule {
public:
   MajorityRule();
   ~MajorityRule();
   int Apply(int self, const std::vector<int>& neighbors) const override;
};

/**
 * A totalisitc rule with a single threshold.
 *
 * Creates two transitions:
 * 1. total <  threshold
 * 2. total >= threshold
 */
class SingleThreshold : public Rule {
public:
   SingleThreshold(double threshold);
    ~SingleThreshold();
   int Apply(int self, const std::vector<int>& neighbors) const override;
};

/**
 * A totalistic rule with two thresholds.
 *
 * Creates three possible transitions:
 * 1. total < low
 * 2. low <= total <= high
 * 3. total > high
 */
class DoubleThreshold : public Rule {
   DoubleThreshold(double low, double high);
   ~DoubleThreshold();
   int Apply(int self, const std::vector<int>& neighbors) const override;
};

#endif // _MOTION_CA_RULE_HPP
