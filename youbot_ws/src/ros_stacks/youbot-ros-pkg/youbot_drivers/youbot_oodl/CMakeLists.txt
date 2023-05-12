cmake_minimum_required(VERSION 2.4.6)
PROJECT(youbot_oodl)

## check required prerequisites
SET(Boost_ADDITIONAL_VERSIONS "1.39" "1.39.0" "1.40" "1.40.0" "1.41" "1.41.0" "1.42" "1.42.0" "1.43" "1.43.0" "1.44" "1.44.0" "1.45" "1.45.0" "1.46" "1.46.0")
find_package(Boost REQUIRED COMPONENTS thread date_time filesystem system)
find_package(catkin REQUIRED COMPONENTS roscpp geometry_msgs brics_actuator tf tf2 trajectory_msgs pr2_msgs nav_msgs diagnostic_msgs std_srvs control_msgs actionlib youbot_driver)

message("catkin_INCLUDE_DIRS: ${catkin_INCLUDE_DIRS}")

catkin_package()

ADD_DEFINITIONS(-DYOUBOT_CONFIGURATIONS_DIR=${youbot_driver_SOURCE_PREFIX}/config)

include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(youbot_oodl 
    src/youbot_oodl.cpp 
    src/YouBotOODLWrapper.cpp
    src/YouBotConfiguration.cpp
    src/joint_state_observer_oodl.cpp
)

target_link_libraries(youbot_oodl ${catkin_LIBRARIES})


