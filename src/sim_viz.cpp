#include <SFML/Graphics.hpp>

#include <sstream>
#include <string>

#include "Model.hpp"
#include "Rule.hpp"
#include "LCAFactory.hpp"
#include "Heading.hpp"

#include <getopt.h>

sf::Color headingToColor(const Heading& heading, int state)
{
   double r, g, b;
   double h = heading.Radians() / (2.0*M_PI);
   double h_prime = h * 6;
   double v = 0.6;
   double s = 1.0 / (state+1);
   double c = v * s;
   double x = c * (1 - fabs(remainder(h_prime, 2.0) - 1));

   if(h_prime <= 1) {
      r = c;
      g = x;
      b = 0;
   }
   else if(h_prime <= 2) {
      r = x;
      g = c;
      b = 0;
   }
   else if(h_prime <= 3) {
      r = 0;
      g = c;
      b = x;
   }
   else if(h_prime <= 4) {
      r = 0;
      g = x;
      b = c;
   }
   else if(h_prime <= 5) {
      r = x;
      g = 0;
      b = c;
   }
   else if(h_prime <= 6) {
      r = c;
      g = 0;
      b = x;
   }

   return sf::Color(r*255,g*255,b*255);
}

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
               /*if(frameRate < 120)*/ frameRate+=10;
               std::cout << "frameRate: " << frameRate << std::endl;
               window.setFramerateLimit(frameRate);
            }
            else if(event.key.code == sf::Keyboard::Down)
            {
               if(frameRate > 10) frameRate-=10;
               std::cout << "frameRate: " << frameRate << std::endl;
               window.setFramerateLimit(frameRate);
            }
            else if(event.key.code == sf::Keyboard::Key::Q)
            {
               return 0;
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
      const std::vector<Agent>& agents = lca->GetAgents();
      const std::vector<int>& states  = lca->GetStates();
      auto network = lca->CurrentNetwork();
      for(int i = 0; i < agents.size(); i++)
      {
         Point agent_i_pos = agents[i].Position();
         for(auto& a : network->GetNeighbors(i))
         {
            Point agent_a_pos = agents[a].Position();
            sf::VertexArray lines(sf::Lines, 2);
            lines[0].position = sf::Vector2f(agent_i_pos.GetX()-0.5, agent_i_pos.GetY()-0.5);
            lines[0].color    = sf::Color(0,0,0,64);
            lines[1].position = sf::Vector2f(agent_a_pos.GetX()-0.5, agent_a_pos.GetY()-0.5);
            lines[1].color    = sf::Color(0,0,0,64);
            window.draw(lines);
         }
      }

      for(int i = 0; i < agents.size(); i++)
      {
         sf::CircleShape agent_shape(0.6);
         // TODO: Add black/white option
         // agent_shape.setOutlineColor(sf::Color::Black);
         // agent_shape.setOutlineThickness(0.18);
         // if(states[i] == 1)
         // {
         //    agent_shape.setFillColor(sf::Color::White);
         // }
         // else
         // {
         //    agent_shape.setFillColor(sf::Color::Black);
         // }
         sf::Color agent_color = sf::Color::Black; // headingToColor(agents[i].GetHeading(), states[i]);
         if(agents[i].IsDark())
            agent_color = sf::Color(120, 120, 120);
         agent_shape.setOutlineColor(agent_color);
         agent_shape.setOutlineThickness(0.4);
         if(states[i] == 1)
         {
            agent_shape.setFillColor(sf::Color::White);
         }
         else
         {
            agent_shape.setFillColor(agent_color);
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
