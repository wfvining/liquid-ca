#ifndef _POINT_HPP
#define _POINT_HPP

/**
 * A point in the plane.
 */
class Point
{
private:
   double _x;
   double _y;
public:
   Point(double x, double y);
   ~Point();

   /**
    * The x-coordinate
    */
   double GetX() const;

   /**
    * The y-coordinate
    */
   double GetY() const;

   /**
    * Get the euclidian distance between two points.
    */
   double Distance(const Point& p) const;

   /**
    * Test whether two points lie within distance d of eachother.
    */
   bool Within(double d, const Point& p) const;

   friend bool operator== (const Point& p, const Point& q);
   friend bool operator!= (const Point& p, const Point& q);
};

#endif // _POINT_HPP
