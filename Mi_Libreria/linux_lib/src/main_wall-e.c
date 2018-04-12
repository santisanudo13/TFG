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
#define ULTRASONIC_ADDR BANK_A_PORT_1
#define GYRO_ADDR BANK_A_PORT_2
#define TOUCH_ADDR BANK_B_PORT_1
/**
 * Motores
 */
#define MOTOR_1 BANK_A_PORT_1
#define MOTOR_2 BANK_A_PORT_2

#define MOTORS_BANK_A BANK_A



/**
 * Variables globales
 */




// Medida del tiempo

int loopTimeMilliSec = 10;

float loopTimeSec = 0;

time_t tLoopStart, tLoopEnd;

int motor_speed = 20;
#define M_PI 3.14159265358979323846

// Radianes por grado
float radiansPerDegree = 0;

// Grados/s por unidad del giroscopio
int degPerSecondPerRawGyroUnit = 1;

// Radianes/s por unidad del giroscopio
float radiansPerSecondPerRawGyroUnit = 0;
// Grados por unidad de motor
int degPerRawMotorUnit = 1;

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
int gainGyroAngle=1156, gainGyroRate=146, gainMotorAngle=7, gainMotorAngularSpeed=9, gainMotorAngleErrorAccumulated=3;
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
int gyroRateRaw = 0;

// La velocidad angular del robot medida en radianes/s
float gyroRate = 0;

// Offset del giroscopio
float gyroOffset = 0;

float diff;

// Ángulo estimado por el valor de la velocidad angular del giroscopio y el tiempo transcurrido
float gyroEstimatedAngle = 0;

// Lecturas del giroscopio y los motores
int left_pos, right_pos;






/**
 * Cabaceras de metodos empleados
 */

void calibrateVariables();
void setLedsColor(int red, int green, int blue);
void initBrick();
void initSensors();
void calibrateGyro();
void readGyroAndMotors();
void saveMotorSpeed();
void calculateMotorSpeed();
void motorsGo();
void updateGyroOffset();
void updateAcumulatedMotorError();


int main(){
	calibrateVariables();

	printf_debuger("\n\nInit brick\n\n");
	initBrick();
	printf_debuger("\n\nInit sensores\n\n");

	initSensors();

	pistorms_motor_reset_all_parameters(MOTORS_BANK_A);

	printf_debuger("\n\nPulsa el sensor tactil para comenzar\n\n");
	while(!pistorms_is_touched(TOUCH_ADDR)){
		i2c_delay(80);
	}

	calibrateGyro();

	while(!pistorms_is_touched(TOUCH_ADDR)){
		//Comienza el bucle
		time(&tLoopStart);


		readGyroAndMotors();


		saveMotorSpeed();


		calculateMotorSpeed();

		motorsGo();

		updateGyroOffset();

		updateAcumulatedMotorError();

		//  Busy wait for the loop to complete
		time(&tLoopEnd);

		i2c_delay(15);
	}

	pistorms_motor_brake_sync(MOTORS_BANK_A);
	pistorms_motor_reset_all_parameters(MOTORS_BANK_A);

	return (0);


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
	setLedsColor(255,0,255);

	int touchID = 0;
	int ultraID = 0;
	int gyroID = 0;

	ultraID =  pistorms_sensor_ultrasonic_configure(ULTRASONIC_ADDR);
	printf_debuger("\n\nSensor Ultrasonido = %d \n\n",ultraID);

	gyroID =  pistorms_sensor_gyro_configure(GYRO_ADDR);
	printf_debuger("\n\nSensor Giroscopio = %d \n\n",gyroID);

	touchID =  pistorms_sensor_touch_configure(TOUCH_ADDR);
	printf_debuger("\n\nSensor Pulsador = %d \n\n",touchID);

}

void calibrateGyro(){
	setLedsColor(255,255,0);

	int gyroRateCalibrateCount = 100;
	int i=0;

	pistorms_gyro_set_mode(GYRO_ADDR, RATE);

	for(i=0; i < gyroRateCalibrateCount; i++)
	{

		gyroOffset = gyroOffset + pistorms_gyro_read(GYRO_ADDR, RATE);
		i2c_delay(100);
	}
	gyroOffset = gyroOffset/gyroRateCalibrateCount;
	printf("Calibración del gyro. GyroOffset: %f\n", gyroOffset);
}

void readGyroAndMotors(){
	// Lectura de gyro
	gyroRateRaw = pistorms_gyro_read(GYRO_ADDR, RATE);
	gyroRate = (gyroRateRaw - gyroOffset) * radiansPerSecondPerRawGyroUnit;

	// Lectura de la posición del motor
	motorAngleRaw = (pistorms_motor_get_pos(MOTOR_1) + pistorms_motor_get_pos(MOTOR_2))/2;
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
	//  Computing the motor duty cycle value
	motorDutyCycle = (gainGyroAngle  				* gyroEstimatedAngle
			+ gainGyroRate   				* gyroRate
			+ gainMotorAngle 				* motorAngleError
			+ gainMotorAngularSpeed 			* motorAngularSpeedError
			+ gainMotorAngleErrorAccumulated * motorAngleErrorAccumulated);

	if(motorDutyCycle > 100)
		motorDutyCycle = 100;
	else if(motorDutyCycle < -100)
		motorDutyCycle = -100;
}

void motorsGo(){
	//  Apply the signal to the motor, and add steering
	pistorms_motor_reset_all_parameters(MOTORS_BANK_A);

	pistorms_motor_set_speed(MOTOR_1, motorDutyCycle);
	pistorms_motor_set_speed(MOTOR_2, motorDutyCycle);

	pistorms_motor_go(MOTOR_1 ,SPEED_GO);
	pistorms_motor_go(MOTOR_2 ,SPEED_GO);

	setLedsColor(0,255,0);

	printf("Potencia Motores: %d\n", (int)motorDutyCycle);
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


