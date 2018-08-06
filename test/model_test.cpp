#include <gmock/gmock.h>

#include "Model.hpp"

TEST(ModelTest, expectedDensityIsCorrect)
{
   double sum = 0;
   for(int i = 0; i < 100; i++)
   {
      Model m(1337+i, 100, 256, 0.5);
      sum += m.CurrentDensity();
   }
   EXPECT_THAT(sum/100.0, ::testing::DoubleNear(0.5, 0.01));

   sum = 0;
   for(int i = 0; i < 100; i++)
   {
      Model m(1337+i, 100, 256, 0.25);
      sum += m.CurrentDensity();
   }
   EXPECT_THAT(sum/100.0, ::testing::DoubleNear(0.25, 0.01));

   sum = 0;
   for(int i = 0; i < 100; i++)
   {
      Model m(1337+i, 100, 256, 0.75);
      sum += m.CurrentDensity();
   }
   EXPECT_THAT(sum/100.0, ::testing::DoubleNear(0.75, 0.01));
}

TEST(ModelTest, densityExtremes)
{
   Model m0(12345, 100, 256, 0.0);
   Model m1(12345, 100, 256, 1.0);

   EXPECT_EQ(m0.CurrentDensity(), 0.0);
   EXPECT_EQ(m1.CurrentDensity(), 1.0);
}

TEST(ModelTest, differentSeedDifferentDensity)
{
   Model m1(1337, 100, 128, 0.5);
   Model m2(1338, 100, 128, 0.5);

   EXPECT_NE(m1.CurrentDensity(), m2.CurrentDensity());
}

TEST(ModelTest, extremelySmallArena)
{
   Model m1(123, 0.25, 32, 0.5);
}