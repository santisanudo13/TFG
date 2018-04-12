
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ev3c.h>
#include <math.h>
#include <time.h>
#include <signal.h>

#define LEFT_MOTOR 'D'
#define RIGHT_MOTOR 'A'
#define GYRO_PORT 2

enum gyro_modes_enum {GYRO_ANG, GYRO_RATE, GYRO_FAS, GYRO_GA, GYRO_CAL};
typedef enum gyro_modes_enum gyro_mode;

static int on = 1;
static int32_t motorEncoderLeft;
static int32_t motorEncoderRight;
static int32_t motorDutyCycleLeft;
static int32_t motorDutyCycleRight;
static int32_t gyroSensorValueRaw;
static int32_t 	motorModeLeft;
static int32_t 	motorModeRight;
static int32_t gyroMode;

static void sleep_ms(int milisec)
{
	struct timespec res;
	res.tv_sec = milisec/1000;
	res.tv_nsec = (milisec*1000000) % 1000000000;
	//clock_nanosleep(CLOCK_MONOTONIC, 0, &res, NULL);
	clock_nanosleep(CLOCK_REALTIME, 0, &res, NULL);
}

static int32_t getPosition(int32_t fd)
{
	char buffer[32];

	lseek(fd,0,SEEK_SET);

	buffer[read(fd, buffer, 32)] = 0;

	return atoi(buffer);
}

static int32_t FastRead(int32_t fd)
{
	char buffer[64];

	lseek(fd,0,SEEK_SET);

	read(fd, buffer, 64);

	return atoi(buffer);
}

static void FastWrite(int32_t fd, char *value)
{
	lseek(fd,0,SEEK_SET);

	write(fd, value, strlen(value));
}

static void SetDuty(int32_t fd, int32_t duty)
{
	char buffer[32];

	sprintf(buffer, "%i", duty);

	lseek( fd, 0, SEEK_SET);
	FastWrite(fd, buffer);
}

void handler()
{
	printf("Fin\n");
	on = 0;
}

static ev3_sensor_ptr sensors = NULL;
static ev3_sensor_ptr gyro_sensor = NULL;
static ev3_motor_ptr motors = NULL;
static ev3_motor_ptr left_motor = NULL;
static ev3_motor_ptr right_motor = NULL;

int main(void)
{

	int steer_speed = 20;

	// Punteros de control

	

	// Medida del tiempo
	uint32_t WAIT_TIME_MS = 10;

	int32_t loopTimeMilliSec = 10;

	float loopTimeSec = loopTimeMilliSec/1000.0;

	time_t tLoopStart, tLoopEnd;

	// Radianes por grado
	float radiansPerDegree = M_PI/180;

	// Grados/s por unidad del giroscopio 
	int32_t degPerSecondPerRawGyroUnit = 1;

	// Radianes/s por unidad del giroscopio
	float radiansPerSecondPerRawGyroUnit = degPerSecondPerRawGyroUnit * radiansPerDegree; 

	// Grados por unidad de motor
	int32_t degPerRawMotorUnit = 1;

	// Radianes por unidad de motor
	float radiansPerRawMotorUnit = degPerRawMotorUnit * radiansPerDegree;

	// RPM por unidad de motor
	float RPMperPerRawMotorUnit = degPerRawMotorUnit * radiansPerDegree;

	// RPM por velocidad
	float RPMperPerPercentSpeed = 1.7;

	// Grados/s por velocidad
	float degPersecPerPercentSpeed = RPMperPerPercentSpeed * 360/60;

	// Radianes/s por velocidad
	float radPerSecPerPercentSpeed = degPersecPerPercentSpeed * radiansPerDegree;

	// Velocidad a la que compensamos el offset del giroscopio
	float gyroDriftCompensationRate = 0.1 * loopTimeSec * radiansPerSecondPerRawGyroUnit;

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
	int32_t left_pos, right_pos;

	int i;

	signal(SIGINT, handler);

	/***************** INICIALIZACIÓN *************/

	// // Búsqueda de sensores
	// sensors = ev3_load_sensors();

	// // Inicialización del giroscopio
	// gyro_sensor = ev3_search_sensor_by_port(sensors, GYRO_PORT);
	// if(gyro_sensor == NULL) 
	// 	printf("Gyro sensor no encontrado\n");

	// gyro_sensor = ev3_open_sensor(gyro_sensor);

/*No es necesario*/
	// Modo calibración
/*	ev3_mode_sensor(gyro_sensor, GYRO_CAL);

	while(i < 6)
	{
		ev3_update_sensor_val(gyro_sensor);
		gyroRateRaw = gyro_sensor->val_data[0].s32;
		usleep(WAIT_TIME_MS);

		if(gyroRateRaw == 0)
			i++;
	}


	sleep_ms(WAIT_TIME_MS);*/

	// // Modo velocidad angular
	// ev3_mode_sensor(gyro_sensor, GYRO_RATE);

	// gyroRateRaw = 0;

	// /* Búsqueda de Motores */
	// motors = ev3_load_motors();
	// if(motors == NULL)
 //  		printf("Motores no encontrados\n");

	// // Inicialización de los motores
	// left_motor = ev3_search_motor_by_port(motors, LEFT_MOTOR);

	// right_motor = ev3_search_motor_by_port(motors, RIGHT_MOTOR);

 // 	ev3_reset_motor(left_motor);
 //  	ev3_open_motor(left_motor);

	// ev3_reset_motor(right_motor);
 //  	ev3_open_motor(right_motor);

	// ev3_stop_command_motor_by_name(left_motor, "brake");
 //  	ev3_stop_command_motor_by_name(right_motor, "brake");

 //    ev3_set_position(left_motor, 0);
 //  	ev3_set_position(right_motor, 0);

	// ev3_command_motor_by_name(left_motor, "run-direct");
	// ev3_command_motor_by_name(right_motor, "run-direct");

	// ev3_set_duty_cycle_sp(left_motor, 0);
	// ev3_set_duty_cycle_sp(right_motor, 0);

	// sleep(2);
 
	gyroSensorValueRaw = open("/sys/class/lego-sensor/sensor2/value0", O_RDONLY);

	gyroMode = open("/sys/class/lego-sensor/sensor2/mode", O_WRONLY);

	motorEncoderLeft = open("/sys/class/tacho-motor/motor0/position", O_RDONLY);
	motorEncoderRight = open("/sys/class/tacho-motor/motor2/position", O_RDONLY);

	motorDutyCycleLeft = open("/sys/class/tacho-motor/motor0/duty_cycle_sp", O_WRONLY);
	motorDutyCycleRight = open("/sys/class/tacho-motor/motor2/duty_cycle_sp", O_WRONLY);

	motorModeLeft = open("/sys/class/tacho-motor/motor0/command", O_WRONLY);
	motorModeRight = open("/sys/class/tacho-motor/motor2/command", O_WRONLY);

	int32_t motorModeStopLeft = open("/sys/class/tacho-motor/motor0/stop_command", O_WRONLY); 
	int32_t motorModeStopRight = open("/sys/class/tacho-motor/motor2/stop_command", O_WRONLY); 

	//FastWrite(gyroMode, "GYRO-CAL");

	FastWrite(motorModeLeft, "reset");
	FastWrite(motorModeRight, "reset");

	sleep(1);
	FastWrite(motorModeStopLeft, "coast");
	FastWrite(motorModeStopRight, "coast");

	FastWrite(motorModeLeft, "run-direct");
	FastWrite(motorModeRight, "run-direct");



	FastWrite(gyroMode, "GYRO-RATE");


	// sleep(2);
 //   	printf("Lectura gyro: %d\n", FastRead(gyroSensorValueRaw));
 //   	printf("Lectura motor1: %d\n", getPosition(motorEncoderLeft));
 //   	printf("Lectura motor2: %d\n", getPosition(motorEncoderRight));
	// //printf("Lectura motor: %d\n", ev3_get_position(right_motor));
 //   	SetDuty(motorDutyCycleRight, 30);
	// sleep(1);
 //   	SetDuty(motorDutyCycleRight, 0);
	// sleep(2);
 //   	printf("Lectura motor1: %d\n", getPosition(motorEncoderLeft));
 //   	printf("Lectura motor2: %d\n", getPosition(motorEncoderRight));

	// printf("Realizada configuración inicial\n");

	// Calibración del offset del giroscopio
	int gyroRateCalibrateCount = 100;

	for(i=0; i < gyroRateCalibrateCount; i++)
	{
		//ev3_update_sensor_val(gyro_sensor);
		//gyroOffset = gyroOffset + gyro_sensor->val_data[0].s32;
		gyroOffset = gyroOffset + FastRead(gyroSensorValueRaw);
		usleep(100000);
	}               
    gyroOffset = gyroOffset/gyroRateCalibrateCount;           

    printf("Calibración del gyro. GyroOffset: %f\n", gyroOffset);


	//on = 0;
    // Ciclo de cómputo
	while(on)
	{
		time(&tLoopStart);
		////////////////////////////////////////////////////////////////
		// Lectura de gyro
		////////////////////////////////////////////////////////////////
		//ev3_update_sensor_val(gyro_sensor);
		//gyroRateRaw = gyro_sensor->val_data[0].s32;
		gyroRateRaw = FastRead(gyroSensorValueRaw);
		gyroRate = (gyroRateRaw - gyroOffset) * radiansPerSecondPerRawGyroUnit; 

		//printf("Gyro:%d\n", gyroRateRaw);

		////////////////////////////////////////////////////////////////
		// Lectura de la posición del motor
		////////////////////////////////////////////////////////////////
		//motorAngleRaw = (ev3_get_position(left_motor) + ev3_get_position(right_motor))/2;
		motorAngleRaw = (getPosition(motorEncoderLeft) + getPosition(motorEncoderRight))/2;
		motorAngle = motorAngleRaw * radiansPerRawMotorUnit;

		//printf("MotorAngle:%f\n", motorAngleRaw);
		motorAngularSpeedReference = 0 * radPerSecPerPercentSpeed;
		motorAngleReference = motorAngleReference + motorAngularSpeedReference * loopTimeSec;

		motorAngleError = motorAngle - motorAngleReference;

	    ////////////////////////////////////////////////////////////////
        //  Computing Motor Speed
      	////////////////////////////////////////////////////////////////
        motorAngularSpeed = (motorAngle - motorAngleHistory[0])/(3 * loopTimeSec);
        motorAngularSpeedError = motorAngularSpeed - motorAngularSpeedReference;
        motorAngleHistory[0] = motorAngleHistory[1];
        motorAngleHistory[1] = motorAngleHistory[2];
        motorAngleHistory[2] = motorAngle;

		////////////////////////////////////////////////////////////////
	    //  Computing the motor duty cycle value
		////////////////////////////////////////////////////////////////
        motorDutyCycle = (gainGyroAngle  				* gyroEstimatedAngle
                       + gainGyroRate   				* gyroRate
                       + gainMotorAngle 				* motorAngleError
                       + gainMotorAngularSpeed 			* motorAngularSpeedError
                       + gainMotorAngleErrorAccumulated * motorAngleErrorAccumulated);

        if(motorDutyCycle > 100)
        	motorDutyCycle = 100;
        else if(motorDutyCycle < -100)
        	motorDutyCycle = -100;

      	////////////////////////////////////////////////////////////////
      	//  Apply the signal to the motor, and add steering
		////////////////////////////////////////////////////////////////
		SetDuty(motorDutyCycleLeft, (int) motorDutyCycle);
		SetDuty(motorDutyCycleRight, (int) motorDutyCycle);
		//ev3_set_duty_cycle_sp(left_motor, (int) motorDutyCycle);
		//ev3_set_duty_cycle_sp(right_motor, (int) motorDutyCycle); 

        printf("Potencia: %d\n", (int)motorDutyCycle);

      	////////////////////////////////////////////////////////////////
     	//  Update angle estimate and Gyro Offset Estimate
      	////////////////////////////////////////////////////////////////
        gyroEstimatedAngle = gyroEstimatedAngle + gyroRate * loopTimeSec;
        gyroOffset = (1 - gyroDriftCompensationRate) * gyroOffset + gyroDriftCompensationRate * gyroRateRaw;

		////////////////////////////////////////////////////////////////
		//  Update Accumulated Motor Error
		////////////////////////////////////////////////////////////////
        motorAngleErrorAccumulated = motorAngleErrorAccumulated + motorAngleError * loopTimeSec;

		////////////////////////////////////////////////////////////////
		//  Busy wait for the loop to complete
		////////////////////////////////////////////////////////////////
        time(&tLoopEnd);

        usleep (15000);
        //while((tLoopEnd - tLoopStart) < loopTimeSec)
        //{
        //	usleep(1000);
        //	time(&tLoopEnd);
        //}	
         	

	}

	SetDuty(motorDutyCycleLeft, 0);
	SetDuty(motorDutyCycleRight, 0);
	//ev3_set_duty_cycle_sp(left_motor, 0);
	//ev3_set_duty_cycle_sp(right_motor, 0); 

	return (0);
}