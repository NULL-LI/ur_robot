
#include <ros/ros.h>
#include <serial/serial.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Empty.h>
#include <std_msgs/String.h>
#include <std_msgs/Float32.h>
#include <string>
#include "bend_srv/call_bend.h"
#include "ur_msgs/IOStates.h"
#include "ur_msgs/SetIO.h"
#include "ur_msgs/SetIORequest.h"
#include "ur_msgs/SetIOResponse.h"
#include <iostream>
#include <cstdio>

#define USHORT unsigned short int
#define BYTE unsigned char
#define CHAR char
#define UCHAR unsigned char
#define UINT unsigned int
#define DWORD unsigned int
#define PVOID void *
#define ULONG unsigned int
#define INT int
#define UINT32 UINT
#define LPVOID void *
#define BOOL BYTE
#define TRUE 1
#define FALSE 0

serial::Serial ser;
using namespace std;

bool call_ur_bend(bend_srv::call_bend::Request &req,
                  bend_srv::call_bend::Response &res) {
  ros::NodeHandle n;

  if (1) {
    ROS_INFO("Called bend angle %f", req.angle);

  } else {
    ROS_ERROR("Failed to call ur io");
    return 1;
  }

  return true;
}

int main(int argc, char **argv) {
  try {
    ser.setPort("/dev/ttyUSB0");
    ser.setBaudrate(9600);
    serial::Timeout to = serial::Timeout::simpleTimeout(1000);
    ser.setTimeout(to);
    ser.open();
  } catch (serial::IOException &e) {
    ROS_ERROR_STREAM("Unable to open port ");
    return -1;
  }
  if (ser.isOpen()) {
    ROS_INFO_STREAM("Serial Port initialized");
  } else {
    return -1;
  }

  ros::init(argc, argv, "ur_bend_server");
  ros::NodeHandle nh;

  ros::ServiceServer service = nh.advertiseService("call_bend", call_ur_bend);

  ROS_INFO("Ready to bend.");
  ros::Rate loop_rate(10);
  ros::NodeHandle n;
  ros::Publisher bend_angle_pub =
      n.advertise<std_msgs::Float32>("/bend_angle", 1000);
  std_msgs::Float32 bend_angle_msg;
std::string inString = "";
float ee_angle=0;

int  ee_angle_int=0;
  while (ros::ok()) {
std::string datastr = ser.read(ser.available());
 sscanf(datastr.data(), "DTU%dDTU", &ee_angle_int);
            ee_angle = ee_angle_int / 10;


          bend_angle_msg.data=ee_angle;
          bend_angle_pub.publish(bend_angle_msg);
    ros::spinOnce();

    loop_rate.sleep();
  }
}
