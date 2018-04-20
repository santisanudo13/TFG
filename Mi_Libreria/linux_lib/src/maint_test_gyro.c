/**
 * @file main.c
 * @author Santiago Sa�udo Mart�nez
 * @date 11 Apr 2018
 * @brief This is a main program to test gyro sensor.
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


/**
 * Sensores
 */
#define GYRO_ADDR BANK_B_PORT_2

#define A BANK_A_PORT_2


/**
 * Cabaceras de metodos empleados
 */

void initBrick();
void initSensors();


int main(){

	printf_debuger("\n\nInit brick\n\n");
	initBrick();
	printf_debuger("\n\nInit sensores\n\n");

	initSensors();

	while(1){

		printf("Gyro Rate: %d, ", pistorms_gyro_read(GYRO_ADDR, RATE));
		printf("Gyro Rate: %d\n", pistorms_gyro_read(A, RATE) );
	}
	return (0);

}

void initBrick(){
	pistorms_init(1);
	//Initialize Pistorms
}

void initSensors(){
	int gyroID = 0;
	int a = 0;


	gyroID =  pistorms_sensor_gyro_configure(GYRO_ADDR);
	a =  pistorms_sensor_gyro_configure(A);

//	printf_debuger("\n\nSensor Giroscopio = %d \n\n",gyroID);



}


