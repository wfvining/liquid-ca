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

   Model m(67, 255, 8, atoi(argv[1]), 0.5, 0.25);
   m.SetMovementRule(RandomWalk());
   
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
      auto agents = m.GetAgents();
      auto states = m.GetStates();
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
      m.Step(majority_rule);
   }
}