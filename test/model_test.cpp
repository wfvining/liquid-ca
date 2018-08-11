#include <gmock/gmock.h>

#include "Model.hpp"

TEST(ModelTest, expectedDensityIsCorrect)
{
   double sum = 0;
   for(int i = 0; i < 100; i++)
   {
      Model m(100, 256, 5.0, 1337+i, 0.5);
      sum += m.CurrentDensity();
   }
   EXPECT_THAT(sum/100.0, ::testing::DoubleNear(0.5, 0.01));

   sum = 0;
   for(int i = 0; i < 100; i++)
   {
      Model m(100, 256, 5.0, 1337+i, 0.25);
      sum += m.CurrentDensity();
   }
   EXPECT_THAT(sum/100.0, ::testing::DoubleNear(0.25, 0.01));

   sum = 0;
   for(int i = 0; i < 100; i++)
   {
      Model m(100, 256, 5.0, 1337+i, 0.75);
      sum += m.CurrentDensity();
   }
   EXPECT_THAT(sum/100.0, ::testing::DoubleNear(0.75, 0.01));
}

TEST(ModelTest, densityExtremes)
{
   Model m0(100, 256, 5.0, 12345, 0.0);
   Model m1(100, 256, 5.0, 12345, 1.0);

   EXPECT_EQ(m0.CurrentDensity(), 0.0);
   EXPECT_EQ(m1.CurrentDensity(), 1.0);
}

TEST(ModelTest, differentSeedDifferentDensity)
{
   Model m1(100, 128, 5.0, 1337, 0.5);
   Model m2(100, 128, 5.0, 1338, 0.5);

   EXPECT_NE(m1.CurrentDensity(), m2.CurrentDensity());
}

TEST(ModelTest, extremelySmallArena)
{
   Model m1(0.25, 32, 5.0, 123, 0.5);
}

TEST(ModelTest, networkChanges)
{
   Model m(50, 128, 5.0, 1337, 0.5);
   for(int i = 0; i < 25; i++)
   {
      m.Step(&identity_rule);
   }
   const ModelStats& stats = m.GetStats();
   const Network& network = stats.GetNetwork();
   EXPECT_FALSE(network.GetSnapshot(0) == network.GetSnapshot(24));
}

TEST(ModelTest, identityRuleUpdate)
{
   Model m(10, 25, 1.0, 1234, 0.5);
   double initial_density = m.CurrentDensity();
   for(int i = 0; i < 100; i++)
   {
      m.Step(&identity_rule);
      EXPECT_EQ(initial_density, m.CurrentDensity());
   }
}

TEST(ModelTest, majorityRuleUnderFullyConnectedGraph)
{
   Model m(10, 50, 20, 1234, 0.7);
   double initial_density = m.CurrentDensity();
   m.Step(majority_rule);
   EXPECT_EQ(1.0, m.CurrentDensity());
}

TEST(ModelTest, alwaysOneRule)
{
   Model m(10, 25, 1.0, 1234, 0.5);
   m.Step(&always_one);
   EXPECT_EQ(m.CurrentDensity(), 1.0);
}

TEST(ModelTest, alwaysZeroRule)
{
   Model m(10, 25, 1.0, 1234, 0.5);
   m.Step(&always_zero);
   EXPECT_EQ(m.CurrentDensity(), 0.0);
}
