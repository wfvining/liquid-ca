#ifndef _MOTION_CA_RULE_HPP
#define _MOTION_CA_RULE_HPP

#include <vector>

class Rule {
public:
   /**
    * Apply the rule to the current state and neighbor's state
    * yeilding the new state.
    */
   virtual std::pair<int, double> Apply(int self, const std::vector<int>& neighbors) const = 0;
};


class Identity : public Rule {
public:
   Identity();
   ~Identity();
   std::pair<int, double> Apply(int self, const std::vector<int>& neighbors) const override;
};

class Constant : public Rule {
public:
   Constant(int c);
   ~Constant();
   std::pair<int, double> Apply(int self, const std::vector<int>& neighbors) const override;
private:
   int state;
};

class MajorityRule : public Rule {
public:
   MajorityRule();
   MajorityRule(bool f);
   ~MajorityRule();
   std::pair<int, double> Apply(int self, const std::vector<int>& neighbors) const override;
private:
   bool flip = true;
};

#if 0

/**
 * A totalisitc rule with a single threshold.
 *
 * Creates two transitions:
 * 1. total <  threshold
 * 2. total >= threshold
 */
class SingleThreshold : public Rule {
public:
   SingleThreshold(double threshold, int above);
   ~SingleThreshold() {}
   int Apply(int self, const std::vector<int>& neighbors) const override;
private:
   double threshold;
   int    above;
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
public:
   DoubleThreshold(double low, double high, int above, int between, int below);
   ~DoubleThreshold() {}
   int Apply(int self, const std::vector<int>& neighbors) const override;
private:
   double high;
   double low;
   int    above;
   int    between;
   int    below;
};

class Quorum : public Rule {
public:
   Quorum(double t);
   ~Quorum() {}
   int Apply(int self, const std::vector<int>& neighbors) const override;
private:
   double threshold;
};
#endif // 0

#endif // _MOTION_CA_RULE_HPP
