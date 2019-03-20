#ifndef _SURFACE_HPP
#define _SURFACE_HPP

class Surface
{
public:
   Surface() {}
   ~Surface() {}

   virtual double Distance(const Point& p, const Point& q) const = 0;
   virtual Point  Move(const Point& p, double speed, double heading) const = 0;
};

#endif // _SURFACE_HPP
