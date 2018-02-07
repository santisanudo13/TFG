#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "marte_pistorms_camera.h"
#include "marte_pistorms.h"
#include "marte_pistorms_sensors.h"
#include "marte_pistorms_motors.h"
#include "marte_pistorms_brick.h"
#include <unistd.h>
#include "bcm2835.h"

/*
This example is made for tracking objects, remember it is necessary to follow this steps to initialize Pistorms Brick, and I2C device with two first functions*/

#define MOTOR_1 BANK_B_PORT_1
#define MOTOR_2 BANK_B_PORT_2

#define MOTORS_BANK_B BANK_B
#define MAX_POSITION 557
#define SPEED 80
#define CENTER 87
#define NUM_MEDIDAS 5
#define RATIO 7.42
#define MARGEN_MOVIMIENTO 10
int main(){
	
	
	pistorms_init(); //Initialize Pistorms

	
	pistorms_motor_reset_all_parameters(MOTORS_BANK_B);
	pistorms_motor_set_speed(MOTOR_1,SPEED);

	camera_init(1,4); //Initialize I2C Device

	camera_start_tracking(); //Send start track command to camera



	
	object_properties_t st;
	float pos,temp;
	int i,j;
	while(1){
		temp=0;
		j=0;
		camera_set_as_active_device();
		for(i=0;i<NUM_MEDIDAS;i++){

			if(camera_objects_detected() > 0){
				camera_object_coordinates(0, &st);
				temp +=st.y_upper_left;
				bcm2835_delay(1);
				j++;
			}
		}
		
			if(j > 0){
				temp = (temp * RATIO/ j);
		
		
		
				if(temp >= MAX_POSITION)
					temp = MAX_POSITION + CENTER;

				if(temp <= CENTER)
					temp = CENTER;

				pos = (int)(temp - CENTER);
				
		
				pistorms_motor_set_pos(MOTOR_1,pos);
				pistorms_motor_go(MOTOR_1 ,ENCODER_GO | ENCODER_ACTIVE_FEEDBACK | SPEED_GO);
			}

			
	bcm2835_delay(40);
		
	}

	

	
	return 0;
}
