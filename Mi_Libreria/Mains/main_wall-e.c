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
int32_t gainGyroAngle=1156, gainGyroRate=146*6, gainMotorAngle=7, gainMotorAngularSpeed=9, gainMotorAngleErrorAccumulated=3*0;
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


// Ángulo estimado por el valor de la velocidad angular del giroscopio y el tiempo transcurrido
float gyroEstimatedAngle = 0;

// Lecturas del giroscopio y los motores
int i=0;
struct timeval stop, start;

// Medida del tiempo

int32_t loopTimeMilliSec = 5;

float loopTimeSec = 0;


struct str
{
	float value;int index;
};



/**
 * Cabaceras de metodos empleados
 */
float percentile90();
int cmp(const void *a,const void *b);
void reset();
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

	printf_debuger("\n\nInit brick\n\n");
	initBrick();
	printf_debuger("\n\nInit sensores\n\n");

	initSensors();

	pistorms_motor_reset_all_parameters(MOTORS_BANK_B);


	printf_debuger("\n\nPulsa el sensor tactil para comenzar\n\n");
	while(!pistorms_is_touched(TOUCH_ADDR)){
		i2c_delay(80);
	}
	//Hacemos que cada ciclo dure 15ms desde que comienza hasta que termine realizando un sleep relativo.
	//Con un baudrate de 50000 la raspberry, cada iteracion sin sleep son unos 6-9ms


	struct timespec periodo;
	periodo.tv_sec=0;
	periodo.tv_nsec=loopTimeMilliSec*1000000;  // periodo de 15ms

	struct timespec next_time; // hora de activación

	while(1){
		calibrateGyroAndMotorEncoders();

		// lee la hora de la primera activación de la tarea
		clock_gettime(CLOCK_MONOTONIC, &next_time);
		while(1){

			gettimeofday(&start, NULL);
			//Para poder parar el bucle en cualquier momento podemos pulsar la tecla del pulsador.
			if(pistorms_is_touched(TOUCH_ADDR)){
				i2c_delay(300);
				break;
			}

			//Comienza el bucle

			readGyroAndMotors();
			saveMotorSpeed();
			calculateMotorSpeed();
			motorsGo();
			updateGyroOffset();
			updateAcumulatedMotorError();

			incr_timespec(&next_time, &periodo);
			clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME,&next_time, NULL);

			gettimeofday(&stop, NULL);
			printf_debuger("Total time = %f miliseconds\n",(double)(stop.tv_usec - start.tv_usec) / 1000);

		}

		motorsStop();
		reset();
		while(!pistorms_is_touched(TOUCH_ADDR)){
			i2c_delay(20);
		}
	}
	return (0);
}

void reset(){
	// Medida del tiempo



	loopTimeSec = 0;


	int motor_speed = 20;

	// Radianes por grado
	radiansPerDegree = 0;

	// Grados/s por unidad del giroscopio
	degPerSecondPerRawGyroUnit = 1;

	// Radianes/s por unidad del giroscopio
	radiansPerSecondPerRawGyroUnit = 0;
	// Grados por unidad de motor
	degPerRawMotorUnit = 1;

	// Radianes por unidad de motor
	radiansPerRawMotorUnit = 0;

	// RPM por unidad de motor
	RPMperPerRawMotorUnit = 0;

	// RPM por velocidad
	RPMperPerPercentSpeed = 1.7;

	// Grados/s por velocidad
	degPersecPerPercentSpeed = 0;

	// Radianes/s por velocidad
	radPerSecPerPercentSpeed = 0;

	// Velocidad a la que compensamos el offset del giroscopio
	gyroDriftCompensationRate = 0;

	// Array que guarda las posiciones del motor anteriores
	motorAngleHistory[0] = 0;
	motorAngleHistory[1] = 0;
	motorAngleHistory[2] = 0;


	// Señal del ángulo del motor (en grados)
	motorAngleRaw = 0;

	// El ángulo del motor convertido a radianes
	motorAngle = 0;

	// El ángulo del motor de referencia. Que el robot tiende a conseguir
	motorAngleReference = 0;

	// Desviación de la medida del ángulo del motor con respecto al ángulo del motor de referencia
	motorAngleError = 0;

	// Error acumulado
	motorAngleErrorAccumulated = 0;

	// La velocidad del motor, estimada por cuanto ha girado en el tiempo transcurrido
	motorAngularSpeed = 0;

	// Velocidad de referencia
	motorAngularSpeedReference = 0;

	// Desviación de la velocidad del motor respecto a la referencia
	motorAngularSpeedError = 0;

	// La señal de "voltaje" que enviamos al motor
	motorDutyCycle = 0;

	// El valor del giroscopio en rate mode
	gyroRateRaw = 0;

	// La velocidad angular del robot medida en radianes/s
	gyroRate = 0;

	// Offset del giroscopio
	gyroOffset = 0;


	// Ángulo estimado por el valor de la velocidad angular del giroscopio y el tiempo transcurrido
	gyroEstimatedAngle = 0;

	// Lecturas del giroscopio y los motores
	i=0;

	calibrateVariables();


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
	printf_debuger("\n\nSensor Giroscopio = %d \n\n",gyroID);

	touchID =  pistorms_sensor_touch_configure(TOUCH_ADDR);
	printf_debuger("\n\nSensor Pulsador = %d \n\n",touchID);
}

void calibrateGyroAndMotorEncoders(){
	setLedsColor(0,255,0);

	pistorms_motor_reset_all_parameters(MOTORS_BANK_B);
	pistorms_motor_reset_pos(MOTOR_1);
	pistorms_motor_reset_pos(MOTOR_2);

	int gyroRateCalibrateCount = 3000;
	int i=0;
	gyroOffset = 0;
	pistorms_gyro_set_mode(GYRO_ADDR, RATE);

	for(i=0; i < gyroRateCalibrateCount; i++)
	{

		short lectura = pistorms_gyro_read(GYRO_ADDR, RATE);
		gyroOffset = gyroOffset + lectura;
		printf_debuger("Lecturas Calibrate: %d\n", lectura);

	}
	gyroOffset = gyroOffset/gyroRateCalibrateCount;
	printf_debuger("Calibración del gyro. GyroOffset: %f\n", gyroOffset);
}

void readGyroAndMotors(){
	// Lectura de gyro
	gyroRateRaw = pistorms_gyro_read(GYRO_ADDR, RATE);

	printf_debuger("\nGyro Rate Raw: %d\n", gyroRateRaw);

	gyroRate = (gyroRateRaw - gyroOffset) * radiansPerSecondPerRawGyroUnit;

	// Lectura de la posición del motor
	long pos1 = pistorms_motor_get_pos(MOTOR_1);
	long pos2 = pistorms_motor_get_pos(MOTOR_2);

	motorAngleRaw = (pos1 + pos2)/2;
	printf_debuger("Motor pos1: %ld, Motors pos2: %ld\n", pos1, pos2);
	printf_debuger("Motor Angle Raw: %f\n", motorAngleRaw);

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

//	motorDutyCycle = motorDutyCycle * -1;
	printf_debuger("Potencia Motores: %d\n", (int)motorDutyCycle);

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
	pistorms_motor_reset_pos(MOTOR_1);
	pistorms_motor_reset_pos(MOTOR_2);

	setLedsColor(255,0,0);
}



int cmp(const void *a,const void *b)
{
	struct str *a1 = (struct str *)a;
	struct str *a2 = (struct str*)b;
	if((*a1).value>(*a2).value)return 1;
	else if((*a1).value<(*a2).value)return -1;
	else return 0;
}

float percentile90(){
	struct str medidas[10];
	for(i = 0; i<10; i++){
		medidas[i].value=pistorms_gyro_read(GYRO_ADDR, RATE);
		medidas[i].index=i;
	}
	//ordena de menor a mayor
	qsort(medidas,10,sizeof(medidas[0]),cmp);
	for(i = 0; i<10; i++){
		printf_debuger("Medida %d: %f\n", i, medidas[i].value);
	}
	int index = (int)ceil(((double)90 / (double)100) * (double)10);
	printf_debuger("Index %d\n", index-1);
	return medidas[index-1].value;
}
