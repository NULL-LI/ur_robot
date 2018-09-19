cmake_minimum_required(VERSION 2.8.3)
project(grasp_srv)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
std_srvs
message_generation
)

#add_message_files(
#    DIRECTORY
#    msg
#  FILES
#  call_grasp.msg
#grasp_status.msg
#)

add_service_files(
    DIRECTORY
    srv
  FILES
  call_grasp.srv
)

generate_messages(DEPENDENCIES std_msgs std_srvs)

catkin_package(
  CATKIN_DEPENDS
    std_msgs
message_runtime
)

include_directories(
  ${catkin_INCLUDE_DIRS}
${PROJECT_SOURCE_DIR}/../../devel/include/
)

# add_executable(grasp_service_node src/grasp_service.cpp)

# target_link_libraries(grasp_service_node
#   ${catkin_LIBRARIES}
# )


