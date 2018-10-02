#include "OneDLattice.hpp"
#include <algorithm>

OneDLattice::OneDLattice(int num_cells, int radius) :
   _lattice(num_cells),
   _states(num_cells),
   _steps(0)
{
   std::random_device rd;
   _rng.seed(rd());

   for(int i = 0; i < num_cells; i++)
   {
      for(int j = i - radius; j <= i + radius; j++)
      {
         if(i == j)
         {
            continue; // no self edges
         }
         else if(j < 0)
         {
            _lattice.AddEdge(i, num_cells + j);
         }
         else if(j >= num_cells)
         {
            _lattice.AddEdge(i, j - num_cells);
         }
         else
         {
            _lattice.AddEdge(i, j);
         }
      }
   }
}

OneDLattice::~OneDLattice() {}

void OneDLattice::Seed(int seed)
{
   _rng.seed(seed);
}

void OneDLattice::SetDensity(double density)
{
   std::uniform_real_distribution<double> uniform(0.0, 1.0);
   for(int& x : _states)
   {
      double p = uniform(_rng);
      if(p < density)
      {
         x = 1;
      }
      else
      {
         x = 0;
      }
   }
   _steps = 0;
}

double OneDLattice::GetDensity() const
{
   return (double)std::accumulate(_states.begin(), _states.end(), 0) / (double)_states.size();
}

void OneDLattice::Step(Rule* rule)
{
   // XXX: This is (almost) duplicate code from Model::Step
   //      - should refactor!
   std::vector<int> new_states(_states.size());
   for(int i = 0; i < _states.size(); i++)
   {
      auto neighbors = _lattice.GetNeighbors(i);
      std::vector<int> neighbor_states;
      for(int n : neighbors)
      {
         neighbor_states.push_back(_states[n]);
      }
      std::vector<Point> neighbor_positions;
      for(int i = 0; i < neighbors.size(); i++)
      {
         if(i < neighbors.size()/2)
         {
            neighbor_positions.push_back(Point(-1, 0));
         }
         else
         {
            neighbor_positions.push_back(Point(1, 0));
         }
      }
      new_states[i] = rule(_states[i], Point(0,0), neighbor_states, neighbor_positions);
   }
   _old_states = _states;
   _states = new_states;
   _steps++;
}

bool OneDLattice::IsChanging() const
{
   if(_steps <= 1) return true;
   else            return _old_states != _states;
}

void OneDLattice::Shuffle()
{
   std::shuffle(_states.begin(), _states.end(), _rng);
}

std::ostream& operator<<(std::ostream &out, const OneDLattice& l)
{
   for(auto s : l._states)
   {
      out << s;
   }
   return out;
}
