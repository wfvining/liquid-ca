#include "Heading.hpp"

#include <cmath> // M_PI

Heading::Heading(double h)
{
   _heading_radians = h - floor(h/(2*M_PI)) * 2*M_PI;
}

Heading::Heading()
{
   _heading_radians = 0;
}

Heading::~Heading() {}

double Heading::Radians() const
{
   return _heading_radians;
}

bool operator== (const Heading& h1, const Heading& h2)
{
   return h1._heading_radians == h2._heading_radians;
}

bool operator!= (const Heading& h1, const Heading& h2)
{
   return !(h1 == h2);
}

Heading operator+ (const Heading& h1, const Heading& h2)
{
   return Heading(h1._heading_radians + h2._heading_radians);
}

Heading operator- (const Heading& h1, const Heading& h2)
{
   return Heading(h1._heading_radians - h2._heading_radians);
}
