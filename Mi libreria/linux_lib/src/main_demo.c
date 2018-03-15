/**
 * @file main.c
 * @author Santiago Sa�udo Mart�nez
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

#define LED_A BANK_A
#define LED_B BANK_B

#define ULTRASONIC_1_ADDR BANK_A_PORT_1
#define ULTRASONIC_2_ADDR BANK_A_PORT_2
#define TOUCH_ADDR BANK_B_PORT_1


#define MOTOR_1 BANK_A_PORT_2
#define MOTOR_2 BANK_A_PORT_1
#define MOTOR_3 BANK_B_PORT_2

#define MOTORS_BANK_A BANK_A
#define MOTORS_BANK_B BANK_B



void rotateSensor();
void decideSide();
void initBrick();
int initMotors();
void motorsBrake();
void motorsRotate();
void motorsForward();
void initSensors();


int ladoMotor = 0;// 0 = derecha, 1 = izquierda
int ladoRotacion = 0;// 0 = izquierda, 1 = derecha

float* buffer;

int main(){

	printf_debuger("\n\nPASO 1: INIT BRICK\n\n");
	initBrick();

	initSensors();

	pistorms_motor_reset_all_parameters(MOTORS_BANK_A);

	while(1){
		printf_debuger("\n\nPASO 2: ESPERANDO A QUE SE PULSE GO\n\n");
		while(!pistorms_brick_get_key_press_value()){
			i2c_delay(50);
		}

		printf_debuger("\n\nPASO 4: GO MOTORS!\n\n");
		initMotors();



		motorsBrake();
	}

}

void initBrick(){
	pistorms_init(1); //Initialize Pistorms
	pistorms_brick_led_On(LED_A,255,0,0); //Red Led
	pistorms_brick_led_On(LED_B,255,0,0);	//Red Led
}

void initSensors(){

	int touchID = 0;
	int ultraID_1 = 0;
	int ultraID_2 = 0;

	ultraID_1 =  pistorms_sensor_ultrasonic_configure(ULTRASONIC_1_ADDR);
	printf_debuger("\n\nUltra = %d \n\n",ultraID_1);

	ultraID_2 =  pistorms_sensor_ultrasonic_configure(ULTRASONIC_2_ADDR);
	printf_debuger("\n\nUltra = %d \n\n",ultraID_2);


	touchID =  pistorms_sensor_touch_configure(TOUCH_ADDR);
	printf_debuger("\n\nTouch = %d \n\n",touchID);

}

int initMotors(){
	//Avanzamos hacia delante con ambos motores al 100% de capacidad.
	motorsForward();



	//Nos metemos en un bucle infinito a menos que pulsemos el boton go o el sensor de tacto
	while(1){
		//Seguimos avanzando hasta que la distancia sea menor de 15cm.
		while(pistorms_ultrasonic_read_distance(ULTRASONIC_1_ADDR ,PROXIMITY_CENTIMETERS) > 15.0 ){
			if(pistorms_is_touched(TOUCH_ADDR))
				return 1;
		}

		rotateSensor();
		motorsRotate();

		while(pistorms_ultrasonic_read_distance(ULTRASONIC_1_ADDR ,PROXIMITY_CENTIMETERS) < 15.0 ){
			if(pistorms_is_touched(TOUCH_ADDR))
				return 1;
		}

		//		//Avanzamos hacia delante con ambos motores al 100% de capacidad.
		motorsForward();
	}

}

/**
 * Configuramos los motores y los inicializamos de manera que el vehiculo comience a rotar sobre si mismo.
 */
void motorsRotate(){

	pistorms_brick_led_On(LED_A,0,0,255); //Blue Led
	pistorms_brick_led_On(LED_B,0,0,255);	//Blue Led

	pistorms_motor_reset_all_parameters(MOTORS_BANK_A);

	if(ladoMotor == 1){
		printf_debuger("Rotamos Izquierda\n");

		pistorms_motor_set_speed(MOTOR_1, -70);
		pistorms_motor_set_speed(MOTOR_2, 70);
	}
	if(ladoMotor == 0){
		printf_debuger("Rotamos Derecha\n");

		pistorms_motor_set_speed(MOTOR_1, 70);
		pistorms_motor_set_speed(MOTOR_2, -70);
	}

	pistorms_motor_go(MOTOR_1 ,SPEED_GO);
	pistorms_motor_go(MOTOR_2 ,SPEED_GO);

}

/**
 * Configuramos los motores y los inicializamos de manera que el vehiculo avance hacia delante con la m�xima velocidad posible.
 */
void motorsForward(){
	pistorms_brick_led_On(LED_A,0,255,0); //Green Led
	pistorms_brick_led_On(LED_B,0,255,0);	//Green Led

	pistorms_motor_reset_all_parameters(MOTORS_BANK_A);

	pistorms_motor_set_speed(MOTOR_1, 80);
	pistorms_motor_set_speed(MOTOR_2, 80);

	pistorms_motor_go(MOTOR_1 ,SPEED_GO);
	pistorms_motor_go(MOTOR_2 ,SPEED_GO);

}

void motorsBrake(){
	pistorms_brick_led_On(LED_A,255,0,0); //Green Led
	pistorms_brick_led_On(LED_B,255,0,0);	//Green Led

	pistorms_motor_reset_all_parameters(MOTORS_BANK_A);

	pistorms_motor_set_speed(MOTOR_1, 0);
	pistorms_motor_set_speed(MOTOR_2, 0);

	pistorms_motor_go(MOTOR_1 ,SPEED_GO);
	pistorms_motor_go(MOTOR_2 ,SPEED_GO);

}



void rotateSensor(){
	motorsBrake();


	pistorms_brick_led_On(LED_A,255,0,255); //Purple Led
	pistorms_brick_led_On(LED_B,255,0,255); //Purple Led



	pistorms_motor_set_speed(MOTOR_3, -25);


	pistorms_motor_set_running_time(MOTOR_3, 2);

	pistorms_motor_go(MOTOR_3 ,TIME_GO);


	decideSide();

	pistorms_motor_reset_all_parameters(MOTORS_BANK_A);
	pistorms_motor_reset_all_parameters(MOTORS_BANK_B);

	pistorms_motor_set_speed(MOTOR_3, 22);

	pistorms_motor_set_running_time(MOTOR_3, 2);

	pistorms_motor_go(MOTOR_3 ,TIME_GO);

	i2c_delay(2000);
}

void decideSide(){
	buffer = malloc(sizeof(float)*100 + 1);

	for(int i=0; i<100; i++){
		buffer[i] = pistorms_ultrasonic_read_distance(ULTRASONIC_2_ADDR ,PROXIMITY_CENTIMETERS);
	}

	float upLeft = 0.0;
	float upRight = 0.0;
	float downRight = 0.0;
	float downLeft = 0.0;

	for(int i=0; i<100; i++){
		//Rotacion ha sido hacia la derecha

		if(i >= 0 && i < 25){
			upRight = upRight + buffer[i];
		}

		if(i >= 25 && i < 50){
			downRight = downRight + buffer[i];
		}

		if(i >= 50 && i < 75){
			downLeft = downLeft + buffer[i];
		}

		if(i >= 75 && i < 100){
			upLeft = upLeft + buffer[i];
		}
	}
	upLeft = upLeft/25.0;
	upRight = upRight/25.0;
	downRight = downRight/25.0;
	downLeft = downLeft/25.0;


	printf_debuger("UpL: %f, UpR: %f, DwL: %f, DwR: %f \n", upLeft,upRight,downLeft,downRight);

	float upDiff = upRight - upLeft;
	//Si la diferencia es superior a 5 porun lado, se gira hacia el lado que es mas pequeño, siendo 1 derecha y 0 izquierda. Si no es superior a 5º la diferencia se comprueba con la zona inferior
	//y si tampoco lo es directamente se deja como estaba.

	int modificado = 0;
	if(upDiff < 0){
		upDiff = upDiff * -1;
		if(upDiff >5){
			ladoMotor = 1;
			modificado = 1;
		}
	}else{
		if(upDiff >5){
			ladoMotor = 0;
			modificado = 1;
		}
	}
	if(!modificado){
		float downDiff = downRight - downLeft;


		if(downDiff < 0){
			downDiff = downDiff * -1;
			if(downDiff >5){
				ladoMotor = 1;
				modificado = 1;
			}
		}else{
			if(downDiff >5){
				ladoMotor = 0;
				modificado = 1;
			}
		}
	}

}
