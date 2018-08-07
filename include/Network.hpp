#ifndef _MOTION_CA_NETWORK_HPP
#define _MOTION_CA_NETWORK_HPP

#include <vector>
#include <set>
#include <stdexcept>

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
    * If the edge is invalid then the snapshot remains unchanged and
    * an out_of_range exception is thrown.
    */
   void AddEdge(int i, int j) throw(std::out_of_range);

   /**
    * Get the density of this snapshot. (proportion of possible edges
    * that actually exist).
    *
    * The density of a network with only one vertex is not a number.
    */
   double Density() const;

   friend bool operator== (const NetworkSnapshot& s, const NetworkSnapshot& g);
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

   /**
    * Get the network snapshot at time t. If t is out of range then
    * throws an exception.
    */
   const NetworkSnapshot& GetSnapshot(unsigned int t) const throw(std::out_of_range);

   /**
    * Get the number of snapshots in the network.
    */
   unsigned int Size() const;
};

#endif // _MOTION_CA_NETWORK_HPP