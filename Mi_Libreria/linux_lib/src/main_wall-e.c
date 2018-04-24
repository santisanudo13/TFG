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

/**
 * Leds
 */
#define LED_A BANK_A
#define LED_B BANK_B
/**
 * Sensores
 */
#define GYRO_ADDR BANK_B_PORT_2
#define TOUCH_ADDR BANK_B_PORT_1
/**
 * Motores
 */
#define MOTOR_1 BANK_B_PORT_1
#define MOTOR_2 BANK_B_PORT_2

#define MOTORS_BANK_B BANK_B



/**
 * Variables globales
 */




// Medida del tiempo

int32_t loopTimeMilliSec = 15;

float loopTimeSec = 0;

time_t tLoopStart, tLoopEnd;

int motor_speed = 20;

// Radianes por grado
float radiansPerDegree = 0;

// Grados/s por unidad del giroscopio
int32_t degPerSecondPerRawGyroUnit = 1;

// Radianes/s por unidad del giroscopio
float radiansPerSecondPerRawGyroUnit = 0;
// Grados por unidad de motor
int32_t degPerRawMotorUnit = 1;

// Radianes por unidad de motor
float radiansPerRawMotorUnit = 0;

// RPM por unidad de motor
float RPMperPerRawMotorUnit = 0;

// RPM por velocidad
float RPMperPerPercentSpeed = 1.7;

// Grados/s por velocidad
float degPersecPerPercentSpeed = 0;

// Radianes/s por velocidad
float radPerSecPerPercentSpeed = 0;

// Velocidad a la que compensamos el offset del giroscopio
float gyroDriftCompensationRate = 0;

// Array que guarda las posiciones del motor anteriores
float motorAngleHistory[3];

// Ganancias de control
int32_t gainGyroAngle=1156, gainGyroRate=146, gainMotorAngle=7, gainMotorAngularSpeed=9, gainMotorAngleErrorAccumulated=3;
//float gainGyroAngle=15, gainGyroRate=0.8, gainMotorAngle=0.12, gainMotorAngularSpeed=0.08, gainMotorAngleErrorAccumulated=3;

// Señal del ángulo del motor (en grados)
float motorAngleRaw = 0;

// El ángulo del motor convertido a radianes
float motorAngle = 0;

// El ángulo del motor de referencia. Que el robot tiende a conseguir
float motorAngleReference = 0;

// Desviación de la medida del ángulo del motor con respecto al ángulo del motor de referencia
float motorAngleError = 0;

// Error acumulado
float motorAngleErrorAccumulated = 0;

// La velocidad del motor, estimada por cuanto ha girado en el tiempo transcurrido
float motorAngularSpeed = 0;

// Velocidad de referencia
float motorAngularSpeedReference = 0;

// Desviación de la velocidad del motor respecto a la referencia
float motorAngularSpeedError = 0;

// La señal de "voltaje" que enviamos al motor
float motorDutyCycle = 0;

// El valor del giroscopio en rate mode
int32_t gyroRateRaw = 0;

// La velocidad angular del robot medida en radianes/s
float gyroRate = 0;

// Offset del giroscopio
float gyroOffset = 0;

float diff;

// Ángulo estimado por el valor de la velocidad angular del giroscopio y el tiempo transcurrido
float gyroEstimatedAngle = 0;

// Lecturas del giroscopio y los motores
int i=0;


struct timeval stop, start;

struct timespec fin;


/**
 * Cabaceras de metodos empleados
 */
void test();
void calibrateVariables();
void setLedsColor(int red, int green, int blue);
void initBrick();
void initSensors();
void calibrateGyroAndMotorEncoders();
void readGyroAndMotors();
void saveMotorSpeed();
void calculateMotorSpeed();
void motorsGo();
void updateGyroOffset();
void updateAcumulatedMotorError();
void motorsStop();

int main(){
	calibrateVariables();

	//	printf_debuger("\n\nInit brick\n\n");
	initBrick();
	//	printf_debuger("\n\nInit sensores\n\n");

	initSensors();

	pistorms_motor_reset_all_parameters(MOTORS_BANK_B);


	//	printf_debuger("\n\nPulsa el sensor tactil para comenzar\n\n");
	while(!pistorms_is_touched(TOUCH_ADDR)){
		i2c_delay(80);
	}


	//	test();

	while(1){
		calibrateGyroAndMotorEncoders();
		while(1){
			//Hacemos que cada ciclo dure 15ms desde que comienza hasta que termine realizando un sleep relativo.
			//Con un baudrate de 50000 la raspberry, cada iteracion sin sleep son unos 6-9ms

			clock_gettime(CLOCK_MONOTONIC, &fin);
			fin.tv_nsec += 15000000;

			if(pistorms_is_touched(TOUCH_ADDR)){
				i2c_delay(300);
				break;
			}

			//Comienza el bucle
			time(&tLoopStart);

			readGyroAndMotors();
			saveMotorSpeed();
			calculateMotorSpeed();
			motorsGo();
			updateGyroOffset();
			updateAcumulatedMotorError();

			time(&tLoopEnd);
			clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &fin, NULL);
		}

		motorsStop();

		while(!pistorms_is_touched(TOUCH_ADDR)){
			i2c_delay(20);
		}
	}
	return (0);
}

void test(){
	pistorms_motor_reset_all_parameters(BANK_B);
	pistorms_motor_reset_pos(MOTOR_1);


	long pos = pistorms_motor_get_pos(MOTOR_1);
	//	printf_debuger("POS: %ld\n",pos);


	pistorms_motor_set_speed(MOTOR_1, 5);
	pistorms_motor_go(MOTOR_1 ,SPEED_GO);

	while(1){
		pos = pistorms_motor_get_pos(MOTOR_1);
		//		printf_debuger("POS: %ld\n",pos);

	}

}

void calibrateVariables(){
	// Medida del tiempo

	loopTimeSec = loopTimeMilliSec/1000.0;

	// Radianes por grado
	radiansPerDegree = M_PI/180;

	// Radianes/s por unidad del giroscopio
	radiansPerSecondPerRawGyroUnit = degPerSecondPerRawGyroUnit * radiansPerDegree;

	// Radianes por unidad de motor
	radiansPerRawMotorUnit = degPerRawMotorUnit * radiansPerDegree;

	// RPM por unidad de motor
	RPMperPerRawMotorUnit = degPerRawMotorUnit * radiansPerDegree;

	// Grados/s por velocidad
	degPersecPerPercentSpeed = RPMperPerPercentSpeed * 360/60;

	// Radianes/s por velocidad
	radPerSecPerPercentSpeed = degPersecPerPercentSpeed * radiansPerDegree;

	// Velocidad a la que compensamos el offset del giroscopio
	gyroDriftCompensationRate = 0.1 * loopTimeSec * radiansPerSecondPerRawGyroUnit;
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

	int touchID = 0;
	int gyroID = 0;

	gyroID =  pistorms_sensor_gyro_configure(GYRO_ADDR);
	//	printf_debuger("\n\nSensor Giroscopio = %d \n\n",gyroID);

	touchID =  pistorms_sensor_touch_configure(TOUCH_ADDR);
	//	printf_debuger("\n\nSensor Pulsador = %d \n\n",touchID);
}

void calibrateGyroAndMotorEncoders(){
	setLedsColor(255,255,0);

	pistorms_motor_reset_pos(MOTOR_1);
	pistorms_motor_reset_pos(MOTOR_2);

	int gyroRateCalibrateCount = 3000;
	int i=0;

	pistorms_gyro_set_mode(GYRO_ADDR, RATE);

	for(i=0; i < gyroRateCalibrateCount; i++)
	{

		short lectura = pistorms_gyro_read(GYRO_ADDR, RATE);
		gyroOffset = gyroOffset + lectura;
				printf_debuger("Lecturas Calibrate: %d\n", lectura);

	}
	gyroOffset = gyroOffset/gyroRateCalibrateCount;
	//	printf_debuger("Calibración del gyro. GyroOffset: %f\n", gyroOffset);
}

void readGyroAndMotors(){
	// Lectura de gyro
	gyroRateRaw = pistorms_gyro_read(GYRO_ADDR, RATE);

	//	printf_debuger("\nGyro Rate Raw: %d\n", gyroRateRaw);

	gyroRate = (gyroRateRaw - gyroOffset) * radiansPerSecondPerRawGyroUnit;

	// Lectura de la posición del motor
	long pos1 = pistorms_motor_get_pos(MOTOR_1);
	long pos2 = pistorms_motor_get_pos(MOTOR_2);

	motorAngleRaw = (pos1 + pos2)/2;
	//	printf_debuger("Motor pos1: %ld, Motors pos2: %ld\n", pos1, pos2);
	//	printf_debuger("Motor Angle Raw: %f\n", motorAngleRaw);

	motorAngle = motorAngleRaw * radiansPerRawMotorUnit;

	motorAngularSpeedReference = 0 * radPerSecPerPercentSpeed;
	motorAngleReference = motorAngleReference + motorAngularSpeedReference * loopTimeSec;

	motorAngleError = motorAngle - motorAngleReference;
}

void saveMotorSpeed(){
	//  Computing Motor Speed
	motorAngularSpeed = (motorAngle - motorAngleHistory[0])/(3 * loopTimeSec);
	motorAngularSpeedError = motorAngularSpeed - motorAngularSpeedReference;

	motorAngleHistory[0] = motorAngleHistory[1];
	motorAngleHistory[1] = motorAngleHistory[2];
	motorAngleHistory[2] = motorAngle;
}

void calculateMotorSpeed(){
		printf_debuger("---------------------\nGyro Estimated Angle: %f\nGyro Rate: %f\nMotor Angle Error: %f\nMotor Angular Speed: %f\nMotor Angle Error Accumulated: %f\n---------------------\n", gyroEstimatedAngle, gyroRate, motorAngleError, motorAngularSpeedError, motorAngleErrorAccumulated);

	//  Computing the motor duty cycle value
	motorDutyCycle =(gainGyroAngle  					* gyroEstimatedAngle
			+ gainGyroRate   					* gyroRate
			+ gainMotorAngle 					* motorAngleError
			+ gainMotorAngularSpeed 			* motorAngularSpeedError
			+ gainMotorAngleErrorAccumulated 	* motorAngleErrorAccumulated);

	if(motorDutyCycle > 100)
		motorDutyCycle = 100;
	else if(motorDutyCycle < -100)
		motorDutyCycle = -100;


	//	printf_debuger("Potencia Motores: %d\n", (int)motorDutyCycle);

}

void motorsGo(){
	//  Apply the signal to the motor, and add steering


	pistorms_motor_set_speed(MOTOR_1, motorDutyCycle);
	pistorms_motor_set_speed(MOTOR_2, motorDutyCycle);

	pistorms_motor_go(MOTOR_1 ,SPEED_GO);
	pistorms_motor_go(MOTOR_2 ,SPEED_GO);


}

void motorsStop(){
	//  Apply the signal to the motor, and add steering

	pistorms_motor_brake_sync(MOTORS_BANK_B);

	pistorms_motor_reset_all_parameters(MOTORS_BANK_B);
	setLedsColor(255,0,0);

}

void updateGyroOffset(){
	//  Update angle estimate and Gyro Offset Estimate
	gyroEstimatedAngle = gyroEstimatedAngle + gyroRate * loopTimeSec;
	gyroOffset = (1 - gyroDriftCompensationRate) * gyroOffset + gyroDriftCompensationRate * gyroRateRaw;
}

void updateAcumulatedMotorError(){
	//  Update Accumulated Motor Error

	motorAngleErrorAccumulated = motorAngleErrorAccumulated + motorAngleError * loopTimeSec;
}


