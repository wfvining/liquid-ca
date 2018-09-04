#include "Network.hpp"
#include "Rule.hpp"

#include <random>

#define NUM_NODES 127

int main(int argc, char**argv)
{
   int seed = atoi(argv[1]);
   int edges_per_snapshot = atoi(argv[2]);
   std::mt19937_64 gen(seed);
   std::uniform_int_distribution<int> node_dist(0, NUM_NODES-1);

   NetworkSnapshot network(NUM_NODES);
   for(int i = 0; i < 5000; i++)
   {
      while(network.EdgeCount() < edges_per_snapshot)
      {
         int u = node_dist(gen);
         int v = node_dist(gen);
         network.AddEdge(u, v);
      }
   }
}
