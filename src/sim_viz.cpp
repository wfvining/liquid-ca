#include <SFML/Graphics.hpp>

#include "Model.hpp"
#include "Rule.hpp"

int main(int argc, char** argv)
{
   unsigned int frameRate = 4;
   if(argc > 2)
   {
      frameRate = atoi(argv[2]);
   }
   
   sf::RenderWindow window(sf::VideoMode(600,600), "Motion-CA");
   window.setFramerateLimit(frameRate);

   sf::View centeredView;
   centeredView.setCenter(0,0);
   centeredView.setSize(70,70);
   window.setView(centeredView);

   Model m(67, 2, 4, atoi(argv[1]), 0.45, 2);
   m.SetMovementRule(LevyWalk(2.0, 67));
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
      for(int i = 0; i < agents.size(); i++)
      {
         for(auto a : network->GetNeighbors(i))
         {
            sf::VertexArray lines(sf::Lines, 2);
            lines[0].position = sf::Vector2f(agents[i].Position().GetX()-0.5, agents[i].Position().GetY()-0.5);
            lines[0].color    = sf::Color(0,0,0,32);
            lines[1].position = sf::Vector2f(agents[a].Position().GetX()-0.5, agents[a].Position().GetY()-0.5);
            lines[1].color    = sf::Color(0,0,0,32);
            window.draw(lines);
         }
      }

      for(int i = 0; i < agents.size(); i++)
      {
         sf::CircleShape agent_shape(0.5);
         agent_shape.setOutlineColor(sf::Color::Black);
         agent_shape.setOutlineThickness(0.1);
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
      m.Step(gkl2d);
   }
}