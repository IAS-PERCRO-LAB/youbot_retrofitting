#!/usr/bin/env python3

import rospy
from geometry_msgs.msg import PoseStamped
from geometry_msgs.msg import Twist
import math
import time

#Global variables definition for command velocity and time-check
cmd = Twist()
last_image_time = 0 #[s]
image_timeout = 1 #[s]
is_image_visibile = False

#Subscriber callback used to read aruco pose and set wheels speed
def pose_sub_callback(msg):
    global cmd, last_image_time

    #Aruco pose in aruco frame (check README for more details)
    aruco_side = msg.pose.position.x
    aruco_approach = msg.pose.position.z
    aruco_yaw = 2*math.acos(msg.pose.orientation.w) - math.pi
    rospy.loginfo("X: " + str(aruco_side) + " Z: " + str(aruco_approach) + " Yaw: " + str(aruco_yaw))

    #K-gains for side, approach and yaw speed
    k_side = 1
    k_approach = 0.8
    k_yaw = 0.5

    #Saturation for side, approach and yaw speed
    side_speed_saturation = 1.5 #[m/s]
    approach_speed_saturation = 1 #[m/s]
    yaw_speed_saturation = 0.5 #[rads/s]

    #Side, approach and yaw tollerances
    side_tollerance = 0.01 #[m]
    approach_tollerance = 0.2 #[m]
    yaw_tollerance = (math.pi)/36 #[rads]

    #Speed definitions in camera frame to aruco frame with rotation matrix Rta = [0 -1 0; 0 0 -1; -1 0 0]
    side_speed = -k_side*(aruco_side)                    
    approach_speed = k_approach*(aruco_approach)
    yaw_speed = -k_yaw*(aruco_yaw)

    #Last pose message time counter
    last_image_time = time.time()

    #Set-speeds conditions
    if(abs(aruco_side) <= side_tollerance):
        cmd.linear.y = 0
    elif(abs(aruco_side) > side_tollerance):
        if(abs(side_speed) < side_speed_saturation):
            cmd.linear.y = side_speed
        else:
            cmd.linear.y = (side_speed/abs(side_speed))*side_speed_saturation 
    else:
        cmd.linear.y = 0

    if(abs(aruco_approach) <= approach_tollerance):
        cmd.linear.x = 0
    elif(abs(aruco_approach) > approach_tollerance):
        if(abs(approach_speed) < approach_speed_saturation):
            cmd.linear.x = approach_speed
        else:
            cmd.linear.x = (approach_speed/abs(approach_speed))*approach_speed_saturation  
    else:
        cmd.linear.x = 0

    if(aruco_yaw <= yaw_tollerance and aruco_yaw >= -yaw_tollerance):
        cmd.angular.z = 0
    elif(aruco_yaw > yaw_tollerance or aruco_yaw < -yaw_tollerance):
        if(abs(yaw_speed) < yaw_speed_saturation):
            cmd.angular.z = yaw_speed
        else:
            cmd.angular.z = (yaw_speed/abs(yaw_speed))*yaw_speed_saturation  
    else:
        cmd.angular.z = 0

    return(cmd)
    
#Image visibility method last-msg-time-based
def image_visibility():
    global is_image_visibile
    if((time.time() - last_image_time) < image_timeout):
        is_image_visibile = True
    else:
        is_image_visibile = False
    
    return is_image_visibile

#Speeds reset
def cmd_reset():
    global cmd
    cmd.linear.x = 0
    cmd.linear.y = 0
    cmd.angular.z = 0

    return(cmd)

if __name__ == '__main__':
    rospy.init_node("youbot_aruco_controller")
    rospy.loginfo("Controller has been started")

    #Speeds publisher and pose subscriber definitions
    twist_pub = rospy.Publisher('cmd_vel', Twist, queue_size=10)
    pose_sub = rospy.Subscriber("/aruco_single/pose", PoseStamped, callback=pose_sub_callback)
    
    #Publishing frequency
    pub_rate = rospy.Rate(10) #[Hz]

    while not rospy.is_shutdown():

        #Aruco visibility check
        image_visibility()

        if(is_image_visibile):
            twist_pub.publish(cmd)
        else:
            cmd_reset()
            twist_pub.publish(cmd)

        pub_rate.sleep()

    rospy.spin()