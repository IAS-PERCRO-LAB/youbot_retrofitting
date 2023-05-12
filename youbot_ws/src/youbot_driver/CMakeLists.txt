CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(youbot_driver)

find_package(catkin REQUIRED COMPONENTS roscpp)

########### Boost ###########
SET(Boost_ADDITIONAL_VERSIONS "1.39" "1.39.0" "1.40" "1.40.0" "1.41" "1.41.0" "1.42" "1.42.0")
FIND_PACKAGE(Boost REQUIRED COMPONENTS thread date_time filesystem system regex)

catkin_package(
  INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/youbot ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/soem/src
  LIBRARIES youbot_driver
  CATKIN_DEPENDS roscpp
  DEPENDS Boost
)

########### Simple Open EtherCAT Master Library ###########
add_subdirectory(soem)

INCLUDE_DIRECTORIES(
 ${PROJECT_SOURCE_DIR}
 ${PROJECT_SOURCE_DIR}/soem/src
)

########### youBot display example application ###########
add_subdirectory(display)

SET(YOUBOT_DRIVER_SRC
  ${PROJECT_SOURCE_DIR}/youbot/EthercatMaster.cpp
  ${PROJECT_SOURCE_DIR}/youbot/EthercatMasterWithThread.cpp
  ${PROJECT_SOURCE_DIR}/youbot/EthercatMasterWithoutThread.cpp
  ${PROJECT_SOURCE_DIR}/generic/Logger.cpp
  ${PROJECT_SOURCE_DIR}/generic/ConfigFile.cpp
  ${PROJECT_SOURCE_DIR}/generic/PidController.cpp
  ${PROJECT_SOURCE_DIR}/generic-joint/JointData.cpp
  ${PROJECT_SOURCE_DIR}/generic-joint/JointTrajectory.cpp
  ${PROJECT_SOURCE_DIR}/youbot/YouBotJoint.cpp
  ${PROJECT_SOURCE_DIR}/youbot/JointLimitMonitor.cpp
  ${PROJECT_SOURCE_DIR}/youbot/YouBotBase.cpp
  ${PROJECT_SOURCE_DIR}/youbot/YouBotManipulator.cpp
  ${PROJECT_SOURCE_DIR}/youbot/YouBotGripper.cpp
  ${PROJECT_SOURCE_DIR}/youbot/YouBotGripperBar.cpp
  ${PROJECT_SOURCE_DIR}/youbot/YouBotGripperParameter.cpp
  ${PROJECT_SOURCE_DIR}/youbot/DataTrace.cpp
  ${PROJECT_SOURCE_DIR}/youbot/GripperDataTrace.cpp
  ${PROJECT_SOURCE_DIR}/youbot/YouBotJointParameter.cpp
  ${PROJECT_SOURCE_DIR}/youbot/YouBotJointParameterReadOnly.cpp
  ${PROJECT_SOURCE_DIR}/youbot/YouBotJointParameterPasswordProtected.cpp
  ${PROJECT_SOURCE_DIR}/youbot/JointTrajectoryController.cpp
  ${PROJECT_SOURCE_DIR}/base-kinematic/FourSwedishWheelOmniBaseKinematic.cpp
  ${PROJECT_SOURCE_DIR}/base-kinematic/FourSwedishWheelOmniBaseKinematicConfiguration.cpp
  ${PROJECT_SOURCE_DIR}/youbot/gdn_lib.cpp
)


SET(YOUBOT_DRIVER_HEADERS
  ${PROJECT_SOURCE_DIR}/youbot/EthercatMaster.hpp
  ${PROJECT_SOURCE_DIR}/youbot/EthercatMasterInterface.hpp
  ${PROJECT_SOURCE_DIR}/youbot/EthercatMasterWithThread.hpp
  ${PROJECT_SOURCE_DIR}/youbot/EthercatMasterWithoutThread.hpp
  ${PROJECT_SOURCE_DIR}/youbot/YouBotGripper.hpp
  ${PROJECT_SOURCE_DIR}/youbot/YouBotGripperBar.hpp
  ${PROJECT_SOURCE_DIR}/youbot/YouBotGripperParameter.hpp
  ${PROJECT_SOURCE_DIR}/youbot/YouBotBase.hpp
  ${PROJECT_SOURCE_DIR}/youbot/YouBotManipulator.hpp
  ${PROJECT_SOURCE_DIR}/youbot/YouBotJoint.hpp
  ${PROJECT_SOURCE_DIR}/youbot/JointLimitMonitor.hpp
  ${PROJECT_SOURCE_DIR}/youbot/YouBotJointParameterReadOnly.hpp
  ${PROJECT_SOURCE_DIR}/youbot/YouBotJointParameterPasswordProtected.hpp
  ${PROJECT_SOURCE_DIR}/youbot/YouBotJointParameter.hpp
  ${PROJECT_SOURCE_DIR}/youbot/YouBotSlaveMailboxMsg.hpp
  ${PROJECT_SOURCE_DIR}/youbot/YouBotSlaveMsg.hpp
  ${PROJECT_SOURCE_DIR}/youbot/ProtocolDefinitions.hpp
  ${PROJECT_SOURCE_DIR}/youbot/YouBotJointStorage.hpp
  ${PROJECT_SOURCE_DIR}/youbot/DataTrace.hpp
  ${PROJECT_SOURCE_DIR}/youbot/GripperDataTrace.hpp
  ${PROJECT_SOURCE_DIR}/youbot/JointTrajectoryController.hpp
  ${PROJECT_SOURCE_DIR}/base-kinematic/FourSwedishWheelOmniBaseKinematic.hpp
  ${PROJECT_SOURCE_DIR}/base-kinematic/FourSwedishWheelOmniBaseKinematicConfiguration.hpp
  ${PROJECT_SOURCE_DIR}/base-kinematic/WheeledBaseKinematic.hpp
  ${PROJECT_SOURCE_DIR}/generic/Exceptions.hpp
  ${PROJECT_SOURCE_DIR}/generic/Logger.hpp
  ${PROJECT_SOURCE_DIR}/generic/Time.hpp
  ${PROJECT_SOURCE_DIR}/generic/Units.hpp
  ${PROJECT_SOURCE_DIR}/generic/ConfigFile.hpp
  ${PROJECT_SOURCE_DIR}/generic/PidController.hpp
  ${PROJECT_SOURCE_DIR}/generic/dataobjectlockfree/DataObjectLockFree.hpp
  ${PROJECT_SOURCE_DIR}/generic/dataobjectlockfree/target.hpp
  ${PROJECT_SOURCE_DIR}/generic/dataobjectlockfree/os/oro_arch.h
  ${PROJECT_SOURCE_DIR}/generic/dataobjectlockfree/os/oro_gcc/oro_arch.h
  ${PROJECT_SOURCE_DIR}/generic/dataobjectlockfree/os/oro_i386/oro_arch.h
  ${PROJECT_SOURCE_DIR}/generic/dataobjectlockfree/os/oro_msvc/oro_arch.h
  ${PROJECT_SOURCE_DIR}/generic/dataobjectlockfree//os/oro_powerpc/oro_arch.h
  ${PROJECT_SOURCE_DIR}/generic/dataobjectlockfree/os/oro_powerpc/oro_atomic.h
  ${PROJECT_SOURCE_DIR}/generic/dataobjectlockfree/os/oro_powerpc/oro_system.h
  ${PROJECT_SOURCE_DIR}/generic/dataobjectlockfree/os/oro_x86_64/oro_arch.h
  ${PROJECT_SOURCE_DIR}/generic-gripper/Gripper.hpp
  ${PROJECT_SOURCE_DIR}/generic-gripper/GripperData.hpp
  ${PROJECT_SOURCE_DIR}/generic-gripper/GripperParameter.hpp
  ${PROJECT_SOURCE_DIR}/generic-joint/Joint.hpp
  ${PROJECT_SOURCE_DIR}/generic-joint/JointData.hpp
  ${PROJECT_SOURCE_DIR}/generic-joint/JointTrajectory.hpp
  ${PROJECT_SOURCE_DIR}/generic-joint/JointParameter.hpp
  ${PROJECT_SOURCE_DIR}/one-dof-gripper/OneDOFGripper.hpp
  ${PROJECT_SOURCE_DIR}/one-dof-gripper/OneDOFGripperData.hpp
)

SET(YOUBOT_DRIVER_CONFIG
  ${PROJECT_SOURCE_DIR}/config/youbot-ethercat.cfg
  ${PROJECT_SOURCE_DIR}/config/youbot-base.cfg
  ${PROJECT_SOURCE_DIR}/config/youbot-manipulator.cfg
)

ADD_LIBRARY(youbot_driver SHARED ${YOUBOT_DRIVER_SRC} ${YOUBOT_DRIVER_HEADERS})

TARGET_LINK_LIBRARIES(youbot_driver soem ${Boost_LIBRARIES})