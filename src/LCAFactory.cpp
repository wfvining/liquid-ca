#include "LCAFactory.hpp"

#include <getopt.h>
#include <limits>
#include <sstream>
#include <streambuf>
#include <fstream>

#include "TotalisticRule.hpp"

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
         {"correlated",          required_argument, 0,            'c'},
         {"max-time",            required_argument, 0,            'T'},
         {"by-position",         no_argument,       &by_position, 'p'},
         {"rule",                required_argument, 0,            'R'},
         {"pdark",               required_argument, 0,            'd'},
         {"pinteractive",        required_argument, 0,            'i'},
         {0,0,0,0}
      };
   int option_index = 0;
   char opt_char;
   std::ifstream file;
   TotalisticRule r;
   while((opt_char = getopt_long(argc, argv, "r:n:a:s:S:c:R:T:",
                                 long_options, &option_index)) != -1)
   {
      std::stringstream message;
      switch(opt_char)
      {
      case 'd':
         pdark_ = atof(optarg);
         break;

      case 'i':
         pinteractive_ = atof(optarg);
         break;

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

      case 'c':
         movement_rule_ = std::make_shared<CorrelatedRandomWalk>(atof(optarg));
         break;

      case 'R':
         file = std::ifstream(std::string(optarg));
         file >> r;

         rule_ = std::make_shared<TotalisticRule>(r);

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
   model.SetMovementRule(movement_rule_);
   model.SetPDark(pdark_);
   model.SetPInteractive(pinteractive_);

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
