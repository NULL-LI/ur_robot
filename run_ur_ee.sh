#!/bin/bash

echo ""
echo "RUNNING UR EE!"

echo ""
echo "Getting IO permission..."
sudo chmod 777 /dev/ttyACM0

echo ""
echo "Refresh ROS environment..."
rospack profile&&

source devel/setup.bash&&


echo ""
echo "Start ROS nodes..."


echo "grasp_service"
rosrun grasp_service grasp_service_node&
echo "twist_service"
rosrun twist_service twist_service_node




