#include "Agent.hpp"

#include <cmath> // M_PI

Agent::Agent(unsigned int seed, double speed, double arena_size) :
   _speed(speed),
   _arena_size(arena_size),
   _rng(seed),
   _time(0),
   _position(0,0),
   _heading(0)
{
   std::uniform_real_distribution<double> coordinate_distribution(-arena_size/2, arena_size/2);
   std::uniform_real_distribution<double> heading_distribution(0, 2*M_PI);
   _position = Point(coordinate_distribution(_rng), coordinate_distribution(_rng));
   _heading  = Heading(heading_distribution(_rng));
}

Agent::Agent(Point p, Heading h, double speed, double arena_size) :
   _speed(speed),
   _arena_size(arena_size),
   _rng(0),
   _position(p),
   _heading(h)
{}

Point Agent::Position() const
{
   return _position;
}

Heading Agent::GetHeading() const
{
   return _heading;
}

void Agent::Step()
{
   double dx = _speed * cos(_heading.Radians());
   double dy = _speed * sin(_heading.Radians());
   Point new_position = Point(_position.GetX() + dx, _position.GetY() + dy);
   if(IsOutOfBounds(new_position))
   {
      _position = Reflect(new_position);
   }
   else
   {
      _position = new_position;
   }
}

bool Agent::IsOutOfBounds(const Point& p) const
{
   return p.GetX() < (-_arena_size / 2)
                     || p.GetX() > (_arena_size / 2)
      || p.GetY() < (-_arena_size / 2)
                    || p.GetY() > (_arena_size / 2);
}

Point Agent::Reflect(const Point& p)
{
   double new_x = p.GetX();
   double new_y = p.GetY();
   if(p.GetX() > _arena_size/2) {
      new_x = _arena_size/2 - (p.GetX() - _arena_size / 2);
      _heading = Heading(M_PI) - _heading;
   }
   else if(p.GetX() < -_arena_size/2) {
      new_x = -_arena_size/2 - (p.GetX() + _arena_size / 2);
      _heading = Heading(M_PI) - _heading;
   }

   if(p.GetY() > _arena_size/2) {
      new_y = _arena_size/2 - (p.GetY() - _arena_size/2);
      _heading = Heading(2*M_PI) - _heading;
   }
   else if(p.GetY() < -_arena_size/2) {
      new_y = -_arena_size/2 - (p.GetY() + _arena_size/2);
      _heading = Heading(2*M_PI) - _heading;
   }

   return Point(new_x, new_y);
}
