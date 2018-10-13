#!/bin/bash

echo ""
echo "RUNNING UR EE!"

echo ""
echo "Refresh ROS environment..."
rospack profile&&

source devel/setup.bash&&


echo ""
echo "Start ROS nodes..."


echo "grasp_service"
rosrun grasp_service grasp_service_node




