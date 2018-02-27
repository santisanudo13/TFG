/**
 * @file marte_pistorms_sensor_ultrasonic.h
 * @author Carlos Ayerbe González
 * @date 8 Feb 2017
 * @brief Driver for control the Ultrasonic of EV3 Sensor.
 * @version 1.0
 *
 * This is a C library for Raspberry Pi (RPi). It provides control over ultrasonic sensor.
 * This sensor gives us the distance to an object in front of it. It does this by sending out
 * high-frequency sound waves and measuring how long it takes
 * the sound to reflect back to the sensor.(It could be in centimeters or in inches).
 * Also this sensor can detect another Ultrasonic Sensor operating nearby.
 * When listening for presence, the sensor detects sound signals but does not send them.
 * 
 */


/* Defines for MARTE_PISTORMS_SENSOR_ULTRASONIC*/
#ifndef MARTE_PISTORMS_SENSOR_ULTRASONIC
#define MARTE_PISTORMS_SENSOR_ULTRASONIC


/* Ultrasonic ID */
#define ULTRASONIC_SENSOR_ID	"US-DIST-CM"

/* Ultrasonic Modes */
#define PROXIMITY_CENTIMETERS	0
#define PROXIMITY_INCHES	1
#define PRESENCE		2

/*Parameters to use the function*/
#define CENTIMETERS 0
#define INCHES 1

/**
 * @brief Detects if the Ultrasonic Sensor is connect correctly.
 * @param connector_id Bank and Port to plug the sensor.
 * @return  -1 if there is an error, or 1 if the sensor is correct.  See \ref marte_pistorms.h  in the section of \ref PistormsCodes
 * 
 * If the Ultrasonic sensor is connect, the function is going to return 1 if it is connect correctly
 * or -1 if it isn't.
 * 
 */
int pistorms_sensor_ultrasonic_configure(int connector_id);

/**
 * @brief Configure the mode of the Ultrasonic Sensor.
 * @param connector_id Bank and Port to plug the sensor.
 * @param mode it can be "CENTIMETERS", "INCHES" or PRESENCE.
 * @return  -1 if there is an error, or 1 if the sensor is correct.  See \ref marte_pistorms.h  in the section of \ref PistormsCodes
 * 
 * 
 * Set up the mode of the sensor, it can be CENTIMETERS, INCHES or PRESENCE.
 */
int pistorms_ultrasonic_set_mode(int connector_id, int mode);

/**
 * @brief Ultrasonic sensor can measure the distance to an object in front of it.
 * @param connector_id Bank and Port to plug the sensor.
 * @param mode it can be "CENTIMETERS" or "INCHES".
 * @return  distance to an object in centimeters or inches.
 * 
 * (Centimeters)
 * The detectable distance is between 3 and 250 centimeters
 * (with an accuracy of +/- 1 centimeters).
 * A value of 255 centimeters means the sensor is not able to detect any 
 * object in front of it.
 * 
 * (Inches)
 * The measurable distance is between 1 and 99 inches 
 * (with an accuracy of +/- 0.394 inches).
 * A value of 100 inches means the sensor is not able to detect any 
 * object in front of it.
 * 
 */
float pistorms_ultrasonic_read_distance(int connector_id, int mode);

/**
 * @brief Ultrasonic sensor can detect another Ultrasonic Sensor operating nearby.
 * @param connector_id Bank and Port to plug the sensor.
 * @return  1 if there is another Ultrasonic sensor, or 0 if not.
 * 
 * In Presence Mode, this sensor can detect another Ultrasonic Sensor
 * operating nearby. When listening for presence, the sensor detects
 * sound signals but does not send them.
 */
int pistorms_ultrasonic_presence(int connector_id);


#endif /* MARTE_PISTORMS_SENSOR_GYRO */