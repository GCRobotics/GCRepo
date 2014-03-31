cmake_minimum_required(VERSION 2.4.6)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
#set(ROS_BUILD_TYPE RelWithDebInfo)

rosbuild_init()

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#uncomment if you have defined messages
rosbuild_genmsg()
#uncomment if you have defined services
#rosbuild_gensrv()

#common commands for building c++ executables and libraries
#rosbuild_add_library(${PROJECT_NAME} src/example.cpp)
#target_link_libraries(${PROJECT_NAME} another_library)
#rosbuild_add_boost_directories()
#rosbuild_link_boost(${PROJECT_NAME} thread)
#rosbuild_add_executable(example examples/example.cpp)
#target_link_libraries(example ${PROJECT_NAME})

find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs message_generation)

add_message_files(
  FILES
  command_state.msg
)

rosbuild_add_executable(command_node src/command_node/command_node.cpp)
rosbuild_add_executable(odometryNode src/Odometry/odometryNode.cpp)
rosbuild_add_executable(motorControlNode src/MotorControl/motorControlNode.cpp src/MotorControl/motorControl.cpp)
#rosbuild_add_executable(DeadReckoner src/DeadReckoning/deadReckoning.cpp)
#rosbuild_add_executable(GlobalPlanner src/PathPlanners/GlobalPlannerNode.cpp src/PathPlanners/GlobalPlanner.cpp src/PathPlanners/Cell.cpp)
#rosbuild_add_executable(LocalPlanner src/PathPlanners/LocalPlannerNode.cpp src/PathPlanners/LocalPlanner.cpp)
rosbuild_add_executable(transform_bc_node src/Transforms/transform_bc_node.cpp)
#rosbuild_add_executable(nav_test src/nav_test/nav_test.cpp)
#target_link_libraries(nav_test ${catkin_LIBRARIES})
