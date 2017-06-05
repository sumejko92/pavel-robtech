#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include <iostream>
#include "keyboard_reader/keyboard_reader.h"
#include "keyboard_reader/Key.h" //msg folder powermateevent.msg
#include "std_msgs/Bool.h" 

ros::Publisher mv_pub;
ros::Subscriber sub;

geometry_msgs::Twist command_move;
int cnt = 0;

//griffin_powernmate(package)::PowermateEvent(msgs)
void key_dirCallback(const keyboard_reader::Key& msg)
{
//is_pressed
  if(msg.key_pressed == 1)
   {

	  cnt = cnt + 1; 
	  ROS_INFO("CNT: [%i]", cnt); 

	 if (cnt % 2 != 0)
	  {
	   ROS_INFO("Moving");
	   command_move.linear.x = 2;
	  }
	   else if (cnt % 2 == 0)
	  {
	   ROS_INFO("Stopping");
	   command_move.linear.x = 0;

	  }
  }

}


int main(int argc, char **argv)
{
  //Initializes ROS, and sets up a node
  ros::init(argc, argv, "move_bot");

  ros::NodeHandle n;
  ///griffin_powermate/events mesto keyboard
  sub = n.subscribe("keyboard", 100, key_dirCallback);

  mv_pub = n.advertise<geometry_msgs::Twist>("/husky_velocity_controller/cmd_vel",1000);

  command_move.linear.x = 0;
  command_move.linear.y = 0;
  command_move.linear.z = 0;
  command_move.angular.x = 0;
  command_move.angular.y = 0;
  command_move.angular.z = 0;

  ros::Rate rate_loop(10);

  while(ros::ok()) 
  {
  //  ros::Rate rate(10);
    mv_pub.publish(command_move);

    ros::spinOnce();
    rate_loop.sleep();
  }


  return 0;
}
