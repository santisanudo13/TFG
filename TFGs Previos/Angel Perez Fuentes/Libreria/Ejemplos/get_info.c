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

	char version[9];
	char vendor[9];
	char device[9];

	camera_software_version(version);

	camera_vendor_id(vendor);
	
	camera_device_id(device);
	

	printf("Software version: %s\n",version);
	bcm2835_delay(500);
	printf("Vendor ID: %s\n",vendor);
	bcm2835_delay(500);
	printf("Device ID: %s\n",device);
	bcm2835_delay(500);

	return 0;
}
