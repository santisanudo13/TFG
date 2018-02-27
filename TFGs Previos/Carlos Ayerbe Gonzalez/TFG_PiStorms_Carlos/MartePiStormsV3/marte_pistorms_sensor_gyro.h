/**
 * @file marte_pistorms_sensor_gyro.h
 * @author Carlos Ayerbe González
 * @date 8 Feb 2017
 * @brief Driver for control the Gyro of EV3 Sensor.
 * @version 1.0
 *
 * This is a C library for Raspberry Pi (RPi). It provides control over gyro sensor.
 * This sensor gives us the rotational motion on a single axis and the total rotation angle in degrees.
 * 
 */


/* Defines for MARTE_PISTORMS_SENSOR_GYRO*/
#ifndef MARTE_PISTORMS_SENSOR_GYRO
#define MARTE_PISTORMS_SENSOR_GYRO


/* Gyro ID */
#define GYRO_SENSOR_ID		"GYRO-RATE"

/* Gyro Modes */
#define ANGLE		0
#define RATE		1

/**
 * @brief Detects if the Gyro Sensor is connect correctly.
 * @param connector_id Bank and Port to plug the sensor.
 * @return  -1 if there is an error, or 1 if the sensor is correct. See \ref marte_pistorms.h  in the section of \ref PistormsCodes
 * 
 * If the Gyro sensor is connect, the function is going to return 1 if it is connect correctly
 * or -1 if it isn't.
 * 
 */
int pistorms_sensor_gyro_configure(int connector_id);

/**
 * @brief Configure the mode of the Gyro Sensor.
 * @param connector_id Bank and Port to plug the sensor.
 * @param mode it can be "ANGLE" or "RATE".
 * @return  -1 if there is an error, or 1 if the sensor is correct.  See \ref marte_pistorms.h  in the section of \ref PistormsCodes
 * 
 * 
 * Set up the mode of the sensor, it can be ANGLE or RATE.
 */
int pistorms_gyro_set_mode(int connector_id, int mode);


/**
 * @brief Read data of the Gyro Sensor depends on the mode.
 * @param connector_id Bank and Port to plug the sensor.
 * @param mode it could be "ANGLE" or "RATE".
 * @return  the angle in degrees if ANGLE or the rotational speed in degrees per second if RATE.
 * 
 * Mode Angle:
 * You can use this rotation angle to detect, for example,
 * how far your robot has turned. This feature means you are able to
 * program turns (on the axis the Gyro Sensor is measuring) with an
 * accuracy of +/- 3 degrees for a 90-degree turn.
 * 
 * Mode Rate:
 * If you rotate the Gyro Sensor in the direction
 * of the arrows on the case of the sensor, the sensor can detect
 * the rate of rotation in degrees per second. (The sensor can measure
 * a maximum rate of spin of 440 degrees per second).
 */
short pistorms_gyro_read(int connector_id, int mode);




#endif /* MARTE_PISTORMS_SENSOR_GYRO */