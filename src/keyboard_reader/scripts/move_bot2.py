#!/usr/bin/env python
import rospy
from geometry_msgs.msg import Twist
from keyboard_reader.msg import Key

#griffin_powermate.msg import PowermateEvent
#roscd tutorial/scripts
#chmod +x subtrac.py


state = False
move = Twist()
cnt = 0

def callback(msg):
    global move,cnt,state
#is_pressed
    if (msg.key_pressed == True):
	cnt = cnt + 1
	rospy.loginfo(cnt)
        state = not state

    	if (state == True):
		rospy.loginfo('Moving')
        	move.linear.x = 1
	else:
		rospy.loginfo('Stopping')
		move.linear.x = 0	

def main_f():

    rospy.init_node('move_bot2', anonymous=True)
#griffin_powermate/events mesto keyboard, mesto key PowermateEvent
    rospy.Subscriber("keyboard", Key, callback)
    pub = rospy.Publisher('husky_velocity_controller/cmd_vel', Twist, queue_size=100)
        
    move.linear.x = 0
    move.linear.y = 0
    move.linear.z = 0
    move.angular.x = 0
    move.angular.y = 0
    move.angular.z = 0

    rate = rospy.Rate(10)

    while not rospy.is_shutdown():

    	pub.publish(move)
    	
	#rospy.spin()
	rate.sleep()


if __name__ == '__main__':
	main_f()
