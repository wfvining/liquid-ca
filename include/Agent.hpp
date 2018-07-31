#ifndef _AGENT_HPP
#define _AGENT_HPP

#include "Point.hpp"
#include "Heading.hpp"

#include <random>

class Agent
{
private:
   Point        _position;
   Heading      _heading;
   double       _speed;
   double       _arena_size;
   unsigned int _time;

   // Random number generator for the agent
   std::mt19937_64 _rng;

public:
   /**
    * Construct an agent with a random position and heading.
    */
   Agent(unsigned int seed, double speed, double arena_size);

   /**
    * Construct an agent at position p with heading h.
    */
   Agent(Point p, Heading h, double speed, double arena_size);

   /**
    * Get the current position of the agent.
    */
   Point Position() const;

   /**
    * Compute the position and heading of the agent after a single
    * timestep.
    */
   void Step();
};

#endif // _AGENT_HPP
