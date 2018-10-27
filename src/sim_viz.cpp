#include <SFML/Graphics.hpp>

#include <sstream>
#include <string>

#include "Model.hpp"
#include "Rule.hpp"

#include <getopt.h>

int main(int argc, char** argv)
{
   double communication_range = 5;
   int    num_agents          = 100;
   double arena_size          = 100;
   int    seed                = 1234;
   double initial_density     = 0.5;
   Rule*  rule                = majority_rule;
   std::shared_ptr<MovementRule> movement_rule = std::make_shared<RandomWalk>();

   unsigned int frameRate = 4;
   int by_position = 0;

   static struct option long_options[] =
      {
         {"initial-density",     required_argument, 0,            'd'},
         {"communication-range", required_argument, 0,            'r'},
         {"num-agents",          required_argument, 0,            'n'},
         {"arena-size",          required_argument, 0,            'a'},
         {"seed",                required_argument, 0,            's'},
         {"mu",                  required_argument, 0,            'm'},
         {"rule",                required_argument, 0,            'R'},
         {"frame-rate",          required_argument, 0,            'f'},
         {"correlated",          required_argument, 0,            'c'},
         {"by-position",         no_argument,       &by_position, 'p'},
         {0,0,0,0}
      };

   int option_index = 0;
   int opt_char;
   while((opt_char = getopt_long(argc, argv, "m:d:r:n:a:s:i:R:c:",
                                 long_options, &option_index)) != -1)
   {
      switch(opt_char)
      {
      case 'd':
         initial_density = atof(optarg);
         break;

      case 'r':
         communication_range = atof(optarg);
         break;

      case 'n':
         num_agents = atoi(optarg);
         break;

      case 'a':
         arena_size = atof(optarg);
         break;

      case 's':
         seed = atoi(optarg);
         break;

      case 'f':
         frameRate = atoi(optarg);
         break;

      case 'R':
         if(std::string(optarg) == "gkl")
         {
            std::cout << "gkl-strict" << std::endl;
            rule = gkl2d_strict;
         }
         else if(std::string(optarg) == "gkl-lax")
         {
            rule = gkl2d_lax;
         }
         else if(std::string(optarg) == "majority")
         {
            rule = majority_rule;
         }
         else if(std::string(optarg) == "gkl-mode")
         {
            rule = gkl2d_mode;
         }
         else
         {
            std::cout << "invalid rule (" << std::string(optarg) << ")" << std::endl;
            exit(-1);
         }
         break;

      case 'c':
         movement_rule = std::make_shared<CorrelatedRandomWalk>(atof(optarg));
         break;

      case ':':
         std::cout << "option " << long_options[option_index].name << "requires an argument" << std::endl;
         exit(-1);
         break;

      case '?':
         std::cout << "unrecognized option" << std::endl;
         exit(-1);
      }
   }

   if(optind >= argc)
   {
      std::cout << "missing required argument <agent-speed>" << std::endl;
   }
   double speed = atof(argv[optind]);
   
   sf::RenderWindow window(sf::VideoMode(600,600), "Motion-CA");
   window.setFramerateLimit(frameRate);

   sf::View centeredView;
   centeredView.setCenter(0,0);
   centeredView.setSize(arena_size+4,arena_size+4);
   window.setView(centeredView);

   Model m(arena_size, num_agents, communication_range, seed, initial_density, speed);
   if(by_position != 0)
   {
      m.SetPositionalState(initial_density);
   }
   m.SetMovementRule(movement_rule);
   std::cout << "initial majority: " << m.CurrentDensity();
   std::cout << " (" << (m.CurrentDensity() > 0.5 ? "white" : "black") << ")" << std::endl;
   int i = 0;
   bool done = false;
   while(window.isOpen())
   {
      sf::Event event;
      while(window.pollEvent(event))
      {
         if(event.type == sf::Event::Closed)
         {
            window.close();
         }
      }

      window.clear(sf::Color::White);
      auto agents  = m.GetAgents();
      auto states  = m.GetStates();
      auto network = m.CurrentNetwork();
      if(!done && (m.CurrentDensity() == 0 || m.CurrentDensity() == 1))
      {
         done = true;
         std::cout << "converged at: " << i << std::endl;
      }

      for(int i = 0; i < agents.size(); i++)
      {
         for(auto a : network->GetNeighbors(i))
         {
            sf::VertexArray lines(sf::Lines, 2);
            lines[0].position = sf::Vector2f(agents[i].Position().GetX()-0.5, agents[i].Position().GetY()-0.5);
            lines[0].color    = sf::Color(0,0,0,128);
            lines[1].position = sf::Vector2f(agents[a].Position().GetX()-0.5, agents[a].Position().GetY()-0.5);
            lines[1].color    = sf::Color(0,0,0,128);
            window.draw(lines);
         }
      }

      for(int i = 0; i < agents.size(); i++)
      {
         sf::CircleShape agent_shape(0.8);
         agent_shape.setOutlineColor(sf::Color::Black);
         agent_shape.setOutlineThickness(0.18);
         if(states[i] == 1)
         {
            agent_shape.setFillColor(sf::Color::White);
         }
         else
         {
            agent_shape.setFillColor(sf::Color::Black);
         }
         agent_shape.setOrigin(1,1);
         agent_shape.setPosition(agents[i].Position().GetX(), agents[i].Position().GetY());
         window.draw(agent_shape);
      }
      window.display();
      if((i % 10) == 0) {
         auto frame = window.capture();
         std::stringstream fname;
         fname << "frame"<<i<<".png";
         frame.saveToFile(fname.str());
      }
      m.Step(rule);
      i++;
   }
}
