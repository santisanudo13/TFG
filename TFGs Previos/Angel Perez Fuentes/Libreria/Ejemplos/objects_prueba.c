#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "marte_pistorms_camera.h"
#include "marte_pistorms.h"
#include "marte_pistorms_sensors.h"
#include <unistd.h>
#include "bcm2835.h"

/*
This example is made for tracking objects, remember it is necessary to follow this steps to initialize Pistorms Brick, and I2C device with two first functions*/


int main(){
	
	
	pistorms_init(); //Initialize Pistorms
	camera_init(1,4); //Initialize I2C Device

	camera_start_tracking(); //Send start track command to camera

	int obj;
	object_properties_t st;
	int i;

	for(i=0;i<3;i++){

		obj = camera_objects_detected();
		printf("Objetos %d\n",obj);

		if(obj > 0){

			camera_object_coordinates(0, &st);
			printf("x_upper_left: %d\n",st.x_upper_left);
			printf("y_upper_left: %d\n",st.y_upper_left);
			printf("x_lower_right: %d\n",st.x_lower_right);
			printf("x_lower_right: %d\n",st.y_lower_right);
			
		}
		bcm2835_delay(500);
	}	


	
	return 0;
}
