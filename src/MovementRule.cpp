#include "MovementRule.hpp"

#include <cmath> // M_PI

LevyWalk::LevyWalk(double mu, int max_step) :
   mu(mu),
   max_step(max_step),
   next_turn(0),
   current_time(0),
   heading_distribution(0, 2*M_PI)
{}

LevyWalk::~LevyWalk() {}

int gen_power_law(double mu, int max_step, std::mt19937_64& gen)
{
   std::uniform_real_distribution<double> u(0.0,1.0);
   double pmin = powf(1.0, -mu+1);
   double pmax = powf((double)max_step, -mu+1);
   double z    = powf((pmax - pmin)*u(gen) + pmin, 1.0/(-mu+1));
   return floor(z);
}

Heading LevyWalk::Turn(const Point&     current_position,
                       const Heading&   current_heading,
                       std::mt19937_64& gen)
{
   current_time++;
   if(current_time >= next_turn)
   {
      next_turn = current_time+gen_power_law(mu, max_step, gen);
      return Heading(heading_distribution(gen));
   }
   else
   {
      return current_heading;
   }
}

std::shared_ptr<MovementRule> LevyWalk::Clone() const
{
   return std::make_shared<LevyWalk>(*this);
}
