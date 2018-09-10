#include <gtest/gtest.h>

#include <random>

#include "Agent.hpp"
#include "Point.hpp"

class AgentTest : public ::testing::Test
{
public:
   Agent agent;
   Agent agent_pos_x;
   Agent agent_neg_x;
   Agent agent_pos_y;
   Agent agent_neg_y;

   Heading agent_pos_x_heading;
   Heading agent_pos_y_heading;
   Heading agent_neg_x_heading;
   Heading agent_neg_y_heading;

   AgentTest() :
      agent(Point(0,0), Heading(0), 1, 10, 1),
      agent_pos_x(Point(0,0), Heading(0), 1, 10, 2),
      agent_neg_x(Point(0,0), Heading(M_PI), 1, 10, 3),
      agent_pos_y(Point(0,0), Heading(M_PI_2), 1, 10, 4),
      agent_neg_y(Point(0,0), Heading(-M_PI_2), 1, 10, 5)
      {
         agent_pos_x_heading = agent_pos_x.GetHeading();
         agent_pos_y_heading = agent_pos_y.GetHeading();
         agent_neg_x_heading = agent_neg_x.GetHeading();
         agent_neg_y_heading = agent_neg_y.GetHeading();
      }
};

TEST_F(AgentTest, getPosition)
{
   Point p(0,0);
   EXPECT_EQ(p, agent.Position());
}

TEST_F(AgentTest, stepNoBounce)
{
   agent_pos_x.Step();
   agent_neg_x.Step();
   agent_pos_y.Step();
   agent_neg_y.Step();

   EXPECT_TRUE(agent_pos_x.Position().Within(0.0000001, Point(1,0))) << agent_pos_x.Position();
   EXPECT_TRUE(agent_neg_x.Position().Within(0.0000001, Point(-1,0)));
   EXPECT_TRUE(agent_pos_y.Position().Within(0.0000001, Point(0,1)));
   EXPECT_TRUE(agent_neg_y.Position().Within(0.0000001, Point(0,-1)));
}

TEST_F(AgentTest, bouncePerpendicularPosition)
{
   for(int i=0; i < 5; i++)
   {
      agent_pos_x.Step();
      agent_neg_x.Step();
      agent_pos_y.Step();
      agent_neg_y.Step();
   }
   EXPECT_TRUE(agent_pos_x.Position().Within(0.0000001, Point(5,0)));
   EXPECT_TRUE(agent_neg_x.Position().Within(0.0000001, Point(-5,0)));
   EXPECT_TRUE(agent_pos_y.Position().Within(0.0000001, Point(0,5)));
   EXPECT_TRUE(agent_neg_y.Position().Within(0.0000001, Point(0,-5)));

   agent_pos_x.Step();
   agent_neg_x.Step();
   agent_pos_y.Step();
   agent_neg_y.Step();
   EXPECT_TRUE(agent_pos_x.Position().Within(0.0000001, Point(4,0))) << "Agent is at: " << agent_pos_x.Position();
   EXPECT_TRUE(agent_neg_x.Position().Within(0.0000001, Point(-4,0))) << "Agent is at: " << agent_neg_x.Position();
   EXPECT_TRUE(agent_pos_y.Position().Within(0.0000001, Point(0,4))) << "Agent is at: " << agent_pos_y.Position();
   EXPECT_TRUE(agent_neg_y.Position().Within(0.0000001, Point(0,-4))) << "Agent is at: " << agent_neg_y.Position();

   Agent a(Point(5,0), Heading(0), 2, 10, 7);
   a.Step();
   EXPECT_TRUE(a.Position().Within(0.0000001, Point(3, 0)));

   Agent a_slow(Point(5,0), Heading(0), 0.5, 10, 8);
   a_slow.Step();
   EXPECT_TRUE(a_slow.Position().Within(0.0000001, Point(4.5, 0)));

}

TEST_F(AgentTest, bouncePerpendicularHeading)
{
   for(int i=0; i < 5; i++)
   {
      agent_pos_x.Step();
      agent_neg_x.Step();
      agent_pos_y.Step();
      agent_neg_y.Step();
   }
   agent_pos_x.Step();
   agent_neg_x.Step();
   agent_pos_y.Step();
   agent_neg_y.Step();

   EXPECT_EQ(agent_pos_x.GetHeading(), agent_pos_x_heading + Heading(M_PI));
   EXPECT_EQ(agent_neg_x.GetHeading(), agent_neg_x_heading + Heading(M_PI));
   EXPECT_EQ(agent_pos_y.GetHeading(), agent_pos_y_heading + Heading(M_PI));
   EXPECT_EQ(agent_neg_y.GetHeading(), agent_neg_y_heading + Heading(M_PI));
}

TEST_F(AgentTest, bounceNonPerpendicularPosition)
{
   Agent a_top_left(Point(0,4.5), Heading(3*M_PI/4), 1, 10, 1);
   Point expected_position_a_top_left(0 - sin(3*M_PI/4), 5 - (cos(M_PI/4) - 0.5));
   a_top_left.Step();
   EXPECT_TRUE(a_top_left.Position().Within(0.0000001, expected_position_a_top_left));

   Agent a_top_right(Point(0,4.5), Heading(M_PI/4), 1, 10, 2);
   Point expected_position_a_top_right(0 + sin(M_PI/4), 5 - (cos(M_PI/4) - 0.5));
   a_top_right.Step();
   EXPECT_TRUE(a_top_right.Position().Within(0.0000001, expected_position_a_top_right));

   Agent a_bottom_left(Point(0, -4.5), Heading(-3*M_PI/4), 1, 10, 3);
   Point expected_position_a_bottom_left(0 - sin(3*M_PI/4), -(5 - (cos(M_PI/4) - 0.5)));
   a_bottom_left.Step();
   EXPECT_TRUE(a_bottom_left.Position().Within(0.0000001, expected_position_a_bottom_left));

   Agent a_bottom_right(Point(0, -4.5), Heading(-M_PI/4), 1, 10, 4);
   Point expected_position_a_bottom_right(0 + sin(M_PI/4), -(5 - (cos(M_PI/4) - 0.5)));
   a_bottom_right.Step();
   EXPECT_TRUE(a_bottom_right.Position().Within(0.0000001, expected_position_a_bottom_right));

   Agent a_left_up(Point(-4.5, 0), Heading(3*M_PI/4), 1, 10, 5);
   Point expected_position_a_left_up(-(5 - (cos(M_PI/4) - 0.5)), 0 + sin(M_PI/4));
   a_left_up.Step();
   EXPECT_TRUE(a_left_up.Position().Within(0.0000001, expected_position_a_left_up));

   Agent a_left_down(Point(-4.5, 0), Heading(-3*M_PI/4), 1, 10, 6);
   Point expected_position_a_left_down(-(5 - (cos(M_PI/4) - 0.5)), 0 - sin(M_PI/4));
   a_left_down.Step();
   EXPECT_TRUE(a_left_down.Position().Within(0.0000001, expected_position_a_left_down));

   Agent a_right_up(Point(4.5, 0), Heading(M_PI/4), 1, 10, 7);
   Point expected_position_a_right_up(5 - (cos(M_PI/4) - 0.5), 0 + sin(M_PI/4));
   a_right_up.Step();
   EXPECT_TRUE(a_right_up.Position().Within(0.0000001, expected_position_a_right_up));

   Agent a_right_down(Point(4.5, 0), Heading(-M_PI/4), 1, 10, 8);
   Point expected_position_a_right_down(5 - (cos(M_PI/4) - 0.5), 0 - sin(M_PI/4));
}

TEST_F(AgentTest, bounceNonPerpendicularHeading)
{
   Agent a_top_left(Point(0,4.5), Heading(3*M_PI/4), 1, 10, 9);
   a_top_left.Step();
   EXPECT_EQ(a_top_left.GetHeading(), Heading(-3*M_PI/4));

   Agent a_top_right(Point(0, 4.5), Heading(M_PI/4), 1, 10, 10);
   a_top_right.Step();
   EXPECT_EQ(a_top_right.GetHeading(), Heading(-M_PI/4));

   Agent a_bottom_right(Point(0, -4.5), Heading(-M_PI/4), 1, 10, 11);
   a_bottom_right.Step();
   EXPECT_EQ(a_bottom_right.GetHeading(), Heading(M_PI/4));

   Agent a_bottom_left(Point(0, -4.5), Heading(-3*M_PI/4), 1, 10, 12);
   a_bottom_left.Step();
   EXPECT_EQ(a_bottom_left.GetHeading(), Heading(3*M_PI/4));

   Agent a_right_up(Point(4.5, 0), Heading(M_PI/4), 1, 10, 13);
   a_right_up.Step();
   EXPECT_EQ(a_right_up.GetHeading(), Heading(3*M_PI/4));

   Agent a_right_down(Point(4.5, 0), Heading(-M_PI/4), 1, 10, 14);
   a_right_down.Step();
   EXPECT_EQ(a_right_down.GetHeading(), Heading(-3*M_PI/4));
}

TEST_F(AgentTest, setHeading)
{
   ASSERT_NE(agent.GetHeading(), Heading(1.0));
   agent.SetHeading(Heading(1.0));
   ASSERT_EQ(agent.GetHeading(), Heading(1.0));
}

TEST_F(AgentTest, zeroVelocityNoMovement)
{
   Agent a(Point(0,0), Heading(0), 0, 10, 15);
   agent.Step();
   EXPECT_EQ(a.Position(), Point(0,0));
   agent.Step();
   agent.Step();
   EXPECT_EQ(a.Position(), Point(0,0));
}