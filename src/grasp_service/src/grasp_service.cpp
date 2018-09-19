
#include <ros/ros.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Empty.h>
#include <std_msgs/String.h>
#include <string>
#include "grasp_srv/call_grasp.h"
#include "ur_msgs/IOStates.h"
#include "ur_msgs/SetIO.h"
#include "ur_msgs/SetIORequest.h"
#include "ur_msgs/SetIOResponse.h"

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

using namespace std;

bool ur_grasped = false;

void pressureStateCallback(const ur_msgs::IOStates &msg) {
  if ((msg.digital_in_states[16]).pin == 16) {
    if ((msg.digital_in_states[16]).state == true) {
      ur_grasped = true;
      ROS_INFO("ur_grasped true");
    } else {
      ur_grasped = false;
      ROS_INFO("ur_grasped false");
    }

  } else {
    ROS_INFO("PIN is not 16 PIN: %d", (msg.digital_in_states[16]).pin);
  }
}

bool call_ur_grasp(grasp_srv::call_grasp::Request &req,
                   grasp_srv::call_grasp::Response &res) {
  ros::NodeHandle n;
  ros::ServiceClient client =
      n.serviceClient<ur_msgs::SetIO>("/ur_driver/set_io");
  ur_msgs::SetIO srv;
  int state = req.grasp ? 0 : 1;

  srv.request.fun = (int8_t)1;
  srv.request.pin = (int8_t)16;
  srv.request.state = state;
  if (client.call(srv)) {
    ROS_INFO("Called ur io. fun %d pin %d state %f", srv.request.fun,
             srv.request.pin, srv.request.state);

  } else {
    ROS_ERROR("Failed to call ur io");
    return 1;
  }

  // usleep(1000000);//+1s

  // res.grasped=ur_grasped;
  res.acted = true;
  return true;
}

int main(int argc, char **argv) {
  /*test of linux data format*/
  //    __int16_t a=-2;
  //    BYTE* testbyteptr=(BYTE*)&a;
  //    printf("high %d  low %d \n",*(testbyteptr+0),*(testbyteptr+1));

  ros::init(argc, argv, "ur_grasp_server");
  ros::NodeHandle nh;

  // ros::Subscriber write_sub = nh.subscribe("write", 1000, write_callback);
  //  ros::Publisher read_pub = nh.advertise<std_msgs::String>("read", 1000);

  ros::ServiceServer service = nh.advertiseService("call_grasp", call_ur_grasp);
  ros::Subscriber sub =
      nh.subscribe("/ur_driver/io_states", 1000, pressureStateCallback);
  ROS_INFO("Ready to grasp.");
  ros::Rate loop_rate(5);
  ros::NodeHandle n;
  ros::Publisher grasp_state_pub =
      n.advertise<std_msgs::Bool>("/grasp_state", 1000);
  std_msgs::Bool grasp_state_msg;
  while (ros::ok()) {
    grasp_state_msg.data = ur_grasped;
    grasp_state_pub.publish(grasp_state_msg);
    ros::spinOnce();

    loop_rate.sleep();
  }
}
