#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "marte_pistorms.h"
#include "marte_pistorms_motors.h"
#include "marte_pistorms_brick.h"
#include <unistd.h>

/*
This example is made for tracking objects, remember it is necessary to follow this steps to initialize Pistorms Brick, and I2C device with two first functions*/

#define MOTOR_1 BANK_B_PORT_1
#define MOTOR_2 BANK_B_PORT_2

#define MOTORS_BANK_B BANK_B
#define MAX_POSITION -557

int main(){

	puts("\n\nPASO 1: INIT\n\n");
	int value = pistorms_init(1); //Initialize Pistorms
	printf("\n\nPASO 2: RESET MOTORS, init value: %d \n\n", value);

	pistorms_motor_reset_all_parameters(MOTORS_BANK_B);
	sleep(3);

	puts("\n\nPASO 3: SET MOTOR 1B SPEED\n\n");

	pistorms_motor_set_speed(MOTOR_1,100);

	puts("\n\nPASO 4: MOTOR GO\n\n");


	pistorms_motor_go(MOTOR_1 ,ENCODER_GO | ENCODER_ACTIVE_FEEDBACK);

	sleep(10);


	sleep(1);





	return 0;
}
