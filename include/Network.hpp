#ifndef _MOTION_CA_NETWORK_HPP
#define _MOTION_CA_NETWORK_HPP

#include <vector>
#include <set>

class NetworkSnapshot
{
private:

   std::vector<std::set<int>> _adjacency_list;
   int _num_vertices;

public:

   NetworkSnapshot(int num_vertices);
   ~NetworkSnapshot();

   /**
    * Add an edge between vertices i and j to the snapshot.
    *
    * If the edge is invalid then the snapshot remains unchanged.
    * (Invalid means that i==j, or one of i or j is outside the range
    * [0,num_vertices))
    */
   void AddEdge(int i, int j);

   /**
    * Get the density of this snapshot. (proportion of possible edges
    * that actually exist).
    *
    * The density of a network with only one vertex is not a number.
    */
   double Density() const;
};

class Network
{
private:

   std::vector<NetworkSnapshot> _snapshots;

public:

   Network();
   ~Network();
   
   /**
    * Append a snapshot to the network.
    */
   void AppendSnapshot(const NetworkSnapshot& snapshot);
};

#endif // _MOTION_CA_NETWORK_HPP