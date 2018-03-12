/**
 * @file linux_pistorms_sensor_ultrasonic.c
 * @author Santiago Sa�udo Mart�nez
 * @date 5 Mar 2018
 * @brief Driver for control the Ultrasonic of EV3 Sensor.
 * @version 1.0
 *
 */


#include "linux_pistorms_sensor_ultrasonic.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "i2c_layer.h"
#include "linux_pistorms.h"


//#define DBG
#include "linux_pistorms_internal.h"
#include "linux_pistorms_sensors.h"


char *read_data;
static int ultrasonic_mode = PROXIMITY_CENTIMETERS;
static int presence_mode = -1;

/*
 * Detects if the Ultrasonic Sensor is connect correctly.
 * */
int pistorms_sensor_ultrasonic_configure(int connector_id){


	pistorms_port_set_type_sensor(connector_id,EV3_TYPE);

	char* ULTRASONIC_id;

	ULTRASONIC_id = pistorms_get_device_id(connector_id);


	if(strcmp(ULTRASONIC_id, ULTRASONIC_SENSOR_ID) != 0){

		printf_dbg("Error ID: %s",ULTRASONIC_id);
		return PISTORMS_ERROR_SENSOR_ID;

	}else{

		printf_dbg("ID Correcto: %s", ULTRASONIC_id);
		return PISTORMS_REASON_OK;

	}


}

/*
 * Configure the mode of the Ultrasonic Sensor.
 * */
int pistorms_ultrasonic_set_mode(int connector_id, int mode){

	int check = pistorms_sensor_set_mode(connector_id,mode);

	if (mode == CENTIMETERS){

		ultrasonic_mode = PROXIMITY_CENTIMETERS;

	}else if (mode == INCHES){

		ultrasonic_mode = PROXIMITY_INCHES;

	}else if (mode == PRESENCE){

		presence_mode = PRESENCE;

	}else{

		printf_dbg("ERROR MODE");
		return PISTORMS_ERROR_SENSOR_MODE;
	}
	return check;

}

/*
 * Detect another Ultrasonic Sensor operating nearby.
 * */
int pistorms_ultrasonic_presence(int connector_id){

	int value = 0;
	char *presence;

	if(presence_mode != PRESENCE){

		pistorms_sensor_set_mode(connector_id,PRESENCE);
		presence_mode = PRESENCE;

	}
	presence = pistorms_sensor_read(connector_id);
	value = presence[0];
	return value;
}

/*
 * Obtain the measure of  distance to an object in front of it.
 * */
float pistorms_ultrasonic_read_distance(int connector_id, int mode){

	float value_final;

	if(mode != ultrasonic_mode){

		if(mode == PROXIMITY_CENTIMETERS){

			printf_dbg("First CM");
			pistorms_sensor_set_mode(connector_id,PROXIMITY_CENTIMETERS);
			ultrasonic_mode = PROXIMITY_CENTIMETERS;

		}else if(mode == INCHES){

			printf_dbg("First Inches");
			pistorms_sensor_set_mode(connector_id,PROXIMITY_INCHES);
			ultrasonic_mode = PROXIMITY_INCHES;

		}else{

			printf_dbg("ERROR MODE");
			return PISTORMS_ERROR_WRONG_CONNECTOR_ID;
		}
	}

	read_data = pistorms_sensor_read(connector_id);
	unsigned int val =  read_data[0] + (read_data[1] << 8);

	value_final = (float)val/10.0;
	return value_final;
}

