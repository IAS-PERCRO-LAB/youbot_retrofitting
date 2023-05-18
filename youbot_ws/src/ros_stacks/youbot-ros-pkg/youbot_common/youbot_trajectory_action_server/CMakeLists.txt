cmake_minimum_required(VERSION 3.0.2)
project(youbot_trajectory_action_server)


SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/cmake_modules")
SET(Boost_ADDITIONAL_VERSIONS "1.39" "1.39.0" "1.40" "1.40.0" "1.41" "1.41.0" "1.42" "1.42.0" "1.43" "1.43.0" "1.44" "1.44.0" "1.45" "1.45.0" "1.46" "1.46.0")

find_package(catkin REQUIRED roscpp brics_actuator)
find_package(cmake_modules REQUIRED)
find_package(Boost REQUIRED COMPONENTS thread date_time filesystem system)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(Eigen REQUIRED)

catkin_package(
  INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include
  LIBRARIES youbot_trajectory_action_server
  CATKIN_DEPENDS roscpp
  DEPENDS Boost Eigen
)

include_directories(include ${catkin_INCLUDE_DIRS} ${Eigen_INCLUDE_DIRS})

add_library(youbot_trajectory_action_server
    src/joint_trajectory_action.cpp)

target_link_libraries(youbot_trajectory_action_server ${catkin_LIBRARIES} ${Boost_LIBRARIES})
