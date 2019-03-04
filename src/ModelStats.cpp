#include "ModelStats.hpp"

#include <cmath>

ModelStats::ModelStats(int num_agents) :
   _aggregate_network(num_agents)
{}

ModelStats::~ModelStats() {}

void ModelStats::PushState(double density, std::shared_ptr<NetworkSnapshot> snapshot)
{
   if(!_network_summary_only) {
      _network.AppendSnapshot(snapshot);
   }
   _aggregate_network.Union(*snapshot);
   _network_density.push_back(_aggregate_network.Density());
   _ca_density.push_back(density);
}

void ModelStats::NetworkSummaryOnly()
{
   _network_summary_only = true;
}

const Network& ModelStats::GetNetwork() const
{
   return _network;
}

unsigned int ModelStats::ElapsedTime() const
{
   return _network.Size();
}

bool ModelStats::IsCorrect() const
{
   if(_ca_density.size() == 0)
   {
      return false;
   }
   else if(_ca_density[0] >= 0.5)
   {
      return _ca_density.back() == 1.0;
   }
   else
   {
      return _ca_density.back() == 0.0;
   }
}

bool ModelStats::IsSynchronized() const
{
   if(_ca_density.size() < 2)
   {
      return false;
   }

   return _ca_density.back() == 1 - *(_ca_density.rbegin()+1)
      && _ca_density.back() == *(_ca_density.rbegin()+2);
}

const std::vector<double>& ModelStats::GetDensityHistory() const
{
   return _ca_density;
}

std::vector<double> ModelStats::AggregateDensityHistory() const
{
   return _network_density;
}

double ModelStats::AverageAggregateDegree() const
{
   return _aggregate_network.AverageDegree();
}

double ModelStats::AggregateDegreeStdDev() const
{
   return sqrt(_aggregate_network.DegreeVariance());
}

double ModelStats::MedianAggregateDegree() const
{
   return _aggregate_network.MedianDegree();
}
