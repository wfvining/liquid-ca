#ifndef _MOTION_CA_NETWORK_HPP
#define _MOTION_CA_NETWORK_HPP

#include <vector>
#include <set>
#include <stdexcept>
#include <memory>
#include <iostream>

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
   void AddEdge(int i, int j);

   /**
    * Get the density of this snapshot. (proportion of possible edges
    * that actually exist).
    *
    * The density of a network with only one vertex is not a number.
    */
   double Density() const;

   /**
    * Return the average degree of the network
    */
   double AverageDegree() const;

   /**
    * Return the degree distribution in a vector.
    */
   std::vector<unsigned int> DegreeDistribution() const;

   /**
    * Return the normalized degree distribution 
    */
   std::vector<double> NormalizedDegreeDistribution() const;

   /**
    * Get the total number of edges (undirected)
    */
   int EdgeCount() const;

   /**
    * Get the set of neighbors to vertex v.
    *
    * If v is not a node in the network then throws an out_of_range
    * exception.
    */
   std::set<int> GetNeighbors(int v) const;

   /**
    * get the number of vertices
    */
   int Size() const;

   void Union(const NetworkSnapshot& s);

   friend bool operator== (const NetworkSnapshot& s, const NetworkSnapshot& g);
   friend std::ostream& operator<< (std::ostream& out, const NetworkSnapshot& s);
};

class Network
{
private:

   std::vector<std::shared_ptr<NetworkSnapshot>> _snapshots;

public:

   Network();
   ~Network();
   
   /**
    * Append a snapshot to the network.
    */
   void AppendSnapshot(std::shared_ptr<NetworkSnapshot> snapshot);

   /**
    * Get the network snapshot at time t. If t is out of range then
    * throws an exception.
    */
   std::shared_ptr<NetworkSnapshot> GetSnapshot(unsigned int t) const;

   /**
    * Get a snapshot that represents the aggregate of all the
    * snapshots in the network.
    */
   NetworkSnapshot Aggregate() const;

   /**
    * Get the number of snapshots in the network.
    */
   unsigned int Size() const;
};

#endif // _MOTION_CA_NETWORK_HPP