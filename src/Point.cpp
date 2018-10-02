#include "Point.hpp"
#include <cmath>

Point::Point(double x, double y) :
   _x(x),
   _y(y)
{}

Point::~Point() {}

double Point::GetX() const
{
   return _x;
}

double Point::GetY() const
{
   return _y;
}

double Point::Distance(const Point& p) const
{
   return sqrt(pow(_x - p._x, 2) + pow(_y - p._y, 2));
}

bool Point::NorthOf(const Point& p) const
{
   // TODO: return true if this is in the 90 degree sector above the
   // point p.
   double adjusted_x = _x - p._x;
   double adjusted_y = _y - p._y;
   return false;
}

bool Point::Within(double d, const Point& p) const
{
   return Distance(p) <= d;
}

bool operator== (const Point& p, const Point& q)
{
   return (p._x == q._x) && (p._y == q._y);
}

bool operator!= (const Point& p, const Point& q)
{
   return !(p == q);
}

std::ostream& operator<< (std::ostream& out, const Point& p)
{
   return out << "Point(" << p._x << "," << p._y << ")";
}
