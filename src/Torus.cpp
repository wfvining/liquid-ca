#include <algorithm>

#include "Torus.hpp"

Torus::Torus(double width, double height) :
   width(width),
   height(height)
{}

Torus::~Torus() {}

double Torus::Distance(const Point& p, const Point q)
{
   double dx = min(fabs(p.GetX() - q.GetX()), width - fabs((p.GetX() - q.GetX())));
   double dy = min(fabs(p.GetY() - q.GetY()), height - fabs((p.GetX() - q.GetX())));

   return sqrt(dx*dx + dy*dy);
}
