#include <gtest/gtest.h>

#include "Point.hpp"

class PointTest : public ::testing::Test
{
public:
   Point p1;
   Point p2;
   Point p3;
   Point p4;
   Point p5;
   Point p6;
   Point p7;

   PointTest() :
      p1(0,0),
      p2(0,1),
      p3(1,0),
      p4(1,2),
      p5(-3,-4),
      p6(1,-2),
      p7(-1,2)
      {}
   ~PointTest() {}
};

TEST_F(PointTest, equalsSelf)
{
   ASSERT_EQ(p1, p1);
   ASSERT_EQ(p2, p2);
   ASSERT_EQ(p3, p3);
   ASSERT_EQ(p4, p4);
   ASSERT_EQ(p5, p5);
}

TEST_F(PointTest, equalsOtherIsFalse)
{
   ASSERT_FALSE(p4 == p2);
}

TEST_F(PointTest, notEqual)
{
   ASSERT_NE(p1, p2);
   ASSERT_NE(p1, p3);
   ASSERT_NE(p1, p4);
   ASSERT_NE(p1, p5);

   ASSERT_NE(p2, p3);

   ASSERT_NE(p6, p7);
   ASSERT_NE(p7, p6);
}

TEST_F(PointTest, distanceToSelfIsZero)
{
   ASSERT_EQ(0, p1.Distance(p1));
   ASSERT_EQ(0, p2.Distance(p2));
   ASSERT_EQ(0, p3.Distance(p3));
   ASSERT_EQ(0, p4.Distance(p4));
   ASSERT_EQ(0, p5.Distance(p5));
   ASSERT_EQ(0, p6.Distance(p6));
   ASSERT_EQ(0, p7.Distance(p7));
}

TEST_F(PointTest, unitDistance)
{
   ASSERT_EQ(1, p1.Distance(Point(0,1)));
   ASSERT_EQ(1, p1.Distance(Point(1,0)));
   ASSERT_EQ(1, p1.Distance(Point(-1,0)));
   ASSERT_EQ(1, p1.Distance(Point(0,-1)));
}
