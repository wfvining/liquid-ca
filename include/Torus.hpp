#ifndef _TORUS_HPP
#define _TORUS_HPP

#include "Surface.hpp"

class Torus : public Surface
{
private:
   double width;
   double height;

public:
   Torus(double width, double height);
   ~Torus();

   /**
    * Returns the minimum distance between two points.
    */
   double Distance(const Point& p, const Point& q) const override;
   Point  Move(const Point& p, double speed, double heading) const override;
};

#endif // _TORUS_HPP
