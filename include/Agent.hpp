#ifndef _AGENT_HPP
#define _AGENT_HPP

#include "Point.hpp"
#include "Heading.hpp"
#include "MovementRule.hpp"

#include <random>
#include <memory>

class Agent
{
private:
   Point        _position;
   Heading      _heading;
   Heading      _previous_heading;
   double       _speed;
   double       _arena_size;
   int _time;
   int _next_update;

   std::shared_ptr<MovementRule> _movement_rule;

   std::mt19937_64 _gen;

   Point Reflect(const Point& p);
   bool  IsOutOfBounds(const Point& p) const;

public:

   /**
    * Construct an agent at position p with heading h.
    */
   Agent(Point p, Heading h, double speed, double arena_size, int seed);

   /**
    * Get the current position of the agent.
    */
   Point Position() const;

   /**
    * Get the current heading of the agent.
    */
   Heading GetHeading() const;

   /**
    * Get the heading of the last move made by the agent.
    */
   Heading GetPreviousHeading() const;

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
    * Set the movement rule for the agent.
    */
   void SetMovementRule(std::shared_ptr<MovementRule> movement_rule);

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
