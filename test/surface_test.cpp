#include <getst/gtest.h>

#include "Point.hpp"
#include "Surface.hpp"

class TorusTest : public ::testing::Test
{
public:
   Torus unit;
   Torus hundred;
   Torus nonSquare;

   TorusTest() : unit(1.0,1.0)
};
