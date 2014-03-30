/*

		NAME 	 - Odometry Node 
		DESIGN - Josh Galicic - David Mine - Scott Caldwell - WhiteGlint - influenced by ROS's odometry tutorial
		PRIORITY - Required - Required for deriving position from encoders
	
		--------OVERVIEW--------
This node takes encoder data from the arduino and calculates the 3D position of the bot.
This position is broadcast through tf as well as odom. The tf transform goes between
'odom', the frame that is fixed to the world and 'base_link', the frame that is attached
to the bot frame.


		--------FUTURE WORK--------
The efficiency scalars are very rough approximations that were found experimentally by driving the
bot and checking encoder counts. They can always be made more precise.

Incorporate gyroscope data. hopfully one day

Simply averaging the encoder counts is subject to erroneous encoder data (i.e. outliers).
This could be improved to detect and disregard outliers. 

A lot of the conversion factors should be setup as ros parameters so that recompiling isnt necessary for
minor changes. Checkout how to do this on the ros wiki, there are also tutorials on how to do this. 

*/

#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include "geometry_msgs/Twist.h"
#include "GCRobotics/Encoder_msg.h"

const float PI = 3.14159265359;

bool mask_odom;
float average_encoder_counts;
char direction = 'f';
geometry_msgs::Twist current_velocity;

void odometryCallback(const GCRobotics::Encoder_msg::ConstPtr& msg);
void velocityCallback(const geometry_msgs::Twist::ConstPtr& msg);
void mask_odom_callback(const GCRobotics::command_state::ConstPtr& msg);

int main(int argc, char **argv)
{

    ros::init(argc, argv, "odometryNode");

    ros::NodeHandle n;
    ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("/odom", 100);
    ros::Subscriber odom_sub = n.subscribe("EncoderData", 100, odometryCallback);
    ros::Subscriber vel_sub = n.subscribe("cmd_vel", 100, velocityCallback);
    ros::Subscriber mask_odom_sub = n.subscribe("command_state", 100, mask_odom_callback);
    tf::TransformBroadcaster odom_broadcaster;
    
    // x, y, and rotation coordinates for the odom frame
    float x = 0;
    float y = 0;
    float w = 0;
    
    // Measurements are in meters
	// "Circle" refers to the circle made by the rotation of the
	// robot about its axis of rotation (which should be dead center)
	float x_conversion = 1.0; // efficiency scalar. how much of a revolution is actually converted to linear x motion
	float y_conversion = 0.667; // efficiency scalar. how much of a revolution is actually converted to linear y motion
	float w_conversion = 0.93; // efficiency scalar. how much of a revolution is actually converted to rotational motion
	float wheel_radius = 0.02699;
	float wheel_circumference = 2 * PI * wheel_radius;
	float cpr = 546.0; // encoder counts per revolution of the wheel-changed
	float degrees_per_circle = 360.0;
	float radians_per_degree = PI / 180.0;
	float circle_circumference = 1.13; // approximate
	float degrees_per_count = w_conversion * (degrees_per_circle * wheel_circumference) / (cpr * circle_circumference);
	float radians_per_count = degrees_per_count * radians_per_degree;
	float linear_x_distance_per_count = x_conversion * wheel_circumference / cpr;
	float linear_y_distance_per_count = y_conversion * wheel_circumference / cpr;
	
	ros::Rate r(10);

	while (n.ok())
	{
	    ros::spinOnce();
	    
        ros::Time current_time = ros::Time::now();
	    
	    if (direction == 'w')
	    {
	        x += average_encoder_counts * linear_x_distance_per_count * cos(w);
	        y += average_encoder_counts * linear_x_distance_per_count * sin(w);
        }
        else if (direction == 's')
        {
            x -= average_encoder_counts * linear_x_distance_per_count * cos(w);
            y -= average_encoder_counts * linear_x_distance_per_count * sin(w);
        }
        else if (direction == 'a')
        {
            x += average_encoder_counts * linear_y_distance_per_count * cos(w + PI / 2.0);
            y += average_encoder_counts * linear_y_distance_per_count * sin(w + PI / 2.0);
        }
        else if (direction == 'd')
        {
            x -= average_encoder_counts * linear_y_distance_per_count * cos(w + PI / 2.0);
            y -= average_encoder_counts * linear_y_distance_per_count * sin(w + PI / 2.0);
        }
        else if (direction == 'q')
            w += average_encoder_counts * radians_per_count;
        else if (direction == 'e')
            w -= average_encoder_counts * radians_per_count;
            
        geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(w);
        
        geometry_msgs::TransformStamped odom_trans;
        odom_trans.header.stamp = current_time;
        odom_trans.header.frame_id = "/odom";
        odom_trans.child_frame_id = "base_link";
        
        odom_trans.transform.translation.x = x;
        odom_trans.transform.translation.y = y;
        odom_trans.transform.translation.z = 0.0;
        odom_trans.transform.rotation = odom_quat;
        
        odom_broadcaster.sendTransform(odom_trans);
        
        nav_msgs::Odometry odom;
        odom.header.stamp = current_time;
        odom.header.frame_id = "/odom";
        
        odom.pose.pose.position.x = x;
        odom.pose.pose.position.y = y;
        odom.pose.pose.position.z = 0.0;
        odom.pose.pose.orientation = odom_quat;
        
        odom.child_frame_id = "base_link";
        odom.twist.twist.linear.x = current_velocity.linear.x;
        odom.twist.twist.linear.y = current_velocity.linear.y;
        odom.twist.twist.linear.z = current_velocity.linear.z;
        odom.twist.twist.angular.z = current_velocity.angular.z;
        
        odom_pub.publish(odom);
        
        average_encoder_counts = 0;
        current_velocity.linear.x = 0;
        current_velocity.linear.y = 0;
        current_velocity.angular.z = 0;
	}
}

// averages the encoder counts
void odometryCallback(const GCRobotics::Encoder_msg::ConstPtr& msg)
{
    if (mask_odom)
        average_encoder_counts = 0;
    else
        average_encoder_counts = (msg->encoder1 + msg->encoder2 + msg->encoder3 + msg->encoder4) / 4.0;
    
    if (msg-> Direction1 ==1 && msg-> Direction2 ==1 && msg-> Direction3 ==1 && msg-> Direction4 ==1)//needs to be changed to match whats being published
        direction = 'w';
    else if (msg-> Direction1 ==-1 && msg-> Direction2 ==-1 && msg-> Direction3 ==-1 && msg-> Direction4 ==-1)
        direction = 's';
    else if (msg-> Direction1 ==-1 && msg-> Direction2 ==1 && msg-> Direction3 ==-1 && msg-> Direction4 ==1)
        direction = 'a';
    else if (msg-> Direction1 ==1 && msg-> Direction2 ==-1 && msg-> Direction3 ==1 && msg-> Direction4 ==-1)
        direction = 'd';
    else if (msg-> Direction1 ==1 && msg-> Direction2 ==1 && msg-> Direction3 ==-1 && msg-> Direction4 ==-1)
        direction = 'q';
    else if (msg-> Direction1 ==-1 && msg-> Direction2 ==-1 && msg-> Direction3 ==1 && msg-> Direction4 ==1)
        direction = 'e';
    else
        direction = 'f';
}

// gathers the current velocity and direction
void velocityCallback(const geometry_msgs::Twist::ConstPtr& msg)
{
    if (mask_odom)
    

    current_velocity.linear.x = msg->linear.x;
    current_velocity.linear.y = msg->linear.y;
    current_velocity.angular.z = msg->angular.z;

 /*  if (msg->linear.x > 0) //not needed with encoder info
        direction = 'w';
    else if (msg->linear.x < 0)
        direction = 's';
    else if (msg->linear.y > 0)
        direction = 'a';
    else if (msg->linear.y < 0)
        direction = 'd';
    else if (msg->angular.z > 0)
        direction = 'q';
    else if (msg->angular.z < 0)
        direction = 'e';
    else
        direction = 'f';*/
}

void mask_odom_callback(const GCRobotics::command_state::ConstPtr& msg)
{
    mask_odom = msg.mask_odom;
}

