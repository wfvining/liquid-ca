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
   unsigned int _next_update;

   Point Reflect(const Point& p);
   bool  IsOutOfBounds(const Point& p) const;

public:

   /**
    * Construct an agent at position p with heading h.
    */
   Agent(Point p, Heading h, double speed, double arena_size);

   /**
    * Get the current position of the agent.
    */
   Point Position() const;

   /**
    * Get the current heading of the agent.
    */
   Heading GetHeading() const;

   /**
    * Set a new heading.
    */
   void SetHeading(Heading h);

   /**
    * Compute the position and heading of the agent after a single
    * timestep.
    */
   void Step();

   /**
    * Set the interval to the next heading update.
    */
   void SetUpdateInterval(int interval);

   /**
    * Returns true if the update interval has elapsed.
    */
   bool ShouldTurn() const;
};

#endif // _AGENT_HPP
