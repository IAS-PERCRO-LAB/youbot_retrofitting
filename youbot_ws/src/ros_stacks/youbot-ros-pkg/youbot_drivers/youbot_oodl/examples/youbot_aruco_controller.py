#!/usr/bin/env python3

import rospy
from geometry_msgs.msg import PoseStamped
from geometry_msgs.msg import Twist
import math
import time

cmd = Twist()
last_image_time = 0
image_timeout = 1
is_image_visibile = False

def pose_sub_callback(msg):
    global cmd, last_image_time

    aruco_side = msg.pose.position.x
    aruco_approach = msg.pose.position.z
    aruco_yaw = 2*math.acos(msg.pose.orientation.w) - math.pi
    rospy.loginfo("X: " + str(aruco_side) + " Z: " + str(aruco_approach) + " Yaw: " + str(aruco_yaw))

    k_side = 1
    k_approach = 0.8
    k_yaw = 0.5

    side_speed_saturation = 1.5
    approach_speed_saturation = 1
    yaw_speed_saturation = 0.5

    side_speed = -k_side*(aruco_side)
    approach_speed = k_approach*(aruco_approach)
    yaw_speed = -k_yaw*(aruco_yaw)

    last_image_time = time.time()

    if(abs(aruco_side) <= 0.01):
        cmd.linear.y = 0
    elif(abs(aruco_side) > 0.01):
        if(abs(side_speed) < side_speed_saturation):
            cmd.linear.y = side_speed
        else:
            cmd.linear.y = (side_speed/abs(side_speed))*side_speed_saturation #
    else:
        cmd.linear.y = 0

    if(abs(aruco_approach) <= 0.2):
        cmd.linear.x = 0
    elif(abs(aruco_approach) > 0.2):
        if(abs(approach_speed) < approach_speed_saturation):
            cmd.linear.x = approach_speed
        else:
            cmd.linear.x = (approach_speed/abs(approach_speed))*approach_speed_saturation  #
    else:
        cmd.linear.x = 0

    if(aruco_yaw <= (math.pi)/36 and aruco_yaw >= -(math.pi)/36):
        cmd.angular.z = 0
    elif(aruco_yaw > (math.pi)/36 or aruco_yaw < -(math.pi)/36):
        if(abs(yaw_speed) < yaw_speed_saturation):
            cmd.angular.z = yaw_speed
        else:
            cmd.angular.z = (yaw_speed/abs(yaw_speed))*yaw_speed_saturation  #
    else:
        cmd.angular.z = 0

    return(cmd)
    
def image_visibility():
    global is_image_visibile
    if((time.time() - last_image_time) < image_timeout):
        is_image_visibile = True
    else:
        is_image_visibile = False
    
    return is_image_visibile

def cmd_reset():
    global cmd
    cmd.linear.x = 0
    cmd.linear.y = 0
    cmd.angular.z = 0

    return(cmd)

if __name__ == '__main__':
    rospy.init_node("youbot_aruco_controller")
    rospy.loginfo("Controller has been started")

    twist_pub = rospy.Publisher('cmd_vel', Twist, queue_size=10)
    pose_sub = rospy.Subscriber("/aruco_single/pose", PoseStamped, callback=pose_sub_callback)
    
    pub_rate = rospy.Rate(10)

    while not rospy.is_shutdown():

        image_visibility()

        if(is_image_visibile):
            twist_pub.publish(cmd)
        else:
            cmd_reset()
            twist_pub.publish(cmd)

        pub_rate.sleep()

    rospy.spin()