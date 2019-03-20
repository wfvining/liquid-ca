#ifndef _BOUNDED_RECTANGLE_HPP
#define _BOUNDED_RECTANGLE_HPP

#include "Surface.hpp"

class BoundedRectangle : public Surface
{
private:
   double width;
   double height;
public:
   double Distance(const Point& p, const Point& q) const override;
   Point  Move(const Point& p, double speed, double heading) const override;
};

#endif // _BOUNDED_RECTANGLE_HPP
