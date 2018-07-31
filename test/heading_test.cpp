#include <gtest/gtest.h>
#include "Heading.hpp"

#include <cmath> // M_PI

TEST(HeadingTest, equalsSelf)
{
   Heading h0(0);
   Heading h2pi(2*M_PI);
   Heading pi(M_PI);
   Heading minusPi(-M_PI);

   ASSERT_EQ(h0, h0);
   ASSERT_EQ(h2pi, h2pi);
   ASSERT_EQ(pi, pi);
   ASSERT_EQ(minusPi, minusPi);
}

TEST(HeadingTest, equivalentHeadings)
{
   Heading zero(0);
   Heading twoPi(2*M_PI);
   Heading minusTwoPi(-2*M_PI);

   Heading pi_2(M_PI_2);
   Heading five_pi_2(5*M_PI_2);

   ASSERT_EQ(zero, twoPi);
   ASSERT_EQ(zero, minusTwoPi);

   ASSERT_EQ(pi_2, five_pi_2);
   ASSERT_EQ(five_pi_2, pi_2);
}

TEST(HeadingTest, additionCommutes)
{
   Heading h1(2.3);
   Heading h2(0.75);

   EXPECT_EQ(h1+h2, h2+h1);
}

TEST(HeadingTest, additiveInverse)
{
   Heading h1(1.2);
   Heading h2(-1.2);

   EXPECT_EQ(h1 + h2, Heading(0));
}

TEST(HeadingTest, subtractSelf)
{
   Heading h0(0);
   Heading h1(1.3);
   Heading h2(-3.0);

   EXPECT_EQ(Heading(0), h0 - h0);
   EXPECT_EQ(Heading(0), h1 - h1);
   EXPECT_EQ(Heading(0), h2 - h2);
}

TEST(HeadingTest, toRadians)
{
   Heading h0(0);
   Heading h1(2*M_PI);
   Heading h2(-M_PI);

   EXPECT_EQ(0, h0.Radians());
   EXPECT_EQ(0, h1.Radians());
   EXPECT_EQ(M_PI, h2.Radians());
}
