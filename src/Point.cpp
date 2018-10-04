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

/**
 * inclusive of the minimum 
 */
bool in_range(double x, double min, double max)
{
   return x >= min && x < max;
}

#if 0
bool Point::NorthEeastOf(const Point& p) const
{
   double adjusted_x = _x - p._x;
   double adjusted_y = _y - p._y;
   double theta = atan2(adjusted_y, adjusted_x);
   return in_range(theta, M_PI/8.0, 3*M_PI/8.0);
}

bool Point::NorthWestOf(const Point& p) const
{
   double adjusted_x = _x - p._x;
   double adjusted_y = _y - p._y;
   double theta = atan2(adjusted_y, adjusted_x);
   return in_range(theta, 5.0*M_PI/8.0, 7.0*M_PI/8.0);
}
#endif

bool Point::NorthOf(const Point& p) const
{
   double adjusted_x = _x - p._x;
   double adjusted_y = _y - p._y;
   double theta = atan2(adjusted_y, adjusted_x);
   return in_range(theta, 3*M_PI/8.0, 5*M_PI/8.0);
}

bool Point::EastOf(const Point& p) const
{
   double adjusted_x = _x - p._x;
   double adjusted_y = _y - p._y;
   double theta = atan2(adjusted_y, adjusted_x);
   return in_range(theta, -M_PI/8.0, M_PI/8.0);
}

bool Point::WestOf(const Point& p) const
{
   double adjusted_x = _x - p._x;
   double adjusted_y = _y - p._y;
   double theta = atan2(adjusted_y, adjusted_x);
   if(theta < 0)
   {
      theta = 2.0*M_PI + theta;
   }
   return in_range(theta, 7.0*M_PI/4.0, 9.0*M_PI/4.0);
}

bool Point::SouthOf(const Point& p) const
{
   double adjusted_x = _x - p._x;
   double adjusted_y = _y - p._y;
   double theta = atan2(adjusted_y, adjusted_x);
   return in_range(theta, -5.0*M_PI/8.0, -3.0*M_PI/8.0);
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
