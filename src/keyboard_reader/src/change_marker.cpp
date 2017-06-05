#include "ros/ros.h"
#include <iostream>
#include "keyboard_reader/keyboard_reader.h"
#include "keyboard_reader/Key.h" 
#include "std_msgs/Bool.h" 
#include <visualization_msgs/Marker.h>



ros::Publisher marker_pub;
ros::Subscriber sub;

int cnt = 0;
uint32_t shape = visualization_msgs::Marker::CUBE;
visualization_msgs::Marker marker;

float cntr=1; //variable to change the marker size


void key_dirCallback(const keyboard_reader::Key& msg)
{
  
  if(msg.key_pressed == 1)
   {

    cnt = cnt + 1; 
    ROS_INFO("CNT: [%i]", cnt); 
	  
    switch (shape)
    {
    case visualization_msgs::Marker::CUBE:
      shape = visualization_msgs::Marker::SPHERE;
      ROS_INFO("SPHERE");
      break;
    case visualization_msgs::Marker::SPHERE:
      shape = visualization_msgs::Marker::ARROW;
      ROS_INFO("ARROW");
      break;
    case visualization_msgs::Marker::ARROW:
      shape = visualization_msgs::Marker::CYLINDER;
      ROS_INFO("CYLINDER");
      break;
    case visualization_msgs::Marker::CYLINDER:
      shape = visualization_msgs::Marker::CUBE;
      ROS_INFO("CUBE");
      break;
    }	  
    marker.scale.x = cntr;
    marker.scale.y = cntr;
    marker.scale.z = cntr; 
    cntr = cntr + 0.5;
    
  }

}


int main(int argc, char **argv)
{
  //Initializes ROS, and sets up a node
  ros::init(argc, argv, "basic_shapes");

  ros::NodeHandle n;

  sub = n.subscribe("keyboard", 100, key_dirCallback);

  marker_pub= n.advertise<visualization_msgs::Marker>("visualization_marker",100);


  ros::Rate rate_loop(10);

// .....................
    
    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    marker.header.frame_id = "/my_frame";
    marker.header.stamp = ros::Time::now();

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    marker.ns = "basic_shapes";
    marker.id = 0;

    // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
    marker.type = shape;

    // Set the marker action.  Options are ADD and DELETE
    marker.action = visualization_msgs::Marker::ADD;

    // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
    marker.pose.position.x = 0;
    marker.pose.position.y = 0;
    marker.pose.position.z = 0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;

    // Set the scale of the marker -- 1x1x1 here means 1m on a side
    marker.scale.x = 1.0;
    marker.scale.y = 1.0;
    marker.scale.z = 1.0;

    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 0.0f;
    marker.color.g = 1.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;


  // Set our initial shape type to be a cube
    //uint32_t shape = visualization_msgs::Marker::CUBE;
   // shape = visualization_msgs::Marker::CUBE;

  while (ros::ok())
  {
    // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
    marker.type = shape;

    // Set the marker action.  Options are ADD and DELETE
    marker.action = visualization_msgs::Marker::ADD;    
    marker.lifetime = ros::Duration();


    // Publish the marker
    marker_pub.publish(marker);

    // Cycle between different shapes

    ros::spinOnce();
    rate_loop.sleep();
  }

  return 0;
}
