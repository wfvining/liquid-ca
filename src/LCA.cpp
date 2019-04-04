#include "LCA.hpp"

LCA::LCA(const Model& model, std::shared_ptr<Rule> rule, int max_time) :
   model_(std::make_unique<Model>(model)),
   max_time_(max_time),
   update_rule_(rule)
{}

LCA::~LCA() {}

void LCA::Run()
{
   for(int i = 0; i < max_time_; i++)
   {
      model_->Step(update_rule_.get());
   }
}

void LCA::Run(std::function<bool(const ModelStats&)> early_stop)
{
   for(int i = 0; i < max_time_; i++)
   {
      if(early_stop(GetStats()))
         return;
      model_->Step(update_rule_.get());
   }
}

void LCA::Run(int k)
{
   for(int i = 0; i < k; i++)
   {
      model_->Step(update_rule_.get());
   }
}

const std::vector<Agent>& LCA::GetAgents() const
{
   return model_->GetAgents();
}

const std::vector<int>& LCA::GetStates() const
{
   return model_->GetStates();
}

std::shared_ptr<NetworkSnapshot> LCA::CurrentNetwork() const
{
   return model_->CurrentNetwork();
}

const ModelStats& LCA::GetStats() const
{
   return model_->GetStats();
}

double LCA::CurrentDensity() const
{
   return model_->CurrentDensity();
}

void LCA::MinimizeMemory()
{
   model_->RecordNetworkDensityOnly();
}
