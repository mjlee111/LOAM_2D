#ifndef NODE_H
#define NODE_H

#include <dynamixel_sdk/dynamixel_sdk.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <ros/ros.h>
#include <thread>
#include <mutex>
#include <std_msgs/Int64.h>

#define ADDR_TORQUE_ENABLE 24
#define ADDR_MOVING_SPEED 32
#define PROTOCOL_VERSION 1.0
#define DXL1_ID 100
#define BAUDRATE 1000000

namespace loam
{
class loam_dynamixel
{
public:
  loam_dynamixel(int argc, char** argv);
  ~loam_dynamixel();
  bool init();
  void spin();

  dynamixel::PortHandler* portHandler;
  dynamixel::PacketHandler* packetHandler;

private:
  int init_argc;
  char** init_argv;
  std::string device_name = "/dev/ttyUSB0";
  double default_rpm = 20;

  bool isCom = false;

  bool initDynamixelSdk();
  void setMotorSpeed(int id, double rpm);

  ros::Subscriber rpm_sub;
  void rpmCallback(const std_msgs::Int64ConstPtr& msg);
};
}  // namespace loam

#endif  // NODE_H