#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include "linux_pistorms.h"
#include "linux_pistorms_brick.h"
//#define DBG
#include "linux_pistorms_internal.h"
#include "linux_pistorms_motors.h"


/*
This example is made for tracking objects, remember it is necessary to follow this steps to initialize Pistorms Brick, and I2C device with two first functions*/


#define LED_A BANK_A
#define LED_B BANK_B

#define SENSOR_1 BANK_A_PORT_1
#define SENSOR_2 BANK_A_PORT_2
#define SENSOR_3 BANK_B_PORT_1
#define SENSOR_4 BANK_B_PORT_2



#define MOTOR_1 BANK_B_PORT_1
#define MOTOR_2 BANK_A_PORT_1

#define MOTORS_BANK_B BANK_B
#define MOTORS_BANK_A BANK_A



int value;

int main(){

	printf_dbg("\n\nPASO 1: INIT\n\n");
	value = pistorms_init(1); //Initialize Pistorms

	testBrick();

	testMotors();



	return 0;
}

void testBrick(){
	printf_dbg("\n\nPASO 2\n\n");

	printf_dbg("Batery: %d\n",pistorms_brick_get_battery_voltage());


	printf_dbg("Go Counter: %d\n",pistorms_brick_get_key_press_count());


	printf_dbg("\n\nPASO 3\n\n");
	pistorms_brick_led_On(LED_A,0,255,0); //Green Led
	pistorms_brick_led_On(LED_B,0,255,0);	//Green Led

	sleep(1);
	printf_dbg("\n\nPASO 4\n\n");
	pistorms_brick_led_On(LED_A,255,0,0); //Red Led
	pistorms_brick_led_On(LED_B,255,0,0);	//Red Led

	sleep(1);
	printf_dbg("\n\nPASO 5\n\n");
	pistorms_brick_led_On(LED_A,0,0,255); //Blue Led
	pistorms_brick_led_On(LED_B,0,0,255);	//Blue Led

	sleep(1);
	pistorms_brick_led_Off(LED_A);
	pistorms_brick_led_Off(LED_B);


}

void testMotors(){
	printf_dbg("\n\nPASO 6: RESET MOTORS, init value: %d \n\n", value);

	pistorms_motor_reset_all_parameters(MOTORS_BANK_B);
	pistorms_motor_reset_all_parameters(MOTORS_BANK_A);

	pistorms_motor_reset_pos(MOTOR_1);
	pistorms_motor_reset_pos(MOTOR_2);

	sleep(1);

	printf_dbg("\n\nPASO 7: SET MOTOR 1B SPEED\n\n");
	pistorms_motor_set_speed(MOTOR_1, -100);
	pistorms_motor_set_speed(MOTOR_2, 100);

	pistorms_motor_set_running_time(MOTOR_1, 5);
	pistorms_motor_set_running_time(MOTOR_2, 3);

	printf_dbg("\n\nPASO 8: MOTOR GO\n\n");


	int pos;
	pos = pistorms_motor_go(MOTOR_1 ,TIME_GO);
	pos = pistorms_motor_go(MOTOR_2 ,TIME_GO);

	printf_dbg("Pos at GO: %d\n",pos);

	for(int i=0; i<5; i++){
		pos = pistorms_motor_get_pos(MOTOR_1);
		printf_dbg("Pos Running: %d\n",pos);
		sleep(1);
	}

	pistorms_motor_float_sync(MOTORS_BANK_B);
	pistorms_motor_float_sync(MOTORS_BANK_A);






}


