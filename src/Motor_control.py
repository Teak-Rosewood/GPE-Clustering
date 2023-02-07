#!/usr/bin/python3
import rospy 
import math 
from geometry_msgs.msg import Twist
import PPi.GPIO as GPIO
import time 

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

Ena, In1, In2 = 12, 2, 3
Enb, In3, In4 = 13, 0, 1

GPIO.setup(Ena, GPIO.OUT)
GPIO.setup(In1, GPIO.OUT)
GPIO.setup(In2, GPIO.OUT)

GPIO.setup(Enb, GPIO.OUT)
GPIO.setup(In3, GPIO.OUT)
GPIO.setup(In4, GPIO.OUT)

pwml = GPIO.PWM(Ena, 50)
pwml.start()
pwmr = GPIO.PWM(Enb, 50)
pwmr.start()

GPIO.output(In2, GPIO.LOW)
GPIO.output(In1, GPIO.HIGH)
GPIO.output(In4, GPIO.HIGH)
GPIO.output(In3, GPIO.LOW)

class cmd_velToPWM:
    linearX:float = 0.0
    angularZ:float = 0.0
    def callback(self,data):
        self.linearX = data.linear.x
        self.angularZ = data.angular.z
        self.getPWM()

    def __init__(self):
        rospy.init_node("cmd_velToPWM");
        rospy.Subscriber('/cmd_vel',Twist,self.callback,10)
        rospy.spin()
    def sideVel(self,linearX, angularZ):
        return linearX-angularZ*self.wheelSeperation/2, linearX+angularZ*self.wheelSeperation/2
    def getPWM(self, wheelSeperation=.13):
        self.wheelSeperation = wheelSeperation
        linearX = min(1.922,max(linearX,0.2))
        if angularZ>1.2:
            angularZ = 1.2
        leftVel, rightVel = self.sideVel(linearX,angularZ)
        PWML  = min(255, math.fabs(leftVel)*255/2)
        PWMR  = min(255, math.fabs(rightVel)*255/2)
        DIRL = 0 if leftVel<0 else 1
        DIRR = 0 if rightVel<0 else 1
        print([(PWML,DIRL), (PWMR, DIRR)])
        return [(PWML,DIRL), (PWMR, DIRR)]

c = cmd_velToPWM()
