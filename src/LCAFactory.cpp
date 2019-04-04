#include "LCAFactory.hpp"

#include <getopt.h>
#include <limits>
#include <sstream>

LCAFactory::LCAFactory() :
   num_agents_(255),
   communication_range_(5),
   arena_size_(100),
   speed_(1),
   seed_(-1),
   max_time_(5000),
   init_(Uniform)
{
   rule_ = std::make_unique<Identity>();
   movement_rule_ = std::make_shared<RandomWalk>();
   seed_distribution_ = std::uniform_int_distribution<int>(0, std::numeric_limits<int>::max());
}

void LCAFactory::ParseRule(std::string rule_spec)
{
   if(rule_spec == "majority")
   {
      rule_ = std::make_shared<MajorityRule>();
   }
   else if(rule_spec == "majority-noswitch")
   {
      rule_ = std::make_shared<MajorityRule>(false);
   }
   else
   {
      std::stringstream message;
      // TODO: Implement additional rules
      message << "invalid LCA rule (" << std::string(optarg) << ")";
      throw std::invalid_argument(message.str());
   }

}

int LCAFactory::Init(int argc, char** argv)
{
   int by_position = 0;

   static struct option long_options[] =
      {
         {"communication-range", required_argument, 0,            'r'},
         {"num-agents",          required_argument, 0,            'n'},
         {"arena-size",          required_argument, 0,            'a'},
         {"speed",               required_argument, 0,            's'},
         {"seed",                required_argument, 0,            'S'},
         {"majority",            required_argument, 0,            'm'},
         {"quorum",              required_argument, 0,            'q'},
         {"correlated",          required_argument, 0,            'c'},
         {"max-time",            required_argument, 0,            'T'},
         {"by-position",         no_argument,       &by_position, 'p'},
         {0,0,0,0}
      };
   int option_index = 0;
   char opt_char;
   while((opt_char = getopt_long(argc, argv, "r:n:a:s:S:i:c:R:T:m:q:",
                                 long_options, &option_index)) != -1)
   {
      std::stringstream message;
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

      case 'm':
         if(std::string(optarg) == "true")
         {
            rule_ = std::make_shared<MajorityRule>(true);
         }
         else
         {
            rule_ = std::make_shared<MajorityRule>(false);
         }
         break;

      case 'q':
         rule_ = std::make_shared<Quorum>(atof(optarg));
         break;

      case 'c':
         movement_rule_ = std::make_shared<CorrelatedRandomWalk>(atof(optarg));
         break;

      case ':':
         message << "option " << long_options[option_index].name << "requires an argument";
         throw std::invalid_argument(message.str());
         break;

      case '?':
         throw std::invalid_argument("unrecognized option");
         break;
      }
   }

   if(by_position != 0)
   {
      init_ = ByPosition;
   }

   if(seed_ != -1)
   {
      random_engine_.seed(seed_);
   }
   else
   {
      std::random_device rd;
      random_engine_.seed(rd());
   }

   return optind;
}

std::unique_ptr<LCA> LCAFactory::Create(double initial_density)
{
   // lock so multiple threads can produce new LCAs at once
   std::lock_guard<std::mutex> lock(new_lca_mutex_);

   int seed = seed_distribution_(random_engine_);

   Model model(arena_size_,
               num_agents_,
               communication_range_,
               seed,
               initial_density,
               speed_);

   if(init_ == ByPosition)
   {
      model.SetPositionalState(initial_density);
   }

   return std::make_unique<LCA>(model, rule_, max_time_);
}

double LCAFactory::ArenaSize() const
{
   return arena_size_;
}
