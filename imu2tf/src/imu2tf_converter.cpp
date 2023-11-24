
#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <sensor_msgs/Imu.h>

void poseCallback(const sensor_msgs::Imu& msg)
{
  static tf::TransformBroadcaster br;
  tf::Transform transform;
  transform.setOrigin(tf::Vector3(0.0, 0.0, 0.0));
  //    tf::Quaternion q;
  //    double r,p;
  //    r = atan(msg.linear_acceleration.y/msg.linear_acceleration.z);
  //    p = atan(-msg.linear_acceleration.x/(sqrt(pow(msg.linear_acceleration.y,2)+pow(msg.linear_acceleration.z,2))));
  //    q.setRPY(r, p, 0);

  tf::Quaternion q(msg.orientation.x, msg.orientation.y, msg.orientation.z, msg.orientation.w);
  tf::Matrix3x3 m(q);

  double roll, pitch, yaw;
  m.getRPY(roll, pitch, yaw);
  q.setRPY(roll, pitch, yaw);

  transform.setRotation(q);
  br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "odom", "base_link"));
  //    br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "odom", "laser"));
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "my_tf_broadcaster");
  ros::NodeHandle node;
  ros::Subscriber sub = node.subscribe("/imu", 10, &poseCallback);

  ros::spin();
  return 0;
};
