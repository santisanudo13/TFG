/**
 * @file main.c
 * @author Santiago Sañudo Martínez
 * @date 5 Mar 2018
 * @brief Small test for all sensors, motors and brick, so we can check everything works properly.
 * @version 1.0
 *
 */

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

#include "linux_pistorms_sensor_ultrasonic.h"
#include "linux_pistorms_sensor_touch.h"
#include "linux_pistorms_sensor_gyro.h"
#include "linux_pistorms_sensor_color.h"



/*
This example is made for tracking objects, remember it is necessary to follow this steps to initialize Pistorms Brick, and I2C device with two first functions*/


#define LED_A BANK_A
#define LED_B BANK_B

#define ULTRASONIC_ADDR BANK_B_PORT_1
#define COLOR_ADDR BANK_B_PORT_2

#define GYRO_ADDR BANK_A_PORT_1
#define TOUCH_ADDR BANK_A_PORT_2



#define MOTOR_1 BANK_B_PORT_1
#define MOTOR_2 BANK_A_PORT_1

#define MOTORS_BANK_B BANK_B
#define MOTORS_BANK_A BANK_A



int value;

void testSensors();
void testMotors();
void testBrick();

int main(){

	printf_debuger("\n\nPASO 1: INIT\n\n");
	value = pistorms_init(1); //Initialize Pistorms

	testBrick();

	testMotors();

	testSensors();


	pistorms_close();

	return 0;
}

void testSensors(){

	int gyroID = 0;
	int touchID = 0;
	int ultraID = 0;
	int colorID = 0;

	gyroID =  pistorms_sensor_gyro_configure(GYRO_ADDR);
	printf_debuger("\n\nGyro = %d \n\n",gyroID);

	ultraID =  pistorms_sensor_ultrasonic_configure(ULTRASONIC_ADDR);
	printf_debuger("\n\nUltra = %d \n\n",ultraID);

	colorID =  pistorms_sensor_color_configure(COLOR_ADDR);
	printf_debuger("\n\nColor = %d \n\n",colorID);

	touchID =  pistorms_sensor_touch_configure(TOUCH_ADDR);
	printf_debuger("\n\nTouch = %d \n\n",touchID);



	sleep(3);

	while(!pistorms_is_touched(TOUCH_ADDR) ){
		printf_debuger("D1: %f, Color: %d, Gyro Angle: %d, Gyro Rate: %d \n", pistorms_ultrasonic_read_distance(ULTRASONIC_ADDR ,PROXIMITY_CENTIMETERS), pistorms_color_measure(COLOR_ADDR), pistorms_gyro_read(GYRO_ADDR,ANGLE), pistorms_gyro_read(GYRO_ADDR,RATE));

	}





}


void testBrick(){
	printf_debuger("\n\nPASO 2\n\n");

	printf_debuger("Firmware Version Bank B: %s\n",pistorms_brick_get_firmware_version(BANK_B));
	printf_debuger("Device ID Bank B: %s\n",pistorms_brick_get_device_id(BANK_B));
	printf_debuger("Vendor ID Bank B: %s\n",pistorms_brick_get_vendor_id(BANK_B));
	printf_debuger("Device ID Bank A: %s\n",pistorms_brick_get_device_id(BANK_A));
	printf_debuger("Vendor ID Bank A: %s\n",pistorms_brick_get_vendor_id(BANK_A));


	printf_debuger("Batery: %d\n",pistorms_brick_get_battery_voltage());


	printf_debuger("Go Counter: %d\n",pistorms_brick_get_key_press_count());


	printf_debuger("\n\nPASO 3: VERDE\n\n");
	pistorms_brick_led_On(LED_A,0,255,0); //Green Led
	pistorms_brick_led_On(LED_B,0,255,0);	//Green Led

	sleep(1);
	printf_debuger("\n\nPASO 4: ROJO\n\n");
	pistorms_brick_led_On(LED_A,255,0,0); //Red Led
	pistorms_brick_led_On(LED_B,255,0,0);	//Red Led

	sleep(1);
	printf_debuger("\n\nPASO 5: AZUL\n\n");
	pistorms_brick_led_On(LED_A,0,0,255); //Blue Led
	pistorms_brick_led_On(LED_B,0,0,255);	//Blue Led

	sleep(1);
	pistorms_brick_led_Off(LED_A);
	pistorms_brick_led_Off(LED_B);


}

void testMotors(){
	printf_debuger("\n\nPASO 6: RESET MOTORS, init value: %d \n\n", value);

	pistorms_motor_reset_all_parameters(MOTORS_BANK_B);
	pistorms_motor_reset_all_parameters(MOTORS_BANK_A);



	sleep(1);

	printf_debuger("\n\nPASO 7: SET MOTOR 1B SPEED\n\n");
	pistorms_motor_set_speed(MOTOR_1, 100);
	pistorms_motor_set_speed(MOTOR_2, 100);

	pistorms_motor_set_running_time(MOTOR_1, 5);
	pistorms_motor_set_running_time(MOTOR_2, 5);

	printf_debuger("\n\nPASO 8: MOTOR GO\n\n");

	pistorms_motor_go(MOTOR_1 ,TIME_GO);
	pistorms_motor_go(MOTOR_2 ,TIME_GO);

	for(int i=0; i<5; i++){
		printf_debuger("RUNNING, Pos1: %ld, Pos2: %ld\n",pistorms_motor_get_pos(MOTOR_1), pistorms_motor_get_pos(MOTOR_2));
		sleep(1);
	}

	pistorms_motor_float_sync(MOTORS_BANK_B);
	pistorms_motor_float_sync(MOTORS_BANK_A);






}


