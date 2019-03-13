#include "LCAFactory.hpp"

#include <getopt.h>
#include <limits>

LCAFactory::LCAFactory(int argc, char** argv) :
   num_agents_(255),
   communication_range_(5),
   arena_size_(100),
   speed_(1),
   seed_(-1),
   max_time_(5000),
   rule_(Identity()),
   init_(Uniform),
{
   movement_rule_ = std::make_unique<RandomWalk>();
   seed_distribution_ = std::uniform_int_distribution<unsigned int>(numeric_limits<unsigned int>::max())
}

void LCAFactory::Init(int argc, char** argv)
{
   int by_position = 0;

   static struct option long_options[] =
      {
         {"communication-range", required_argument, 0,            'r'},
         {"num-agents",          required_argument, 0,            'n'},
         {"arena-size",          required_argument, 0,            'a'},
         {"seed",                required_argument, 0,            's'},
         {"rule",                required_argument, 0,            'R'},
         {"correlated",          required_argument, 0,            'c'},
         {"max-time",            required_argument, 0,            'T'},
         {"by-position",         no_argument,       &by_position, 'p'},
         {0,0,0,0}
      };
   int option_index = 0;

   while((opt_char = getopt_long(argc, argv, "r:n:a:s:S:i:c:R:T:",
                                 long_options, &option_index)) != -1)
   {
      switch(opt_char)
      {
      case 'r':
         communication_range_ = atof(optarg);
         break;

      case 'n':
         num_agents_ = atoi(optarg);
         break;

      case 'a':
         arena_size_ = atof(optarg);
         break;

      case 's':
         speed_ = atof(optarg);
         break;

      case 'S':
         seed_ = atoi(optarg);

         break;

      case 'T':
         max_time_ = atoi(optarg);
         break;

      case 'R':
         if(std::string(optarg) == "majority")
         {
            // nothing to be done
            // Default is the majority rule
         }
         else
         {
            // TODO: Implement additional rules
            std::ostringstream message;
            message << "invalid LCA rule (" << std::string(optarg) << ")";
            throw std::invalid_argument(message.str());
         }
         break;

      case 'c':
         movement_rule_ = std::make_shared<CorrelatedRandomWalk>(atof(optarg));
         break;

      case ':':
         std::ostringstream message;
         message << "option " << long_options[option_index].name << "requires an argument";
         throw std::invalid_argument(message.str());
         break;

      case '?':
         std::ostringstream message;
         message << "unrecognized option";
         throw std::invalid_argument(message.str());
         break;
      }
   }

   if(by_position != 0)
   {
      init_ = ByPosition;
   }

   if(seed_ != -1)
   {
      // then don't use an explicit seed
      random_engine_.seed(seed_);
   }
}

std::unique_ptr<LCA> LCAFactory::Create(double initial_density)
{
   // lock so multiple threads can produce new LCAs at once
   std::lock_guard<std::mutex> lock(new_lca_mutex_);

   std::unique_ptr<Model> model = std::make_unique<Model>
      (arena_size_,
       num_agents_,
       communication_range_,
       seed_distribution_(random_engine_),
       initial_density,
       speed_);

   if(init_ == ByPosition)
   {
      model->SetPositionalState(initial_density);
   }

   return std::make_unique<LCA>(model, max_time_);
}
