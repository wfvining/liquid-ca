#include <gtest/gtest.h>

#include "Range.hpp"

TEST(RangeTest, inclusive)
{
   Range r(0.0, 1.0);
   ASSERT_FALSE( r.Contains(-0.1) );
   ASSERT_TRUE(  r.Contains(0.0)  );
   ASSERT_TRUE(  r.Contains(1.0)  );
   ASSERT_TRUE(  r.Contains(0.5)  );
   ASSERT_FALSE( r.Contains(1.1)  );
}

TEST(RangeRest, exclusive)
{
   Range r = Range(0.0, 1.0).ExcludeMin().ExcludeMax();
   ASSERT_FALSE( r.Contains(-0.1) );
   ASSERT_FALSE( r.Contains(0.0)  );
   ASSERT_FALSE( r.Contains(1.0)  );
   ASSERT_TRUE(  r.Contains(0.5)  );
   ASSERT_FALSE( r.Contains(1.1)  );
}

TEST(RangeTest, exclusiveMin)
{
   Range r = Range(0.0, 1.0).ExcludeMin();
   ASSERT_FALSE( r.Contains(0.0)     );
   ASSERT_TRUE(  r.Contains(1.0)     );
   ASSERT_FALSE( r.Contains(1.00001) );
}

TEST(RangeTest, exclusiveMax)
{
   Range r = Range(0.0, 1.0).ExcludeMax();
   ASSERT_TRUE(  r.Contains(0.0) );
   ASSERT_FALSE( r.Contains(1.0) );
}

TEST(RangeTest, negativeRange)
{
   Range r(-2.5, -1.0);
   ASSERT_TRUE( r.Contains(-2.5) );
   ASSERT_TRUE( r.Contains(-1.0) );
   ASSERT_TRUE( r.Contains(-1.5) );

   ASSERT_FALSE( r.Contains(-3.0) );
   ASSERT_FALSE( r.Contains(-0.5) );
   ASSERT_FALSE( r.Contains(1.0)  );
   ASSERT_FALSE( r.Contains(2.5)  );
}

TEST(RangeTest, singleton)
{
   Range r(0.75,0.75);

   ASSERT_TRUE(  r.Contains(0.75) );
   ASSERT_FALSE( r.Contains(0.750000000001) );
}
