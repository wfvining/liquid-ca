#include "Network.hpp"

#include <algorithm>

/// NetworkSnapshot functions

NetworkSnapshot::NetworkSnapshot(int num_vertices) :
   _num_vertices(num_vertices),
   _adjacency_list(num_vertices)
{}

NetworkSnapshot::~NetworkSnapshot() {}

void NetworkSnapshot::AddEdge(int i, int j)
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
   return n / (_num_vertices * (_num_vertices-1)); // XXX
}

std::set<int> NetworkSnapshot::GetNeighbors(int v) const
{
   if(v < 0 || v >= _num_vertices)
   {
      throw std::out_of_range("Network::GetNeighbors");
   }
   return _adjacency_list[v];
}

double NetworkSnapshot::AverageDegree() const
{
   unsigned int total_degree = 0;
   for(auto& adjacencies : _adjacency_list)
   {
      total_degree += adjacencies.size();
   }
   return total_degree / _num_vertices;
}

double NetworkSnapshot::DegreeVariance() const
{
   double avg = AverageDegree();
   double variance = 0.0;
   for(auto adjacencies : _adjacency_list)
   {
      variance += (avg - adjacencies.size())*(avg - adjacencies.size());
   }
   return variance/_num_vertices;
}

std::vector<unsigned int> NetworkSnapshot::DegreeDistribution() const
{
   std::vector<unsigned int> degree_distribution(_num_vertices);
   std::fill(degree_distribution.begin(), degree_distribution.end(), 0);
   for(auto& al : _adjacency_list)
   {
      unsigned int degree = al.size();
      degree_distribution[degree] += 1;
   }
   return degree_distribution;
}

std::vector<double> NetworkSnapshot::NormalizedDegreeDistribution() const
{
   std::vector<unsigned int> degree_distribution = DegreeDistribution();
   std::vector<double> normalized_distribution(degree_distribution.size());
   std::transform(degree_distribution.begin(), degree_distribution.end(),
                  normalized_distribution.begin(),
                  [this](unsigned int d) { return (double)d/(double)_num_vertices; });
   return normalized_distribution;
}

int NetworkSnapshot::EdgeCount() const
{
   int n = 0;
   for(auto& al : _adjacency_list)
   {
      n += al.size();
   }
   return n / 2;
}

void NetworkSnapshot::Union(const NetworkSnapshot& s)
{
   for(int i = 0; i < _adjacency_list.size(); i++)
   {
      _adjacency_list[i].insert(s._adjacency_list[i].begin(),
                                s._adjacency_list[i].end());
   }
}

int NetworkSnapshot::Size() const
{
   return _adjacency_list.size();
}

int NetworkSnapshot::Degree(int v) const
{
   return _adjacency_list[v].size();
}

bool operator== (const NetworkSnapshot& s, const NetworkSnapshot& g)
{
   return s._adjacency_list == g._adjacency_list;
}

std::ostream& operator<< (std::ostream& out, const NetworkSnapshot& s)
{
   // output the snapshot as dot.
   out << "graph {" << std::endl
       << "  node[shape=point,label=\"\"]" << std::endl;
   for(int u = 0; u < s._adjacency_list.size(); u++)
   {
      out << "  " << u << std::endl;
      for(int v : s._adjacency_list[u])
      {
         if(u < v) {
            out << "  " << u << " -- " << v << std::endl;
         }
      }
   }
   return out << "}";
}

/// Network functions

Network::Network() {}

Network::~Network() {}

void Network::AppendSnapshot(std::shared_ptr<NetworkSnapshot> snapshot)
{
   _snapshots.push_back(snapshot);
}

std::shared_ptr<NetworkSnapshot> Network::GetSnapshot(unsigned int t) const
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

unsigned int Network::Size() const
{
   return _snapshots.size();
}

NetworkSnapshot Network::Aggregate() const
{
   int size = _snapshots[0]->Size();
   NetworkSnapshot aggregate(size);
   for(std::shared_ptr<NetworkSnapshot> snapshot : _snapshots)
   {
      aggregate.Union(*snapshot);
   }
   return aggregate;
}
