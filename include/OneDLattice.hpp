#ifndef _MOTION_CA_ONE_D_LATTICE_HPP
#define _MOTION_CA_ONE_D_LATTICE_HPP

#include <vector>
#include <random>

#include "Network.hpp"
#include "Rule.hpp"

class OneDLattice
{
private:
   NetworkSnapshot  _lattice;
   std::vector<int> _old_states;
   std::vector<int> _states;
   std::mt19937_64  _rng;
   int              _steps;

public:
   OneDLattice(int num_cells, int radius);
   ~OneDLattice();

   /**
    * set the seed to be used by the RNG that initializes the cell
    * states
    */
   void Seed(int s);
   
   /**
    * Reinitialize the lattice with the given density
    */
   void SetDensity(double density);

   /**
    * Get the current density.
    */
   double GetDensity() const;

   /**
    * Step the CA
    */
   void Step(const Rule* rule);

   /**
    * Return true if the state vector is still changing.
    */
   bool IsChanging() const;
};

#endif // _MOTION_CA_ONE_D_LATTICE_HPP