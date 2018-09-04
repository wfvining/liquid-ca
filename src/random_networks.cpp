#include "Network.hpp"
#include "Rule.hpp"

#include <random>

#define NUM_NODES 127

int main(int argc, char**argv)
{
   int seed = atoi(argv[1]);
   int k    = atoi(argv[2]);
   std::mt19937_64 gen(seed);
   std::uniform_int_distribution<int> node_dist(0, NUM_NODES-1);

   NetworkSnapshot network(NUM_NODES);
   for(int node = 0; node < NUM_NODES; node++)
   {
      while(network.Degree(node) < k)
      {
         int u = node_dist(gen);
         network.AddEdge(node, u);
      }
   }
}
