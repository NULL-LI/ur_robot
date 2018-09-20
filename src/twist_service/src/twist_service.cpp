
#include "twist_srv/call_twist.h"
#include "ur_msgs/IOStates.h"
#include "ur_msgs/SetIO.h"
#include "ur_msgs/SetIORequest.h"
#include "ur_msgs/SetIOResponse.h"
#include <cstdio>
#include <iostream>
#include <ros/ros.h>
#include <serial/serial.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Empty.h>
#include <std_msgs/Float32.h>
#include <std_msgs/String.h>
#include <string>

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

double ee_angle_send=0;

bool call_ur_twist(twist_srv::call_twist::Request &req,
                   twist_srv::call_twist::Response &res) {
//  ros::NodeHandle n;

//  ROS_INFO("%s",ser.available()?"available":"not available");
  if (1) {
    ee_angle_send=req.angle;

    ROS_INFO("Called twist %f", req.angle);
  } else {
    ROS_ERROR("Serial unavailable!");
    return 1;
  }
  return true;
}

int main(int argc, char **argv) {
  try {
    ser.setPort("/dev/ttyACM0");
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

  ros::ServiceServer service = nh.advertiseService("call_twist", call_ur_twist);

  ROS_INFO("Ready to twist.");
  ros::Rate loop_rate(10);
  ros::NodeHandle n;
  ros::Publisher bend_angle_pub =
      n.advertise<std_msgs::Float32>("/bend_angle", 1000);
  std_msgs::Float32 bend_angle_msg;
  std::string inString = "";
  float ee_angle = 0;

  int ee_angle_int = 0;
  while (ros::ok()) {
      ROS_INFO("%s",ser.available()?"available":"not available");
    std::string datastr = ser.read(ser.available());
    sscanf(datastr.data(), "DTU%dDTU", &ee_angle_int);
    ee_angle = ee_angle_int / 10;

    bend_angle_msg.data = ee_angle;
    bend_angle_pub.publish(bend_angle_msg);

    string stringSend;
    char charSend[50];
    sprintf(charSend, "UTD%dUTD\n\r", (int)(ee_angle_send * 10));
    ROS_INFO("%s",charSend);
    stringSend = charSend;
    ser.write(stringSend);

    ros::spinOnce();

    loop_rate.sleep();
  }
}
