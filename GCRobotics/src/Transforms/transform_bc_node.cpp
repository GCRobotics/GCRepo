#include <ros/ros.h>
#include <tf/transform_broadcaster.h>

int main(int argc, char** argv){
  ros::init(argc, argv, "transform_bc_node");
  ros::NodeHandle n;

  ros::Rate r(1);

  tf::TransformBroadcaster broadcaster;

  while(n.ok()){
    broadcaster.sendTransform(
      tf::StampedTransform(
        tf::Transform(tf::Quaternion(0, 0, 0, 1), tf::Vector3(0.15, 0.0, 0.065)),
        ros::Time::now(),"base_link", "camera_link"));
        
    broadcaster.sendTransform(
      tf::StampedTransform(
        tf::Transform(tf::Quaternion(0, 0, 0, 1), tf::Vector3(0.0, 0.0, 0.0)),
        ros::Time::now(),"camera_rgb_optical_frame", "camera_depth_optical_frame"));
    r.sleep();
  }
}
