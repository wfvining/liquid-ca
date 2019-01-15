#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cmath>

#include "Network.hpp"

class NetworkTest : public ::testing::Test
{
public:
   NetworkSnapshot fully_connected_snapshot;
   NetworkSnapshot fully_disconnected_snapshot;
   NetworkTest() :
      fully_connected_snapshot(10),
      fully_disconnected_snapshot(10)
      {
         for(int i = 0; i < 10; i++)
         {
            for(int j = i+1; j < 10; j++)
            {
               fully_connected_snapshot.AddEdge(i,j);
            }
         }
      }
   ~NetworkTest() {}
};

TEST_F(NetworkTest, fullyConnectedDensityIsOne)
{
   EXPECT_EQ(1.0, fully_connected_snapshot.Density());
}

TEST_F(NetworkTest, fullyDisconnectedDensityIsZero)
{
   EXPECT_EQ(0.0, fully_disconnected_snapshot.Density());
}

TEST_F(NetworkTest, oneVertexDensity)
{
   NetworkSnapshot s(1);
   EXPECT_TRUE(std::isnan(s.Density()));
}

TEST_F(NetworkTest, twoVertexDensity)
{
   NetworkSnapshot s(2);
   EXPECT_EQ(0, s.Density());
   s.AddEdge(0,1);
   EXPECT_EQ(1, s.Density());
}

TEST_F(NetworkTest, densityIncreasesToMax)
{
   NetworkSnapshot s(3);
   s.AddEdge(0,1);
   ASSERT_THAT(s.Density(), ::testing::DoubleEq(1.0/3.0));
   s.AddEdge(1,2);
   ASSERT_THAT(s.Density(), ::testing::DoubleEq(2.0/3.0));
   s.AddEdge(2,0);
   ASSERT_EQ(s.Density(), 1.0);
}

TEST_F(NetworkTest, equality)
{
   EXPECT_EQ(fully_connected_snapshot, fully_connected_snapshot);
   EXPECT_FALSE(fully_connected_snapshot == fully_disconnected_snapshot);
}

TEST_F(NetworkTest, getNeighborsEmpty)
{
   for(int i = 0; i < 10; i++)
   {
      EXPECT_EQ(fully_disconnected_snapshot.GetNeighbors(i).size(), 0);
   }
}

TEST_F(NetworkTest, getNeighborsNonEmpty)
{
   for(int i = 0; i < 10; i++)
   {
      auto neighbors = fully_connected_snapshot.GetNeighbors(i);
      for(int j = 0; j < 10; j++)
      {
         if(i != j)
         {
            EXPECT_NE(neighbors.find(j), neighbors.end());
         }
         else
         {
            EXPECT_EQ(neighbors.find(j), neighbors.end());
         }
      }
   }
}

TEST_F(NetworkTest, edgeCount)
{
   EXPECT_EQ(45, fully_connected_snapshot.EdgeCount());
   EXPECT_EQ(0, fully_disconnected_snapshot.EdgeCount());
}

TEST_F(NetworkTest, averageDegree)
{
   EXPECT_EQ(0, fully_disconnected_snapshot.AverageDegree());
   EXPECT_EQ(9, fully_connected_snapshot.AverageDegree());
}

TEST_F(NetworkTest, medianDegreeEmpty)
{
   EXPECT_EQ(0.0, fully_disconnected_snapshot.MedianDegree());
}

TEST_F(NetworkTest, medianDegreeFull)
{
   EXPECT_EQ(9.0, fully_connected_snapshot.MedianDegree());
}

TEST_F(NetworkTest, medianDegreeEvenNumberOfNodes)
{
   NetworkSnapshot n(4);
   EXPECT_EQ(0.0, n.MedianDegree());
   n.AddEdge(0,1);
   EXPECT_EQ(0.5, n.MedianDegree());
   n.AddEdge(1,2);
   EXPECT_EQ(1.0, n.MedianDegree());
   n.AddEdge(2,3);
   EXPECT_EQ(1.5, n.MedianDegree());
}

TEST_F(NetworkTest, medianDegreeOddNumberOfNodes)
{
   NetworkSnapshot n(5);
   EXPECT_EQ(0.0, n.MedianDegree());
   n.AddEdge(0,1);
   EXPECT_EQ(0.0, n.MedianDegree());
   n.AddEdge(1,2);
   EXPECT_EQ(1.0, n.MedianDegree());
   n.AddEdge(1,3);
   n.AddEdge(1,4);
   EXPECT_EQ(1.0, n.MedianDegree());
   n.AddEdge(4,0);
   EXPECT_EQ(2.0, n.MedianDegree());
}

TEST_F(NetworkTest, degreeDistribution1)
{
   NetworkSnapshot d1(6);
   d1.AddEdge(0,1);
   d1.AddEdge(2,3);
   d1.AddEdge(4,5);
   auto d1_dist = d1.DegreeDistribution();
   EXPECT_EQ(d1_dist[0], 0);
   EXPECT_EQ(d1_dist[1], 6);
   for(auto iter = d1_dist.begin() + 2; iter < d1_dist.end(); iter++)
   {
      EXPECT_EQ(*iter, 0);
   }
}

TEST_F(NetworkTest, degreeDistributionSize)
{
   auto full_dist = fully_connected_snapshot.DegreeDistribution();
   auto empty_dist = fully_disconnected_snapshot.DegreeDistribution();
   ASSERT_EQ(10, full_dist.size());
   ASSERT_EQ(10, empty_dist.size());
}

TEST_F(NetworkTest, degreeDistributionFull)
{
   auto full_dist = fully_connected_snapshot.DegreeDistribution();
   for(int i = 0; i < full_dist.size(); i++)
   {
      if(i == 9)
      {
         ASSERT_EQ(10, full_dist[i]);
      }
      else
      {
         EXPECT_EQ(0, full_dist[i]);
      }
   }
}

TEST_F(NetworkTest, degreeDistributionEmpty)
{
   auto full_dist = fully_disconnected_snapshot.DegreeDistribution();
   for(int i = 0; i < full_dist.size(); i++)
   {
      if(i == 0)
      {
         ASSERT_EQ(10, full_dist[i]);
      }
      else
      {
         EXPECT_EQ(0, full_dist[i]);
      }
   }
}

TEST_F(NetworkTest, snapshotUnion)
{
   NetworkSnapshot snapshot1(10);
   NetworkSnapshot snapshot2(10);
   NetworkSnapshot snapshot3(10);
   NetworkSnapshot snapshot_final(10);
   snapshot1.AddEdge(0,1);
   snapshot1.AddEdge(0,2);
   snapshot2.AddEdge(0,1);
   snapshot2.AddEdge(0,2);
   snapshot3.AddEdge(0,3);
   snapshot3.AddEdge(0,4);
   snapshot3.AddEdge(0,5);
   snapshot3.AddEdge(0,6);

   for(int i = 1; i <= 6; i++)
   {
      snapshot_final.AddEdge(0,i);
   }

   NetworkSnapshot u(10);
   u.Union(snapshot1);
   u.Union(snapshot2);
   u.Union(snapshot3);

   ASSERT_EQ(u, snapshot_final);
}
