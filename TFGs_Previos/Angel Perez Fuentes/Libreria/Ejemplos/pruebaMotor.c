#include "marte_pistorms_sensor_touch.h"
#include "marte_pistorms_sensor_ultrasonic.h"
#include "marte_pistorms_brick.h"
#include "marte_pistorms_motors.h"
#include "marte_pistorms.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>



/*
This example is made for tracking objects, remember it is necessary to follow this steps to initialize Pistorms Brick, and I2C device with two first functions*/


#define MOTOR_1 BANK_B_PORT_1
#define MOTOR_2 BANK_B_PORT_2

#define MOTORS_BANK_B BANK_B
#define MAX_POSITION -557

int main(){
	
	
	pistorms_init(); //Initialize Pistorms
		
	pistorms_motor_reset_all_parameters(MOTORS_BANK_B);

	pistorms_motor_reset_pos(MOTOR_1);
	
    
  
    	pistorms_motor_set_speed(MOTOR_1,-13);//Negative speed is due to the position of the motor.
   
	pistorms_motor_set_pos(MOTOR_1,-200);
    
	pistorms_motor_go(MOTOR_1 ,ENCODER_GO | ENCODER_ACTIVE_FEEDBACK);
    	
	bcm2835_delay(300);
   	pistorms_motor_set_pos(MOTOR_1,-275);
	
	//pistorms_motor_go(MOTOR_1 ,ENCODER_GO | ENCODER_ACTIVE_FEEDBACK);

	bcm2835_delay(300);
   	pistorms_motor_set_pos(MOTOR_1,-100);
	
	//pistorms_motor_go(MOTOR_1 ,ENCODER_GO | ENCODER_ACTIVE_FEEDBACK);

	bcm2835_delay(300);
   	pistorms_motor_set_pos(MOTOR_1,-400);
	
	//pistorms_motor_go(MOTOR_1 ,ENCODER_GO | ENCODER_ACTIVE_FEEDBACK);

	bcm2835_delay(300);
   	pistorms_motor_set_pos(MOTOR_1,-450);
	
	pistorms_motor_go(MOTOR_1 ,ENCODER_GO | ENCODER_ACTIVE_FEEDBACK);

	bcm2835_delay(300);
   	pistorms_motor_set_pos(MOTOR_1,0);
	
	pistorms_motor_go(MOTOR_1 ,ENCODER_GO | ENCODER_ACTIVE_FEEDBACK);
	/*
	if((pistorms_motor_get_pos(MOTOR_1)) < pos_deseada){

		pistorms_motor_set_speed(MOTOR_1,15);
	}else{
		pistorms_motor_set_speed(MOTOR_1,-15);
	}

	pistorms_motor_go(MOTOR_1 ,SPEED_GO);

	while((pistorms_motor_get_pos(MOTOR_1)) != pos_deseada)
		;
	pistorms_motor_brake(MOTOR_1);*/

	

	
	
	sleep(1);
	

	
	

	return 0;
}
