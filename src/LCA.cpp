#include "LCA.hpp"

LCA::LCA(const Model& model, std::shared_ptr<Rule> rule, int max_time) :
   model_(std::make_unique<Model>(model)),
   max_time_(max_time),
   update_rule_(rule)
{}

void LCA::Run()
{
   for(int i = 0; i < max_time_; i++)
   {
      model_->Step(update_rule_);
   }
}
