#include "Network.hpp"

/// NetworkSnapshot functions

NetworkSnapshot::NetworkSnapshot(int num_vertices) :
   _num_vertices(num_vertices),
   _adjacency_list(num_vertices)
{}

NetworkSnapshot::~NetworkSnapshot() {}

void NetworkSnapshot::AddEdge(int i, int j)
{
   // quietly reject invalid input
   if(i == j || i < 0 || j < 0 || i >= _num_vertices || j >= _num_vertices)
   {
      return;
   }

   _adjacency_list[i].insert(j);
   _adjacency_list[j].insert(i);
}

double NetworkSnapshot::Density() const
{
   double n = 0;
   for(auto& v : _adjacency_list)
   {
      n += v.size();
   }
   return n / (_num_vertices * (_num_vertices-1));
}

/// Network functions

Network::Network() {}

Network::~Network() {}

void Network::AppendSnapshot(const NetworkSnapshot& snapshot)
{
   _snapshots.push_back(snapshot);
}
