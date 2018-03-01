/**
 * @file marte_pistorms_sensors.c
 * @author Carlos Ayerbe González
 * @date 6 Feb 2017
 * @brief Drivers for sensors from Pistorms + Raspberry PI model B.
 * @version 1.0
 * 
 */

#include "linux_pistorms_sensors.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include "i2c_layer.h"
#include "linux_pistorms.h"

//#define DBG
#include "linux_pistorms_internal.h"

char sensor_data[32] = {0};
int data;

int file;


/*
 * Configure the Sensor Type Modes.
 * */
int pistorms_port_set_type_sensor(int connector_id, int type){

	struct timespec tim;
	tim.tv_sec = 0;
	tim.tv_nsec = 10000000;

	char bufType[2];
	bufType[0] = 0;
	bufType[1] = type;	/**< Value of Type mode */

	int check = _set_active_bank(connector_id);

	if((connector_id == BANK_A_PORT_1) ||(connector_id == BANK_B_PORT_1)){
		bufType[0] = PORT_TYPE_1;

	}else if((connector_id == BANK_A_PORT_2) ||(connector_id == BANK_B_PORT_2)){
		bufType[0] = PORT_TYPE_2;

	} else {
		return check;

	}


	i2c_write(file, bufType[0],type);
	nanosleep(&tim,NULL);

	return check;


}

/*
 * Obtains the mode of EV3 Sensor Modes (Color,Gyro,Infrared,Proximiy).
 * */
int pistorms_sensor_get_mode(int connector_id){
	int value;
	char* bufSensorMode;
	int sensorMode;

	int check = _set_active_bank(connector_id);

	if((connector_id == BANK_A_PORT_1) ||(connector_id == BANK_B_PORT_1)){
		sensorMode = PORT_1_MODE;

	}else if((connector_id == BANK_A_PORT_2) ||(connector_id == BANK_B_PORT_2)){
		sensorMode = PORT_2_MODE;

	} else {
		return check;

	}

	bufSensorMode = i2c_read(file, sensorMode,1);/**< Read number of bytes of the requerided register*/
	value = bufSensorMode[0];
	return value;

}


/*
 * Sets the mode of EV3 Sensor Modes (Color,Gyro,Infrared,Proximiy).
 * */
int pistorms_sensor_set_mode(int connector_id, int mode){

	struct timespec tim;
	tim.tv_sec = 0;
	tim.tv_nsec = 10000000;

	char bufMode[2];
	bufMode[0] =0;
	bufMode[1] = mode;

	int check = _set_active_bank(connector_id);

	if((connector_id == BANK_A_PORT_1) ||(connector_id == BANK_B_PORT_1)){
		bufMode[0] = PORT_1_MODE;

	}else if((connector_id == BANK_A_PORT_2) ||(connector_id == BANK_B_PORT_2)){
		bufMode[0] = PORT_2_MODE;

	} else {
		return check;

	}

	i2c_write(file, bufMode[0],mode);
	nanosleep(&tim,NULL);
	return check;

}  

/*
 * Reads the data of EV3 sensors.
 * */
char * pistorms_sensor_read(int connector_id){

	int sensorData;

	_set_active_bank(connector_id);

	if((connector_id == BANK_A_PORT_1) ||(connector_id == BANK_B_PORT_1)){

		sensorData = PORT_1_DATA;

	}else if((connector_id == BANK_A_PORT_2) ||(connector_id == BANK_B_PORT_2)){

		sensorData = PORT_2_DATA;

	} else {

		return PISTORMS_ERROR_BAD_CONNECTOR;

	}
	char* value;
	value = i2c_read(file, sensorData,4);
	return value;


}
