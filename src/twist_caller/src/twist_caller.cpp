
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


int main(int argc, char **argv) {

    ros::init(argc, argv, "twist_caller");
    ros::NodeHandle nh;

//  ros::ServiceServer service = nh.advertiseService("call_twist", call_ur_twist);
    ros::ServiceClient client = nh.serviceClient<twist_srv::call_twist>("call_twist");
    twist_srv::call_twist srv;

    ros::Rate loop_rate(0.5);
    ros::NodeHandle n;
    int ee_angle = 0;
    int ee_angle_max = 270;

//
//    int ee_angle_int = 0;

    while (ros::ok()) {

        ee_angle = (ee_angle + 90) % ee_angle_max;
        srv.request.angle = ee_angle;
//        client.call(srv);
        printf("ee_angle: %d\n", ee_angle);
        ros::spinOnce();

        loop_rate.sleep();
    }
}
