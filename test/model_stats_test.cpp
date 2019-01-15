#include <gtest/gtest.h>

#include "Model.hpp"
#include "Network.hpp"

class ModelStatsTest : public ::testing::Test
{
public:
   ModelStats empty;
   ModelStats stats;
   ModelStats stats_dense;
   ModelStats stats_zero;
   ModelStats stats_one;
   ModelStats stats_synchronized;

   std::shared_ptr<NetworkSnapshot> t0;
   std::shared_ptr<NetworkSnapshot> t1;
   std::shared_ptr<NetworkSnapshot> t2;
   ModelStatsTest() :
      empty(10),
      stats(10),
      stats_dense(10),
      stats_zero(10),
      stats_one(10),
      stats_synchronized(10)
   {
      t0 = std::make_shared<NetworkSnapshot>(10);
      t1 = std::make_shared<NetworkSnapshot>(10);
      t2 = std::make_shared<NetworkSnapshot>(10);

      for(int i = 0; i < 9; i++)
      {
         t0->AddEdge(i, i+1);
         t1->AddEdge(i, i+1);
      }

      t1->AddEdge(0,9);
      t2->AddEdge(0,9);
      t2->AddEdge(9,7);

      stats.PushState(0.1, t0);
      stats.PushState(0.2, t1);
      stats.PushState(0.3, t2);

      stats_dense.PushState(0.53, t0);
      stats_dense.PushState(0.63, t1);
      stats_dense.PushState(0.73, t2);

      stats_zero.PushState(0.0, t0);
      stats_one.PushState(1.0, t1);

      stats_synchronized.PushState(0.23, t0);
      stats_synchronized.PushState(0.0, t0);
      stats_synchronized.PushState(1.0, t0);
   }
};

TEST_F(ModelStatsTest, densityOrder)
{
   const std::vector<double>& dh = stats.GetDensityHistory();
   EXPECT_EQ(0.1, dh[0]);
   EXPECT_EQ(0.2, dh[1]);
   EXPECT_EQ(0.3, dh[2]);
}

TEST_F(ModelStatsTest, emptyNetwork)
{
   EXPECT_EQ(0, empty.GetNetwork().Size());
   EXPECT_EQ(0, empty.GetDensityHistory().size());
}

TEST_F(ModelStatsTest, snapshotOrder)
{
   const Network& network = stats.GetNetwork();
   EXPECT_EQ(network.GetSnapshot(0), t0);
   EXPECT_EQ(network.GetSnapshot(1), t1);
   EXPECT_EQ(network.GetSnapshot(2), t2);
}

TEST_F(ModelStatsTest, elapsedTime)
{
   EXPECT_EQ(3, stats.ElapsedTime());
   EXPECT_EQ(0, empty.ElapsedTime());
}

TEST_F(ModelStatsTest, correctClassification)
{
   stats.PushState(0.0, t1);
   EXPECT_TRUE(stats.IsCorrect());

   stats_dense.PushState(1.0, t0);
   EXPECT_TRUE(stats_dense.IsCorrect());

   EXPECT_TRUE(stats_one.IsCorrect());
   EXPECT_TRUE(stats_zero.IsCorrect());
}

TEST_F(ModelStatsTest, incorrectClassification)
{
   EXPECT_FALSE(stats.IsCorrect());
   EXPECT_FALSE(stats_dense.IsCorrect());
   EXPECT_FALSE(empty.IsCorrect());

   stats.PushState(1.0, t1);
   EXPECT_FALSE(stats.IsCorrect());

   stats_dense.PushState(0.0, t1);
   EXPECT_FALSE(stats.IsCorrect());
}

TEST_F(ModelStatsTest, synchronizedTest)
{
   EXPECT_FALSE(stats_synchronized.IsSynchronized());
   stats_synchronized.PushState(0.0, t0);
   EXPECT_TRUE(stats_synchronized.IsSynchronized());
   EXPECT_FALSE(stats_dense.IsSynchronized());
}

TEST_F(ModelStatsTest, medianAggregateDegreeNoEdges)
{
   EXPECT_EQ(0.0, empty.MedianAggregateDegree());
}
