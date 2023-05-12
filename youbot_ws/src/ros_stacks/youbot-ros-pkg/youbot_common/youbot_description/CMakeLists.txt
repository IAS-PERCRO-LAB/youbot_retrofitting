cmake_minimum_required(VERSION 2.4.6)
PROJECT(youbot_description)

SET(Boost_ADDITIONAL_VERSIONS "1.39" "1.39.0" "1.40" "1.40.0" "1.41" "1.41.0" "1.42" "1.42.0" "1.43" "1.43.0" "1.44" "1.44.0" "1.45" "1.45.0" "1.46" "1.46.0")
find_package(Boost REQUIRED COMPONENTS thread date_time filesystem system)
find_package(catkin REQUIRED youbot_trajectory_action_server brics_actuator pr2_controller_interface urdf)

catkin_package()

include_directories(include ${catkin_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src)

#youbot base teleoperation experiment 
add_executable(youbot_keyboard_teleop_experiment examples/keyboard_teleop_experiment.cpp)
target_link_libraries(youbot_keyboard_teleop_experiment ${catkin_LIBRARIES})

#test for a joint position control of the youbot arm
add_library(joint_position_control src/joint_position_control.cpp)
target_link_libraries(joint_position_control ${catkin_LIBRARIES})

#youbot universal controller: joint space position control, joint velocity control, joint torque control
add_library(youbot_universal_control src/youbot_universal_control.cpp src/joint_state_observer_gazebo.cpp)
target_link_libraries(youbot_universal_control  ${catkin_LIBRARIES})

add_executable(arm_joint_position_control_test examples/arm_joint_position_control_test.cpp)
target_link_libraries(arm_joint_position_control_test  ${catkin_LIBRARIES})

add_executable(arm_joint_velocity_control_test examples/arm_joint_velocity_control_test.cpp)
target_link_libraries(arm_joint_velocity_control_test  ${catkin_LIBRARIES})

add_executable(arm_joint_torque_control_test examples/arm_joint_torque_control_test.cpp)
target_link_libraries(arm_joint_torque_control_test  ${catkin_LIBRARIES})

#static joint states publisher
add_executable(static_joint_publisher src/static_joint_publisher.cpp)
target_link_libraries(static_joint_publisher  ${catkin_LIBRARIES})

#depricated
add_executable(base_command_republisher src/base_command_republisher.cpp)
target_link_libraries(base_command_republisher  ${catkin_LIBRARIES})
