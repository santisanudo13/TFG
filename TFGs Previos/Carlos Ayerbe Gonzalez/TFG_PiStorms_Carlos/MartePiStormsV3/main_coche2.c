#include "marte_pistorms_sensor_touch.h"
#include "marte_pistorms_sensor_ultrasonic.h"
#include "marte_pistorms_brick.h"
#include "marte_pistorms_motors.h"
#include "marte_pistorms.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


#define ULTRASONIC_ADDR BANK_B_PORT_1 
#define TOUCH_ADDR BANK_B_PORT_2

#define LED_A BANK_A
#define LED_B BANK_B

#define MOTOR_1 BANK_B_PORT_1
#define MOTOR_2 BANK_B_PORT_2

#define MOTORS_BANK_B BANK_B


int main(int argc, char **argv) {
  
  int ultraID,touchID;
  
  pistorms_init();
  
  ultraID =  pistorms_sensor_ultrasonic_configure(ULTRASONIC_ADDR );
  //printf("Ultra =%d ",ultraID);
  
  touchID =  pistorms_sensor_touch_configure(TOUCH_ADDR );
  //printf("Touch =%d ",touchID);
  
  pistorms_brick_led_On(LED_A,0,255,0); //Green Led
  pistorms_brick_led_On(LED_B,0,255,0);	//Green Led
  
  pistorms_motor_reset_all_parameters(MOTORS_BANK_B);
  sleep(1);
  
  while(!pistorms_brick_get_key_press_value()){
    
  
    pistorms_motor_set_speed(MOTOR_1,-50);//Negative speed is due to the position of the motor.
    pistorms_motor_set_speed(MOTOR_2,-50);
    
    pistorms_motor_go(MOTOR_1 ,SPEED_GO);
    pistorms_motor_go(MOTOR_2 ,SPEED_GO);
    
    if((pistorms_ultrasonic_read_distance(ULTRASONIC_ADDR ,CENTIMETERS) < 6.0) ||( pistorms_is_touched(TOUCH_ADDR ))){
      
      pistorms_motor_brake_sync(MOTORS_BANK_B);
      sleep(1);
      
      pistorms_motor_reset_pos(MOTOR_1);
      pistorms_motor_reset_pos(MOTOR_2);
        
      pistorms_motor_set_speed(MOTOR_1,40);
      pistorms_motor_set_speed(MOTOR_2,40);
      pistorms_motor_set_running_time(MOTOR_1,2);
      pistorms_motor_set_running_time(MOTOR_2,2);
     
      
      pistorms_motor_go(MOTOR_1 ,SPEED_GO | TIME_GO);
      pistorms_motor_go(MOTOR_2 ,SPEED_GO | TIME_GO);
      
      sleep(3);//Waiting three seconds for the movement of the car.
      
      pistorms_motor_brake_sync(MOTORS_BANK_B);
      //sleep(1);
      
      pistorms_motor_reset_pos(MOTOR_1);
      pistorms_motor_reset_pos(MOTOR_2);
      
      pistorms_motor_set_speed(MOTOR_1,-30);
      pistorms_motor_set_speed(MOTOR_2,30);
      pistorms_motor_set_running_time(MOTOR_1,2);
      pistorms_motor_set_running_time(MOTOR_2,2);
      
      pistorms_motor_go(MOTOR_1 ,SPEED_GO | TIME_GO);
      pistorms_motor_go(MOTOR_2 ,SPEED_GO | TIME_GO);
      
      sleep(2);//Waiting two seconds for the movement of the car.
       
    }
    
  }  
  
  //Button GO has been touched.
  
  pistorms_brick_led_On(LED_A,255,0,0);//Red Led
  pistorms_brick_led_On(LED_B,255,0,0);//Red Led
  
  pistorms_motor_brake_sync(MOTORS_BANK_B);
  pistorms_motor_reset_all_parameters(MOTORS_BANK_B);
  
  pistorms_close();
  
}