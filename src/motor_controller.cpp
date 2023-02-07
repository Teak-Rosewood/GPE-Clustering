#include "ros/ros.h"
#include "geometry_msgs/Twist.h"

//#include <cppgpio.hpp>
// #include <pigpio/pigpio.h>
// #include <pigpio/pigpiod_if2.h>
#include <pigpio/pigpiod_if.h>
#include <iostream>
#include <math.h>
#define wheel_base 0.135
#define wheel_diameter 0.064
#define max_wheel_RPM 1000

class motor_controller 
{
    public:
        int right_dutyCyclePercentage, left_dutyCyclePercentage;
        int direction;
        void vel_callback(const geometry_msgs::Twist::ConstPtr &Twist);

    private:

        float linear_vel, angular_vel, right_vel, left_vel, right_rpm, left_rpm;      
        void wheelVelocity ();
        void setRPM ();
        void settingDutyCycle();
        void setDirection();
};

void motor_controller::vel_callback(const geometry_msgs::Twist::ConstPtr &Twist)
{
    linear_vel = Twist->linear.x;
    angular_vel = Twist->angular.z;

    if(linear_vel > 2.0)
        linear_vel = 2.0;
    if(angular_vel> 1.2)
        angular_vel = 1.2;
    wheelVelocity();
}

void motor_controller::wheelVelocity()
{
    right_vel = abs(linear_vel + angular_vel * wheel_base / 2);
    left_vel = abs(linear_vel - angular_vel * wheel_base / 2);
    setRPM();
    setDirection();
}

void motor_controller::setDirection()
{
    // direction == 1 => forward
    // direction == 2 => backward
    // direction == 3 => left 
    // direction == 4 => right 

    if((linear_vel + angular_vel * wheel_base / 2 ) < 0 && (linear_vel - angular_vel * wheel_base / 2) < 0)
        direction =  2;
    if((linear_vel + angular_vel * wheel_base / 2 ) < 0 &&  (linear_vel - angular_vel * wheel_base / 2) > 0)
        direction =  4; 
    if((linear_vel + angular_vel * wheel_base / 2 ) > 0 &&(linear_vel - angular_vel * wheel_base / 2) < 0)
        direction =  3; 
    if((linear_vel + angular_vel * wheel_base / 2 ) > 0 && (linear_vel - angular_vel * wheel_base / 2) > 0) 
        direction = 1;
}

void motor_controller::setRPM()
{
    right_rpm = right_vel * 60 / (wheel_diameter * M_PI);
    left_rpm = right_vel * 60 / (wheel_diameter * M_PI);

    settingDutyCycle();
}

void motor_controller::settingDutyCycle()
{
    right_dutyCyclePercentage = right_rpm / max_wheel_RPM;
    left_dutyCyclePercentage = left_rpm / max_wheel_RPM;

    right_dutyCyclePercentage = right_dutyCyclePercentage * 255;
    left_dutyCyclePercentage = left_dutyCyclePercentage * 255;
}

int main (int argc, char** argv)
{
    motor_controller motor_control;
    
    ros::init(argc, argv, "motorControllerNode");
    ros::NodeHandle n;
    ros::Rate rate(60);
    
    if (gpioInitialise() < 0)
    {
        fprintf(stderr, "pigpio initialisation failed\n");
        return 1;
    }
    gpioSetMode(13, PI_OUTPUT);

    gpioSetMode(1, PI_OUTPUT);
    gpioSetMode(2, PI_OUTPUT);
    gpioSetMode(0, PI_OUTPUT);
    gpioSetMode(3, PI_OUTPUT);
    // Initiallizing subscribers 

    ros::Subscriber subCMD_VEL = n.subscribe("/cmd_vel", 1, &motor_controller::vel_callback , &motor_control); 

    ros::Duration(0.1).sleep();

    while (ros::ok())
    {
        ros::spinOnce();
        gpioPWM(12 , motor_control.left_dutyCyclePercentage);
        gpioPWM(13 , motor_control.right_dutyCyclePercentage);
    
        if(motor_control.direction == 1)
        {
            gpioWrite(2, 1);
            gpioWrite(3, 0);
            gpioWrite(0, 0);
            gpioWrite(1, 1);
        }
        else if (motor_control.direction == 3)
        {
            gpioWrite(2, 1);
            gpioWrite(3, 0);
            gpioWrite(0, 0);
            gpioWrite(1, 1);
        }
        else if (motor_control.direction == 4)
        {
            gpioWrite(2, 0);
            gpioWrite(3, 1);
            gpioWrite(0, 0);
            gpioWrite(1, 1);
        }
    }

}