# Install dependencies on ROS Noetic Ubuntu 20.04

sudo apt install ros-noetic-pr2-msgs
sudo apt install libcppunit-dev
sudo apt install libeigen3-dev 
sudo apt install ros-noetic-pr2-controller-manager
sudo apt install ros-noetic-xacro
sudo apt install libcap2-bin
catkin build

# Enable sudo permissions
sudo setcap cap_net_raw+ep devel/.private/youbot_oodl/lib/youbot_oodl/youbot_oodl

# X from 2 to 254
put network static address: 192.168.1.X/255.255.255.0 

# test on the robot
roslaunch youbot_oodl youbot_oodl_driver.launch
roslaunch youbot_oodl youbot_joint_state_publisher.launch
rosrun youbot_oodl youbot_keyboard_teleop.py




