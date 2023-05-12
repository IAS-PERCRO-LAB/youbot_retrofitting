# Install dependencies on ROS Noetic Ubuntu 20.04
```
sudo apt update
sudo apt install -y libcppunit-dev libeigen3-dev libcap2-bin ros-noetic-pr2-msgs ros-noetic-pr2-controller-manager ros-noetic-xacro

cd youbot-retrofitting/youbot_ws
catkin clean -y
catkin build
source devel/setup.bash
```

Avoid Error while loading shared libraries
```
sudo ldconfig /opt/ros/noetic/lib
```

# Enable sudo permissions
```
sudo setcap cap_net_raw+ep devel/.private/youbot_oodl/lib/youbot_oodl/youbot_oodl
```

# Connect the Ethernet cable and set the network static address

```
IP 192.168.1.X - NET MASK 255.255.255.0
```

# test on the robot
```
roslaunch youbot_oodl youbot_oodl_driver.launch
roslaunch youbot_oodl youbot_joint_state_publisher.launch
rosrun youbot_oodl youbot_keyboard_teleop.py
```



