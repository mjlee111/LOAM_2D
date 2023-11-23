#include "../include/node.h"

namespace loam
{
loam_dynamixel::loam_dynamixel(int argc, char** argv)
  : init_argc(argc), init_argv(argv), device_name("/dev/ttyUSB1"), default_rpm(20)
{
  bool isRos = init();
  if (isRos)
  {
    spin();
  }
}

loam_dynamixel::~loam_dynamixel()
{
  if (ros::isStarted())
  {
    ROS_INFO("Killing Node");
    setMotorSpeed(DXL1_ID, 0);
    ros::shutdown();
    ros::waitForShutdown();
  }
}

bool loam_dynamixel::init()
{
  ros::init(init_argc, init_argv, "loam_dynamixel");
  if (!ros::master::check())
  {
    ROS_INFO("master not available.");
    ROS_INFO("ending node");

    return false;
  }
  ROS_INFO("master available.");
  ros::start();
  isCom = initDynamixelSdk();
  ros::NodeHandle nh;

  rpm_sub = nh.subscribe<std_msgs::Int64>("/loam_dynamixel/rpm", 1, &loam_dynamixel::rpmCallback, this);
  return true;
}

void loam_dynamixel::spin()
{
  while (ros::ok())
  {
    if (isCom)
    {
      setMotorSpeed(DXL1_ID, default_rpm);
    }
    ros::spinOnce();
  }
  setMotorSpeed(DXL1_ID, 0);
}

bool loam_dynamixel::initDynamixelSdk()
{
  portHandler = dynamixel::PortHandler::getPortHandler(device_name.c_str());
  packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
  uint8_t dxl_error = 0;
  int dxl_comm_result = COMM_TX_FAIL;

  if (!portHandler->openPort())
  {
    ROS_ERROR("Failed to open the port!");
    return false;
  }
  ROS_INFO("Opened port %s", device_name.c_str());

  if (!portHandler->setBaudRate(BAUDRATE))
  {
    ROS_ERROR("Failed to set the baudrate!");
    return false;
  }
  ROS_INFO("Set baudrate to %d", BAUDRATE);

  return true;
}

void loam_dynamixel::setMotorSpeed(int id, double rpm)
{
  rpm /= 0.114;
  uint8_t dxl_error = 0;
  int dxl_comm_result = COMM_TX_FAIL;
  uint32_t spd = (unsigned int)((int)rpm);
  dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, (uint8_t)id, ADDR_MOVING_SPEED, spd, &dxl_error);
}

void loam_dynamixel::rpmCallback(const std_msgs::Int64ConstPtr& msg)
{
  default_rpm = msg->data;
  ROS_INFO("Setting RPM to : %d", default_rpm);
}
}  // namespace loam