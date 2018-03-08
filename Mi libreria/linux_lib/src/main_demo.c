/**
 * @file main.c
 * @author Santiago Saï¿½udo Martï¿½nez
 * @date 5 Mar 2018
 * @brief This is a demo of a car working properly
 * @version 1.0
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <cstdlib>

#include "linux_pistorms.h"
#include "linux_pistorms_brick.h"
#include "linux_pistorms_internal.h"
#include "linux_pistorms_motors.h"

#include "linux_pistorms_sensor_ultrasonic.h"
#include "linux_pistorms_sensor_touch.h"
#include "linux_pistorms_sensor_gyro.h"
#include "linux_pistorms_sensor_color.h"



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




void initSensors();
void initBrick();
void initMotors();
void motorsRotate();
void motorsForward();
void motorsBrake();




int main(){

	printf_debuger("\n\nPASO 1: INIT BRICK\n\n");
	initBrick();


	printf_debuger("\n\nPASO 2: ESPERANDO A QUE SE PULSE GO\n\n");
	while(!pistorms_brick_get_key_press_value()){
		i2c_delay(200);
	}
	printf_debuger("\n\nPASO 3: INIT SENSORS\n\n");
	initSensors();

	printf_debuger("\n\nPASO 4: GO MOTORS!\n\n");
	initMotors();



	return 0;
}

void initSensors(){

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
}


void initBrick(){
	value = pistorms_init(1); //Initialize Pistorms
	pistorms_brick_led_On(LED_A,255,0,0); //Red Led
	pistorms_brick_led_On(LED_B,255,0,0);	//Red Led


}

void initMotors(){


	//Avanzamos hacia delante con ambos motores al 100% de capacidad.
	motorsForward();

	//Nos metemos en un bucle infinito a menos que pulsemos el boton go o el sensor de tacto
	while(!pistorms_brick_get_key_press_value() || !pistorms_is_touched(TOUCH_ADDR)){
		//Seguimos avanzando hasta que la distancia sea menor de 15cm.
		while(pistorms_ultrasonic_read_distance(ULTRASONIC_ADDR ,PROXIMITY_CENTIMETERS) > 15.0){
			i2c_delay(50);
		}
		// Frenamos y esperamos a que frenen los motores
		motorsBrake();
		i2c_delay(100);

		//Comenzamos ha girar el vehiculo
		int angle = pistorms_gyro_read(GYRO_ADDR,RATE);
		motorsRotate();

		//Calculamos cuando hemos girado 45 grados
		while(std::abs(angle-pistorms_gyro_read(GYRO_ADDR,RATE)) < 45){
			i2c_delay(50);
		}

		// Frenamos y esperamos a que frenen los motores
		motorsBrake();
		i2c_delay(100);

		//Avanzamos hacia delante con ambos motores al 100% de capacidad.
		motorsForward();
	}









}
/**
 * Configuramos los motores y los inicializamos de manera que el vehiculo comience a rotar sobre si mismo.
 */
void motorsRotate(){
	pistorms_brick_led_On(LED_A,0,0,255); //Blue Led
	pistorms_brick_led_On(LED_B,0,0,255);	//Blue Led

	pistorms_motor_reset_all_parameters(MOTORS_BANK_B);
	pistorms_motor_reset_all_parameters(MOTORS_BANK_A);

	pistorms_motor_set_speed(MOTOR_1, -50);
	pistorms_motor_set_speed(MOTOR_2, 50);

	pistorms_motor_go(MOTOR_1 ,SPEED_GO);
	pistorms_motor_go(MOTOR_2 ,SPEED_GO);
}

/**
 * Configuramos los motores y los inicializamos de manera que el vehiculo avance hacia delante con la máxima velocidad posible.
 */
void motorsForward(){
	pistorms_brick_led_On(LED_A,0,255,0); //Green Led
	pistorms_brick_led_On(LED_B,0,255,0);	//Green Led

	pistorms_motor_reset_all_parameters(MOTORS_BANK_B);
	pistorms_motor_reset_all_parameters(MOTORS_BANK_A);

	pistorms_motor_set_speed(MOTOR_1, 100);
	pistorms_motor_set_speed(MOTOR_2, 100);

	pistorms_motor_go(MOTOR_1 ,SPEED_GO);
	pistorms_motor_go(MOTOR_2 ,SPEED_GO);
}

/**
 * Hacemos que todos los motores se paren simultaneamente.
 */
void motorsBrake(){
	pistorms_motor_brake_sync(MOTORS_BANK_B);
	pistorms_motor_brake_sync(MOTORS_BANK_A);
}
