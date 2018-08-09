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
   EXPECT_EQ(1, fully_connected_snapshot.Density());
}

TEST_F(NetworkTest, fullyDisconnectedDensityIsZero)
{
   EXPECT_EQ(0, fully_disconnected_snapshot.Density());
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