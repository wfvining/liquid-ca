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

void Agent::Step()
{
   // TODO
}