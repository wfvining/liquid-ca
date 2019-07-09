#include "Agent.hpp"

#include <cmath> // M_PI
#include <climits>

Agent::Agent(Point p, Heading h, double speed, double arena_size, int seed) :
   _speed(speed),
   _arena_size(arena_size),
   _position(p),
   _previous_heading(h + Heading(M_PI)),
   _heading(h),
   _gen(seed)
{
   _movement_rule = std::make_shared<MovementRule>();
}

Point Agent::Position() const
{
   return _position;
}

Heading Agent::GetHeading() const
{
   return _heading;
}

Heading Agent::GetPreviousHeading() const
{
   return _previous_heading;
}

void Agent::SetHeading(Heading h)
{
   _heading = h;
}

void Agent::Step()
{
   double dx = _speed * cos(_heading.Radians());
   double dy = _speed * sin(_heading.Radians());
   _position = Point(_position.GetX() + dx, _position.GetY() + dy);
   while(IsOutOfBounds(_position))
   {
      _position = Reflect(_position);
   }
   _previous_heading = _heading;
   _heading = _movement_rule->Turn(_position, _heading, _gen);
}

void Agent::SetMovementRule(std::shared_ptr<MovementRule> rule)
{
   _movement_rule = rule;
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
