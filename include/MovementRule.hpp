#ifndef _MOVEMENT_RULE_HPP
#define _MOVEMENT_RULE_HPP

#include <random>
#include <memory>

#include "Point.hpp"
#include "Heading.hpp"

class MovementRule
{
public:
   MovementRule() {}
   ~MovementRule() {}

   /**
    * Generate a new heading.
    *
    * TODO: make this a template so we can use different random
    * generators.
    */
   virtual Heading Turn(const Point&     current_position,
                        const Heading&   current_heading,
                        std::mt19937_64& gen)
      {
         return current_heading;
      }

   /**
    * Polymorphic constructor idiom. Create a copy of this rule.
    */
   virtual std::shared_ptr<MovementRule> Clone() const
      {
         return std::make_shared<MovementRule>(*this);
      }
};

class LevyWalk : public MovementRule
{
private:
   unsigned int next_turn;
   unsigned int current_time;
   std::uniform_real_distribution<double> heading_distribution;
   double mu;
   int    max_step;
public:
   LevyWalk(double mu, int max_step);
   ~LevyWalk();

   Heading Turn(const Point&     current_position,
                const Heading&   current_heading,
                std::mt19937_64& gen) override;
   std::shared_ptr<MovementRule> Clone() const override;
};

class CorrelatedRandomWalk : public MovementRule
{
public:
   CorrelatedRandomWalk(double sigma);
   ~CorrelatedRandomWalk();

   Heading Turn(const Point&     current_position,
                const Heading&   current_heading,
                std::mt19937_64& gen) override;
   std::shared_ptr<MovementRule> Clone() const override;
};

class RandomWalk : public MovementRule
{
private:
   std::uniform_real_distribution<double> heading_distribution;
public:
   RandomWalk();
   ~RandomWalk();

   Heading Turn(const Point&, const Heading&, std::mt19937_64& gen) override;
   std::shared_ptr<MovementRule> Clone() const override;
};

#endif // _MOVEMENT_RULE_HPP