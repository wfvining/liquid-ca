#include "Network.hpp"

/// NetworkSnapshot functions

NetworkSnapshot::NetworkSnapshot(int num_vertices) :
   _num_vertices(num_vertices),
   _adjacency_list(num_vertices)
{}

NetworkSnapshot::~NetworkSnapshot() {}

void NetworkSnapshot::AddEdge(int i, int j) throw(std::out_of_range)
{
   // reject invalid input
   if(i == j || i < 0 || j < 0 || i >= _num_vertices || j >= _num_vertices)
   {
      throw(std::out_of_range("NetworkSnapshot::AddEdge()"));
   }
   else
   {
      _adjacency_list[i].insert(j);
      _adjacency_list[j].insert(i);
   }
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

bool operator== (const NetworkSnapshot& s, const NetworkSnapshot& g)
{
   return s._adjacency_list == g._adjacency_list;
}

/// Network functions

Network::Network() {}

Network::~Network() {}

void Network::AppendSnapshot(const NetworkSnapshot& snapshot)
{
   _snapshots.push_back(snapshot);
}

const NetworkSnapshot& Network::GetSnapshot(unsigned int t) const throw(std::out_of_range)
{
   if(t > _snapshots.size())
   {
      throw(std::out_of_range("Network::GetSnapshot()"));
   }
   else
   {
      return _snapshots[t];
   }
}
