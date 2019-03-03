#include <gtest/gtest.h>

#include "Rule.hpp"

class RuleTest : public ::testing::Test
{
public:
   MajorityRule     majority_rule;
   std::vector<int> half_ones = {0,1,0,1};
   std::vector<int> all_ones  = {1,1,1,1};
   std::vector<int> all_zeros = {0,0,0,0};
   std::vector<int> slim_majority_ones =  {0,0,1,1,1,1};
   std::vector<int> slim_majority_zeros = {0,0,0,0,1,1};
   RuleTest() {}
   ~RuleTest() {}
};

TEST_F(RuleTest, slimMajorityWithSelf)
{
   EXPECT_EQ(1, majority_rule.Apply(1, half_ones));
   EXPECT_EQ(0, majority_rule.Apply(0, half_ones));
}

TEST_F(RuleTest, slimMajorityAgainstSelf)
{
   EXPECT_EQ(1, majority_rule.Apply(0, slim_majority_ones));
   EXPECT_EQ(0, majority_rule.Apply(1, slim_majority_zeros));
}

TEST_F(RuleTest, absoluteMajority)
{
   EXPECT_EQ(1, majority_rule.Apply(1, all_ones ));
   EXPECT_EQ(1, majority_rule.Apply(0, all_ones ));
   EXPECT_EQ(0, majority_rule.Apply(0, all_zeros));
   EXPECT_EQ(0, majority_rule.Apply(1, all_zeros));
}

TEST_F(RuleTest, singleThresholdTie)
{
   SingleThreshold st_rule1(0.5, 1);
   SingleThreshold st_rule0(0.5, 0);
   EXPECT_EQ(1, st_rule1.Apply(1, {0,0,1}));
   EXPECT_EQ(0, st_rule0.Apply(1, {0,0,1}));
}

TEST_F(RuleTest, singleThresholdNotTied)
{
   SingleThreshold st_rule1(0.5, 1);
   SingleThreshold st_rule0(0.5, 0);
   EXPECT_EQ(0, st_rule1.Apply(0, half_ones));
   EXPECT_EQ(1, st_rule1.Apply(1, half_ones));

   EXPECT_EQ(1, st_rule0.Apply(0, half_ones));
   EXPECT_EQ(0, st_rule0.Apply(1, half_ones));
}

TEST_F(RuleTest, singleThreshold25Percent)
{
   SingleThreshold st_rule(0.25, 1);
   EXPECT_EQ(1, st_rule.Apply(1, {0,0,0}))   << "0.25 should result in 1";
   EXPECT_EQ(1, st_rule.Apply(0, {1,1,0}))   << "0.50 should result in 1";
   EXPECT_EQ(1, st_rule.Apply(1, {0,0}))     << "0.33 should result in 1";
   EXPECT_EQ(0, st_rule.Apply(0, {0,0,0}))   << "0.00 should result in 0";
   EXPECT_EQ(0, st_rule.Apply(0, {1,0,0,0})) << "0.20 should result in 0 (self 0)";
   EXPECT_EQ(0, st_rule.Apply(1, {0,0,0,0})) << "0.20 should result in 0 (self 1)";
   EXPECT_EQ(1, st_rule.Apply(1, {1,1,1}))   << "1.00 should result in 1";
}

TEST_F(RuleTest, singleThreshold75Percent)
{
   SingleThreshold st_rule(0.75, 1);
   EXPECT_EQ(1, st_rule.Apply(1, {1,1,0}))   << "0.75 should result in 1";
   EXPECT_EQ(0, st_rule.Apply(0, {1,1,0}))   << "0.50 should result in 0";
   EXPECT_EQ(0, st_rule.Apply(0, {1,1}))     << "0.66 should result in 0";
   EXPECT_EQ(0, st_rule.Apply(0, {0,0,0}))   << "0.00 should result in 0";
   EXPECT_EQ(0, st_rule.Apply(0, {1,0,0,0})) << "0.20 should result in 0 (self 0)";
   EXPECT_EQ(0, st_rule.Apply(1, {0,0,0,0})) << "0.20 should result in 0 (self 1)";
   EXPECT_EQ(1, st_rule.Apply(0, {1,1,1,1})) << "0.80 should result in 1 (self 0)";
   EXPECT_EQ(1, st_rule.Apply(1, {0,1,1,1})) << "0.80 should result in 1 (self 1)";
   EXPECT_EQ(1, st_rule.Apply(1, {1,1,1}))   << "1.00 should result in 1";
}

TEST_F(RuleTest, singleThresholdEdgeCases)
{
   SingleThreshold st_rule1(1.0, 1);
   EXPECT_EQ(0, st_rule1.Apply(1, {1,1,0}))  ;
   EXPECT_EQ(0, st_rule1.Apply(0, {1,1,0}))  ;
   EXPECT_EQ(0, st_rule1.Apply(0, {1,1}))    ;
   EXPECT_EQ(0, st_rule1.Apply(0, {0,0,0}))  ;
   EXPECT_EQ(0, st_rule1.Apply(0, {1,0,0,0}));
   EXPECT_EQ(0, st_rule1.Apply(1, {0,0,0,0}));
   EXPECT_EQ(0, st_rule1.Apply(0, {1,1,1,1}));
   EXPECT_EQ(0, st_rule1.Apply(1, {0,1,1,1}));
   EXPECT_EQ(1, st_rule1.Apply(1, {1,1,1}))  ;

   SingleThreshold st_rule0(0.0, 1);
   EXPECT_EQ(1, st_rule0.Apply(1, {1,1,0}))  ;
   EXPECT_EQ(1, st_rule0.Apply(0, {1,1,0}))  ;
   EXPECT_EQ(1, st_rule0.Apply(0, {1,1}))    ;
   EXPECT_EQ(1, st_rule0.Apply(0, {0,0,0}))  ;
   EXPECT_EQ(1, st_rule0.Apply(0, {1,0,0,0}));
   EXPECT_EQ(1, st_rule0.Apply(1, {0,0,0,0}));
   EXPECT_EQ(1, st_rule0.Apply(0, {1,1,1,1}));
   EXPECT_EQ(1, st_rule0.Apply(1, {0,1,1,1}));
   EXPECT_EQ(1, st_rule0.Apply(1, {1,1,1}))  ;
}

TEST_F(RuleTest, singleThresholdAlone)
{
   SingleThreshold st_rule(0.25, 1);
   EXPECT_EQ(1, st_rule.Apply(1, {})) << "self 1 no neighbors should be 1";
   EXPECT_EQ(0, st_rule.Apply(0, {})) << "self 0 no neighbors should be 0";
}

TEST_F(RuleTest, doubleThresholdOnLow)
{
   DoubleThreshold dt_rule(0.25, 0.5, 0, 1, 2);
   EXPECT_EQ(1, dt_rule.Apply(1, {0,0,0})) << "self 1";
   EXPECT_EQ(1, dt_rule.Apply(0, {1,0,0})) << "self 0";
}

TEST_F(RuleTest, doubleThresholdOnHigh)
{
   DoubleThreshold dt_rule(0.25, 0.5, 0, 1, 2);
   EXPECT_EQ(1, dt_rule.Apply(1, {1,0,0})) << "self 1";
   EXPECT_EQ(1, dt_rule.Apply(0, {1,1,0})) << "self 0";
}

TEST_F(RuleTest, doubleThresholdInRange)
{
   DoubleThreshold dt_rule(0.25, 0.75, 0, 1, 2);
   EXPECT_EQ(0, dt_rule.Apply(0, {}))           << "0.00 should result in 0 (self 1)";
   EXPECT_EQ(0, dt_rule.Apply(1, {0,0,0,0}))    << "0.20 should result in 0 (self 1)";
   EXPECT_EQ(0, dt_rule.Apply(0, {0,0,1,0}))    << "0.20 should result in 0 (self 0)";
   EXPECT_EQ(1, dt_rule.Apply(1, {0,0}))        << "0.33 should result in 1";
   EXPECT_EQ(2, dt_rule.Apply(1, {}))           << "1.00 should result in 2 (no neighbors)";
   EXPECT_EQ(2, dt_rule.Apply(1, {1, 1, 1, 0})) << "0.80 should result in 2";
}
