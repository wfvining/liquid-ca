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

// TODO: Network changes.
TEST(ModelTest, networkChanges)
{
   Model m(50, 128, 5.0, 1337, 0.5);
   for(int i = 0; i < 25; i++)
   {
      m.Step();
   }
   const ModelStats& stats = m.GetStats();
   const Network& network = stats.GetNetwork();
   EXPECT_FALSE(network.GetSnapshot(0) == network.GetSnapshot(24));
}

class ModelStatsTest : public ::testing::Test
{
public:
   ModelStats stats;
   NetworkSnapshot t0;
   NetworkSnapshot t1;
   NetworkSnapshot t2;
   ModelStatsTest() :
      t0(10),
      t1(10),
      t2(10) {

      for(int i = 0; i < 9; i++)
      {
         t0.AddEdge(i, i+1);
         t1.AddEdge(i, i+1);
      }
      t0.AddEdge(0,9);
      t1.AddEdge(0,9);
      t2.AddEdge(0,9);
      t2.AddEdge(9,7);

      stats.PushState(0.3, t0);
      stats.PushState(0.3, t1);
      stats.PushState(0.3, t2);
   }
};
