/**
 * @file main.c
 * @author Santiago Sa�udo Mart�nez
 * @date 11 Apr 2018
 * @brief This is a main program for a balancer robot. Its goal its to make sure it doesn't fall and stays rised.
 * @version 1.0
 *
 */
#include <math.h>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>



#include "linux_pistorms_internal.h"

#include "i2c_layer.h"
#include "linux_pistorms.h"
#include "linux_pistorms_brick.h"
#include "linux_pistorms_motors.h"
#include "linux_pistorms_sensor_ultrasonic.h"
#include "linux_pistorms_sensor_touch.h"
#include "linux_pistorms_sensor_gyro.h"
#include "linux_pistorms_sensor_color.h"
#include "linux_pistorms_camera.h"
#include "timespec_operations.h"




/**
 * Leds
 */
#define LED_A BANK_A
#define LED_B BANK_B
/**
 * Sensores
 */
#define GYRO_ADDR BANK_B_PORT_1
#define TOUCH_ADDR BANK_B_PORT_2
/**
 * Motores
 */
#define MOTOR_1 BANK_B_PORT_1
#define MOTOR_2 BANK_B_PORT_2

#define MOTORS_BANK_B BANK_B



/**
 * Variables globales
 */


int senalActual = 0;
int senalPuntoConsigna = 0;
int senalError = 0;
float constanteProporcional = 2.5;

// Medida del tiempo

int32_t loopTimeMilliSec = 10;



int motor_speed = 0;







// Lecturas del giroscopio y los motores
int i=0;


struct timeval stop, start;

struct timespec fin;


/**
 * Cabaceras de metodos empleados
 */
float proporcional();
float integral();
float derivativo();
void setLedsColor(int red, int green, int blue);
void initBrick();
void initSensors();
void calibrateGyroAndMotorEncoders();
void motorsGo(int speed);
void motorsStop();

int main(){

	printf_debuger("\n\nInit brick\n\n");
	initBrick();
	printf_debuger("\n\nInit sensores\n\n");

	initSensors();
	motorsStop();

	while(!pistorms_is_touched(TOUCH_ADDR)){
		i2c_delay(20);
	}
	struct timespec periodo;
	periodo.tv_sec=0;
	periodo.tv_nsec=loopTimeMilliSec*1000000;  // periodo de 15ms

	struct timespec next_time; // hora de activación

	printf_debuger("\n\nPulsa el sensor tactil para comenzar\n\n");
	while(1){

		calibrateGyroAndMotorEncoders();
		clock_gettime(CLOCK_MONOTONIC, &next_time);
		while(1){
			if(pistorms_is_touched(TOUCH_ADDR)){
				i2c_delay(300);
				break;
			}
			gettimeofday(&start,NULL);
			senalActual = pistorms_gyro_read(GYRO_ADDR, ANGLE);
			senalError = senalPuntoConsigna - senalActual;
			printf_debuger("Señal Consigna: %d, Señal Actual: %d, Señal Error: %d\n", senalPuntoConsigna,senalActual,senalError);

			motor_speed = (proporcional() + integral() + derivativo());

			if(motor_speed > 100)
				motor_speed = 100;
			else if(motor_speed < -100)
				motor_speed = -100;

			motorsGo(motor_speed*-1);

			incr_timespec(&next_time, &periodo);
			clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME,&next_time, NULL);

			gettimeofday(&stop, NULL);
			printf_debuger("Total time = %f miliseconds\n",(double)(stop.tv_usec - start.tv_usec) / 1000);
		}
		motorsStop();
		while(!pistorms_is_touched(TOUCH_ADDR)){
			i2c_delay(20);
		}

	}

}

float proporcional(){
	return senalError * constanteProporcional;
}

float integral(){

	return 0.0;
}

float derivativo(){
	return 0.0;

}

void setLedsColor(int red, int green, int blue){
	pistorms_brick_led_On(LED_A,red,green,blue); //Red Led
	pistorms_brick_led_On(LED_B,red,green,blue);
}

void initBrick(){
	pistorms_init(1);
	//Initialize Pistorms
	setLedsColor(255,0,0); //Red Led
}

void initSensors(){

	int gyroID = 0;
	int touchID = 0;


	gyroID =  pistorms_sensor_gyro_configure(GYRO_ADDR);
	pistorms_gyro_set_mode(GYRO_ADDR, ANGLE);
	printf_debuger("\n\nSensor Giroscopio = %d \n\n",gyroID);


	touchID =  pistorms_sensor_touch_configure(TOUCH_ADDR);
	printf_debuger("\n\nSensor Pulsador = %d \n\n",touchID);

}

void calibrateGyroAndMotorEncoders(){
	setLedsColor(0,255,0);

	int gyroRateCalibrateCount = 1000;
	int i=0;

	pistorms_gyro_set_mode(GYRO_ADDR, ANGLE);

	for(i=0; i < gyroRateCalibrateCount; i++)
	{

		short lectura = pistorms_gyro_read(GYRO_ADDR, ANGLE);
		senalPuntoConsigna = senalPuntoConsigna + lectura;
		printf_debuger("Lecturas Calibrate Angle Mode: %d\n", lectura);

	}
	senalPuntoConsigna = senalPuntoConsigna/gyroRateCalibrateCount;
	printf_debuger("Calibración del gyro. GyroOffset: %d\n", senalPuntoConsigna);
}


void motorsGo(int speed){
	//  Apply the signal to the motor, and add steering
	pistorms_motor_set_speed(MOTOR_1, speed);
	pistorms_motor_set_speed(MOTOR_2, speed);

	pistorms_motor_go(MOTOR_1 ,SPEED_GO);
	pistorms_motor_go(MOTOR_2 ,SPEED_GO);

}

void motorsStop(){
	//  Apply the signal to the motor, and add steering

	pistorms_motor_brake_sync(MOTORS_BANK_B);

	pistorms_motor_reset_all_parameters(MOTORS_BANK_B);
	setLedsColor(255,0,0);
}



