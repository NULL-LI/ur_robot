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


echo "twist_service"
rosrun ur_ee_service ur_ee_service_node




