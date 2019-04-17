#include <SFML/Graphics.hpp>

#include <sstream>
#include <string>

#include "Model.hpp"
#include "Rule.hpp"
#include "LCAFactory.hpp"

#include <getopt.h>

int main(int argc, char** argv)
{
   LCAFactory factory;
   int arg_index = factory.Init(argc, argv);
   double initial_density = 0.5;
   if(arg_index < argc)
   {
      initial_density = atof(argv[arg_index]);
   }

   std::unique_ptr<LCA> lca = factory.Create(initial_density);
   lca->MinimizeMemory();

   sf::RenderWindow window(sf::VideoMode(600,600), "Liquid-CA");

   int frameRate = 10;
   window.setFramerateLimit(frameRate);

   sf::View centeredView;
   centeredView.setCenter(0,0);
   centeredView.setSize(factory.ArenaSize()+4,factory.ArenaSize()+4);
   window.setView(centeredView);

   std::cout << "initial majority: " << lca->CurrentDensity();
   std::cout << " (" << (lca->CurrentDensity() > 0.5 ? "white" : "black") << ")" << std::endl;
   int i = 0;
   bool done = false;
   bool run  = false;
   while(window.isOpen())
   {
      sf::Event event;
      while(window.pollEvent(event))
      {
         switch(event.type)
         {
         case sf::Event::Closed:
            window.close();
            break;

         case sf::Event::KeyPressed:
            if(event.key.code == sf::Keyboard::Space)
            {
               run = !run;
            }
            else if(event.key.code == sf::Keyboard::Up)
            {
               if(frameRate < 60) frameRate+=10;
               std::cout << "frameRate: " << frameRate << std::endl;
               window.setFramerateLimit(frameRate);
            }
            else if(event.key.code == sf::Keyboard::Down)
            {
               if(frameRate > 10) frameRate-=10;
               std::cout << "frameRate: " << frameRate << std::endl;
               window.setFramerateLimit(frameRate);
            }

            break;
         }
      }

      if(!done && (lca->CurrentDensity() == 0.0 || lca->CurrentDensity() == 1.0))
      {
         done = true;
         std::cout << "converged at " << i << std::endl;
      }

      window.clear(sf::Color::White);
      auto agents  = lca->GetAgents();
      auto states  = lca->GetStates();
      auto network = lca->CurrentNetwork();
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
      // if((i % 10) == 0) {
      //    auto frame = window.capture();
      //    std::stringstream fname;
      //    fname << "frame"<<i<<".png";
      //    frame.saveToFile(fname.str());
      // }
      if(run) {
         lca->Run(1);
         i++;
      }
   }
}
