/*

		NAME 	 - Control Node 
		DESIGN - WhiteGlint - Scott Caldwell
		PRIORITY - Required for the Moment - Required for directing the robot in the arena
	
		--------OVERVIEW--------
This node is a pseudo state machine that increments state to alternate control between the high level and the arduino.
The node will store preset destinations for the robot to move about the map. The map will have to be premade and the
destinations will have to be crafted to the map.

		--------FUTURE WORK--------
TODO

*/

#include <string>
#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <tf/transform_broadcaster.h>
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "geometry_msgs/Quaternion.h"
#include "GCRobotics/command_state.h"
#include <ros/console.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

void command_state_callback(const GCRobotics::command_state::ConstPtr& msg);
void move_to_goal(GCRobotics::command_state current_state);
float to_rads(float degrees);

const float PI = 3.14159265359;
    
boost::shared_ptr<ros::Publisher> command_state_pub;
boost::shared_ptr<ros::Publisher> initial_pose_pub;
boost::shared_ptr<ros::Subscriber> command_state_sub;

int main(int argc, char **argv)
{
    ros::init(argc, argv, "command_node");
    ros::NodeHandle n;
    ros::Rate r(5);
        
    command_state_pub = boost::make_shared<ros::Publisher>(n.advertise<GCRobotics::command_state>("/command_state", 100));
    initial_pose_pub = boost::make_shared<ros::Publisher>(n.advertise<geometry_msgs::PoseWithCovarianceStamped>("/initialpose", 100));
    command_state_sub = boost::make_shared<ros::Subscriber>(n.subscribe("command_state", 100, command_state_callback));

    geometry_msgs::PoseWithCovarianceStamped initial_pose;
    initial_pose.header.stamp = ros::Time::now();
    
    initial_pose.pose.pose.position.x = 60;
    initial_pose.pose.pose.position.y = 60;
    initial_pose.pose.pose.position.z = 0;

    geometry_msgs::Quaternion initial_quaternion = tf::createQuaternionMsgFromRollPitchYaw(0, 0, to_rads(270));
    initial_pose.pose.pose.orientation = initial_quaternion;
    
    (*initial_pose_pub).publish(initial_pose);
    
    GCRobotics::command_state initial_state;
    initial_state.state = 0;
    initial_state.mask_odom = true;
    
    (*command_state_pub).publish(initial_state);
	
	ros::spin();
}

void command_state_callback(const GCRobotics::command_state::ConstPtr& msg)
{
    GCRobotics::command_state current_state;
    int state = msg->state;
    
    if (state % 2)
    {
        current_state.state = state;
        current_state.mask_odom = false;
        (*command_state_pub).publish(current_state);
        
        move_to_goal(current_state);
        
        current_state.state++;
        current_state.mask_odom = true;
        (*command_state_pub).publish(current_state);
    }
    else
    {
        char* info_message;

        switch (state)
        {
        case 0:
            info_message = "Waiting for start signal and initial alignment.";
            break;
        case 2:
            info_message = "Waiting for fridge alignment.";
            break;
        case 4: // back up from fridge
            info_message = "Waiting for food retrieval.";
            break;
        case 6: // move to table
            info_message = "Waiting for fridge alignment.";
            break;
        case 8: // move closer to table
            info_message = "Waiting for table alignment.";
            break;
        case 10: // back up from table
            info_message = "Waiting for food deposit.";
            break;
        case 12: // go home, you're drunk
            info_message = "Waiting for table alignment.";
            break;
        default:
            info_message = "Indeterminate state.";
        }
        
        ROS_INFO("%s", info_message);
    }
}

void move_to_goal(GCRobotics::command_state current_state)
{
    
    // tell the action client that we want to spin a thread by default
    MoveBaseClient ac("move_base", true);

    int state = current_state.state;
    char* info_message;
    geometry_msgs::Quaternion rotation_quaternion;
    move_base_msgs::MoveBaseGoal goal;
    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();
    
    switch (state)
    {
    case 1: // move to fridge
        goal.target_pose.pose.position.x = 169; // 20 cm from fridge
        goal.target_pose.pose.position.y = 62; // center of fridge
        rotation_quaternion = tf::createQuaternionMsgFromRollPitchYaw(0, 0, to_rads(270));
        goal.target_pose.pose.orientation = rotation_quaternion;
        info_message = "to the fridge";
        break;
    case 3: // move closer to fridge
        goal.target_pose.pose.position.x = 184; // 5 cm from fridge
        goal.target_pose.pose.position.y = 62; // center of fridge
        rotation_quaternion = tf::createQuaternionMsgFromRollPitchYaw(0, 0, to_rads(270));
        goal.target_pose.pose.orientation = rotation_quaternion;
        info_message = "closer to the fridge";
        break;
    case 5: // back up from fridge
        goal.target_pose.pose.position.x = 169; // 20 cm from fridge
        goal.target_pose.pose.position.y = 62; // center of fridge
        rotation_quaternion = tf::createQuaternionMsgFromRollPitchYaw(0, 0, to_rads(270));
        goal.target_pose.pose.orientation = rotation_quaternion;
        info_message = "away from the fridge";
        break;
    case 7: // move to table
        goal.target_pose.pose.position.x = 84; // 20 cm from table
        goal.target_pose.pose.position.y = 180; // center of table
        rotation_quaternion = tf::createQuaternionMsgFromRollPitchYaw(0, 0, to_rads(90));
        goal.target_pose.pose.orientation = rotation_quaternion;
        info_message = "to the table";
        break;
    case 9: // move closer to table
        goal.target_pose.pose.position.x = 69; // 5 cm from table
        goal.target_pose.pose.position.y = 180; // center of table
        rotation_quaternion = tf::createQuaternionMsgFromRollPitchYaw(0, 0, to_rads(90));
        goal.target_pose.pose.orientation = rotation_quaternion;
        info_message = "closer to the table";
        break;
    case 11: // back up from table
        goal.target_pose.pose.position.x = 84; // 20 cm from table
        goal.target_pose.pose.position.y = 180; // center of table
        rotation_quaternion = tf::createQuaternionMsgFromRollPitchYaw(0, 0, to_rads(180));
        goal.target_pose.pose.orientation = rotation_quaternion;
        info_message = "away from the table";
        break;
    case 13: // go home, you're drunk
        goal.target_pose.pose.position.x = 35; // 20 cm from wall
        goal.target_pose.pose.position.y = 35; // 20 cm from wall
        rotation_quaternion = tf::createQuaternionMsgFromRollPitchYaw(0, 0, to_rads(270));
        goal.target_pose.pose.orientation = rotation_quaternion;
        info_message = "home";
        break;
    }
    
    ROS_INFO("Attempting to move %s", info_message);

    ac.sendGoal(goal);

    ac.waitForResult();
}

float to_rads(float degrees)
{
    return degrees * PI / 180;
}






























