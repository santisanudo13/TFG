/**
 * @file marte_pistorms_sensors.h
 * @author Carlos Ayerbe González
 * @date 6 Feb 2017
 * @brief Drivers for sensors from Pistorms + Raspberry PI model B.
 * @version 1.0
 *
 * This is a C library for Raspberry Pi (RPi). It provides control and functions
 * for using sensors in Pistorms brick. 
 */


/* Defines for MARTE_PISTORMS_SENSORS */
#ifndef MARTE_PISTORMS_SENSORS 
#define MARTE_PISTORMS_SENSORS 

/*! I2C Registers for Sensor Modes */
#define  PORT_TYPE_1	0x6F
#define  PORT_TYPE_2  	0xA3

/*! Registers for EV3 Sensors  */
#define PORT_1_READY		0x70
#define PORT_1_SENSOR_ID	0x71
#define PORT_1_MODE		0x81
#define PORT_1_DATA		0x83
#define PORT_1_DATA_RESET	0x84

#define PORT_2_READY		0xA4
#define PORT_2_SENSOR_ID	0xA5	
#define PORT_2_MODE		0xB5
#define PORT_2_DATA		0xB7
#define PORT_2_DATA_RESET	0xB8

/* Sensor Type Modes */
#define I2C_TYPE		9
#define TOUCH_TYPE		18
#define EV3_TYPE		19


/**
 * @brief Determine sensor type on the specified port.
 * @param connector_id Bank and Port to plug the Touch sensor.
 * @param type the type of the sensor which is plugged.
 * @return Pistorms codes, see \ref marte_pistorms.h  in the section of \ref PistormsCodes
 * 
 * Configure the type mode on the specified port.
 * Default Type Modes in Pistorms is EV3 ----> value 19. So it is neccesary to change this value to use
 * other types of sensors.
 * 
 * It is necessary to generate a  short time after doing the write to wait for the change in the mode.
 * This time is generate by th efunction nanosleep().
 * 		\code
 *		  struct timespec tim;
 *		  tim.tv_sec = 0;
 *		  tim.tv_nsec = 10000000;
 * 		\endcode
 */
int pistorms_port_set_type_sensor(int connector_id,int type);

/**
 * @brief Obtain the mode that the EV3 Sensor is running.
 * @param connector_id Bank and Port to plug the EV3 sensor.
 * @return value depends on the mode of the sensor(value ---> 0,1 or 2) or -1 if there is an error, see \ref marte_pistorms.h  in the section of \ref PistormsCodes
 * 
 * Gets the  mode of EV3 Sensor. 
 * Default mode is 0 for all EV3 Sensors.
 * Gyro Sensor --- 
 * 		|--->value = 0 -> ANGLE
 * 		|--->value = 1 -> RATE
 * Color Sensor --- 
 * 		  |--->value = 0 -> REFLECTED_LIGHT
 * 		  |--->value = 1 -> AMBIENT_LIGHT
 * 		  |--->value = 2 -> MEASURE_COLOR
 * Infrared Sensor --- 
 * 		    |--->value = 0 -> PROXIMITY
 * 		    |--->value = 1 -> BEACON
 * 		    |--->value = 2 -> REMOTE
 * Ultrasonic Sensor --- 
 * 		      |--->value = 0 -> PROXIMITY_CENTIMETERS
 * 		      |--->value = 1 -> PROXIMITY_INCHES
 * 		      |--->value = 2 -> PRESENCE
 */
int pistorms_sensor_get_mode(int connector_id);

/**
 * @brief Set the mode for the EV3 Sensor.
 * @param connector_id Bank and Port to plug the EV3 sensor.
 * @param mode Mode of the Sensor.
 * @return Pistorms codes, see \ref marte_pistorms.h  in the section of \ref PistormsCodes
 * 
 * Gets the  mode of EV3 Sensor. 
 * Default mode is 0 for all EV3 Sensors.
 * Gyro Sensor --- 
 * 		|--->value = 0 -> ANGLE
 * 		|--->value = 1 -> RATE
 * Color Sensor --- 
 * 		  |--->value = 0 -> REFLECTED_LIGHT
 * 		  |--->value = 1 -> AMBIENT_LIGHT
 * 		  |--->value = 2 -> MEASURE_COLOR
 * Infrared Sensor --- 
 * 		    |--->value = 0 -> PROXIMITY
 * 		    |--->value = 1 -> BEACON
 * 		    |--->value = 2 -> REMOTE
 * Ultrasonic Sensor --- 
 * 		      |--->value = 0 -> PROXIMITY_CENTIMETERS
 * 		      |--->value = 1 -> PROXIMITY_INCHES
 * 		      |--->value = 2 -> PRESENCE
 * 
 * It is necessary to generate a  short time after doing the write to wait for the change in the mode.
 * This time is generate by th efunction nanosleep().
 * 		\code
 *		  struct timespec tim;
 *		  tim.tv_sec = 0;
 *		  tim.tv_nsec = 10000000;
 * 		\endcode
 *
 */
int pistorms_sensor_set_mode(int connector_id, int mode);

/**
 * @brief Read the data of EV3 sensors.
 * @param connector_id Bank and Port to plug the EV3 sensor.
 * @return the buffer with the data which the sensor is reading.
 * 
 * Gets the data that the sensor is reading in real time.
 * The function is going to return the buffer with the information of the sensor is reading.
 * 
 */
char * pistorms_sensor_read(int connector_id);


#endif /* MARTE_PISTORMS_SENSORS */
